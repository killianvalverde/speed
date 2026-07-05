/* speed - Generic C++ library.
 * Copyright (C) 2015-2026 Killian Valverde.
 *
 * This file is part of speed.
 *
 * speed is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * speed is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with speed. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file operations.cpp
 * @brief Core operations implementation for the system::process submodule.
 * @author Killian Valverde
 * @date 2017-01-08
*/

#include "../../../platform/platform.hpp"
#include "../../operations.hpp"

#include <cstring>
#include <limits>

#include <fcntl.h>
#include <sys/resource.h>
#include <sys/wait.h>

#include "../../../errors/errors.hpp"

namespace speed::system::process {

static void free_command_line(char** argv) noexcept
{
    if (argv == nullptr)
    {
        return;
    }

    for (std::size_t i = 0; argv[i] != nullptr; ++i)
    {
        std::free(argv[i]);
    }

    std::free(argv);
}

static bool parse_command_line(const char* cmd, char*** argv, std::error_code* err_code) noexcept
{
    // TODO(Killian): Fix bug where the escape character is copied into the resulting string.

    constexpr std::size_t initial_argv_capacity = 16;

    const std::size_t cmd_length = std::strlen(cmd);
    std::size_t argv_capacity = initial_argv_capacity;
    std::size_t argc = 0;
    std::size_t token_begin = 0;
    std::size_t last_slash = 0;
    bool escaped = false;
    bool quote_open = false;
    bool first_argument = true;

    char** argv_buffer = static_cast<char**>(std::calloc(argv_capacity, sizeof(char*)));
    if (argv_buffer == nullptr)
    {
        errors::assign_errc(std::errc::not_enough_memory, err_code);
        return false;
    }

    for (std::size_t i = 0; i < cmd_length; ++i)
    {
        // Obtenemos el índice del último slash encontrado.
        if (i != 0 &&
            cmd[i - 1] == '/' &&
            (quote_open || !std::isspace(static_cast<unsigned char>(cmd[i]))) &&
            (!quote_open || cmd[i] != '"'))
        {
            last_slash = i;
        }

        // Tomamos en cuenta las secuencias de escape.
        if (cmd[i] == '\\' && !escaped)
        {
            escaped = true;
            if (i + 1 < cmd_length)
            {
                continue;
            }
        }

        // Tomamos en cuenta las double quote.
        if (cmd[i] == '"' && !escaped && !quote_open)
        {
            token_begin = i + 1;
            quote_open = true;

            if (i + 1 < cmd_length)
            {
                continue;
            }
        }

        // Si hay un corte.
        if ((!escaped && !quote_open && std::isspace(static_cast<unsigned char>(cmd[i]))) ||
            (!escaped && quote_open && cmd[i] == '"') ||
            i + 1 == cmd_length)
        {
            const std::size_t new_entries = first_argument ? 2 : 1;

            // Si el array actual es demasiado pequeño lo agrandamos.
            if (argc + new_entries + 1 > argv_capacity)
            {
                while (argc + new_entries + 1 > argv_capacity)
                {
                    argv_capacity *= 2;
                }

                auto new_argv = static_cast<char**>(std::realloc(argv_buffer,
                    argv_capacity * sizeof(char*)));

                if (new_argv == nullptr)
                {
                    free_command_line(argv_buffer);
                    errors::assign_errc(std::errc::not_enough_memory, err_code);
                    return false;
                }

                argv_buffer = new_argv;
            }

            // Si el caracter actual es el último y no es un espacio en blanco y tampoco una
            // comilla, lo tomamos en cuenta.
            if (i + 1 == cmd_length &&
                (escaped || !std::isspace(static_cast<unsigned char>(cmd[i]))) &&
                (escaped || cmd[i] != '"'))
            {
                ++i;
            }

            const std::size_t token_size = i - token_begin;

            // Tomamos memoria para la cadena actual.
            argv_buffer[argc] = static_cast<char*>(std::calloc(token_size + 1, 1));
            if (argv_buffer[argc] == nullptr)
            {
                free_command_line(argv_buffer);
                errors::assign_errc(std::errc::not_enough_memory, err_code);
                return false;
            }

            // Realizamos la copia de la cadena de caracteres.
            std::memcpy(argv_buffer[argc], &cmd[token_begin], token_size);
            ++argc;

            // Si es el primer argumento entonces el nombre del programa debe de ser tomado
            // como parámetro.
            if (first_argument)
            {
                const std::size_t basename_size = i - last_slash;

                argv_buffer[argc] = static_cast<char*>(std::calloc(basename_size + 1, 1));
                if (argv_buffer[argc] == nullptr)
                {
                    free_command_line(argv_buffer);
                    errors::assign_errc(std::errc::not_enough_memory, err_code);
                    return false;
                }

                std::memcpy(argv_buffer[argc], &cmd[last_slash], basename_size);
                ++argc;
                first_argument = false;
            }

            // No tomamos en cuenta los espacios sobrantes.
            while (i + 1 < cmd_length && std::isspace(static_cast<unsigned char>(cmd[i + 1])))
            {
                ++i;
            }

            token_begin = i + 1;
            quote_open = false;
        }

        // La secuencia de escape ha sido tomada en cuenta.
        escaped = false;
    }

    if (argc == 0)
    {
        free_command_line(argv_buffer);
        errors::assign_errc(std::errc::invalid_argument, err_code);
        return false;
    }

    argv_buffer[argc] = nullptr;
    *argv = argv_buffer;
    return true;
}

bool execute(const char* cmd, execution_result* res, std::error_code* err_code) noexcept
{
    constexpr std::uint64_t nsec_per_usec = 1'000ULL;
    constexpr std::uint64_t nsec_per_sec = 1'000'000'000ULL;

    timespec start_ts{};
    timespec end_ts{};
    int exec_pipe[2] = {-1, -1};

    if (cmd == nullptr)
    {
        errors::assign_errc(std::errc::invalid_argument, err_code);
        return false;
    }
    while (std::isspace(static_cast<unsigned char>(*cmd)))
    {
        ++cmd;
    }
    if (*cmd == '\0')
    {
        errors::assign_errc(std::errc::invalid_argument, err_code);
        return false;
    }

    if (pipe(exec_pipe) == -1)
    {
        errors::assign_errno_error_code(errno, err_code);
        return false;
    }
    if (fcntl(exec_pipe[0], F_SETFD, FD_CLOEXEC) == -1 ||
        fcntl(exec_pipe[1], F_SETFD, FD_CLOEXEC) == -1 ||
        clock_gettime(CLOCK_MONOTONIC, &start_ts) == -1)
    {
        errors::assign_errno_error_code(errno, err_code);
        close(exec_pipe[0]);
        close(exec_pipe[1]);
        return false;
    }

    switch (const ::pid_t pid = fork())
    {
    case -1:
        errors::assign_errno_error_code(errno, err_code);
        close(exec_pipe[0]);
        close(exec_pipe[1]);
        return false;

    case 0:
        {
            char** argv = nullptr;
            std::error_code child_err;

            auto notify_and_exit = [](const int error_val, int pipe_fd)
            {
                for (ssize_t written = 0; written < static_cast<ssize_t>(sizeof(error_val));)
                {
                    const ssize_t r = write(pipe_fd,
                        reinterpret_cast<const char*>(&error_val) + written,
                        sizeof(error_val) - static_cast<std::size_t>(written));

                    if (r == -1 && errno != EINTR)
                    {
                        break;
                    }
                    if (r > 0)
                    {
                        written += r;
                    }
                }
                _exit(127);
            };

            close(exec_pipe[0]);

            if (!parse_command_line(cmd, &argv, &child_err))
            {
                notify_and_exit(child_err.value(), exec_pipe[1]);
            }

            execvp(argv[0], &argv[1]);
            const int e = errno;
            free_command_line(argv);
            notify_and_exit(e, exec_pipe[1]);
        }

    default:
        {
            int status;
            rusage usage{};
            int exec_errno = 0;
            ssize_t bytes_read = 0;

            close(exec_pipe[1]);

            // TODO(Killian): This implementation may have an invalid usage if the function is used
            //  concurrently
            if (wait4(pid, &status, 0, &usage) == -1)
            {
                errors::assign_errno_error_code(errno, err_code);
                close(exec_pipe[0]);
                return false;
            }

            if (clock_gettime(CLOCK_MONOTONIC, &end_ts) == -1)
            {
                errors::assign_errno_error_code(errno, err_code);
                close(exec_pipe[0]);
                return false;
            }

            while (bytes_read < static_cast<ssize_t>(sizeof(exec_errno)))
            {
                const ssize_t r = read(exec_pipe[0],
                    reinterpret_cast<char*>(&exec_errno) + bytes_read,
                    sizeof(exec_errno) - static_cast<std::size_t>(bytes_read));

                if (r == 0)
                {
                    break;
                }
                if (r == -1 && errno != EINTR)
                {
                    break;
                }
                if (r > 0)
                {
                    bytes_read += r;
                }
            }

            close(exec_pipe[0]);

            if (bytes_read == static_cast<ssize_t>(sizeof(exec_errno)))
            {
                errors::assign_errno_error_code(exec_errno, err_code);
                return false;
            }

            if (res != nullptr)
            {
                res->exit_code = WIFEXITED(status) ? WEXITSTATUS(status) :
                    WIFSIGNALED(status) ? 128 + WTERMSIG(status) : -1;

                res->user_cpu_time
                    .set_seconds(usage.ru_utime.tv_sec)
                    .set_nanoseconds(static_cast<std::uint64_t>(usage.ru_utime.tv_usec) *
                        nsec_per_usec);

                res->kernel_cpu_time
                    .set_seconds(usage.ru_stime.tv_sec)
                    .set_nanoseconds(static_cast<std::uint64_t>(usage.ru_stime.tv_usec) *
                        nsec_per_usec);

                time_t elapsed_sec = end_ts.tv_sec - start_ts.tv_sec;
                long elapsed_nsec = end_ts.tv_nsec - start_ts.tv_nsec;
                if (elapsed_nsec < 0)
                {
                    --elapsed_sec;
                    elapsed_nsec += static_cast<long>(nsec_per_sec);
                }

                res->elapsed_time
                    .set_seconds(elapsed_sec)
                    .set_nanoseconds(static_cast<std::uint64_t>(elapsed_nsec));
            }

            return true;
        }
    }
}

pid_t get_pid() noexcept
{
    return ::getpid();
}

user_id get_uid() noexcept
{
    return user_id(::getuid());
}

bool nanosleep(std::uint64_t sec, std::uint64_t nsec, std::error_code* err_code) noexcept
{
    constexpr std::uint64_t nsec_per_sec = 1'000'000'000ULL;

    if (nsec >= nsec_per_sec ||
        sec > static_cast<std::uint64_t>(std::numeric_limits<time_t>::max()))
    {
        errors::assign_errc(std::errc::invalid_argument, err_code);
        return false;
    }

    timespec tm{
        .tv_sec = static_cast<time_t>(sec),
        .tv_nsec = static_cast<long>(nsec)
    };

    if (::nanosleep(&tm, nullptr) == -1)
    {
        errors::assign_errno_error_code(errno, err_code);
        return false;
    }

    return true;
}

}
