/* speed - Generic C++ library.
 * Copyright (C) 2015-2024 Killian Valverde.
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
 * @file        operations.cpp
 * @brief       process operations source.
 * @author      Killian Valverde
 * @date        2017/01/08
 */

#include "../../../compatibility/compatibility.hpp"
#ifdef SPEED_GLIBC

#include "operations.hpp"

#include <cstring>

namespace speed::system::api::glibc::process {

bool execute_command(
        const char* cmd,
        int* return_val,
        std::error_code* err_code
) noexcept
{
    while (isspace(*cmd))
    {
        ++cmd;
    }
    
    ::pid_t pid = ::fork();
    
    switch (pid)
    {
        case -1:
            system::errors::assign_system_error_code(errno, err_code);
            return false;
        
        case 0:
        {
            constexpr const std::size_t initial_argv_max = 1024;
            std::size_t max_argv = initial_argv_max;
            const char* initial_argv[initial_argv_max] = {nullptr};
            const char** argv = initial_argv;
            std::size_t cmd_len = ::strlen(cmd);
            std::size_t cur_argv = 0;
            std::size_t lst_cursor = 0;
            std::size_t lst_slash = 0;
            bool esc = false;
            bool double_qut_open = false;
            bool first_arg = true;
            
            for (std::size_t i = 0; i < cmd_len; ++i)
            {
                // Obtenemos el índice del último slash encontrado.
                if (i != 0 &&
                    cmd[i - 1] == '/' &&
                    (double_qut_open || !isspace(cmd[i])) &&
                    (!double_qut_open || cmd[i] != '\"'))
                {
                    lst_slash = i;
                }
                
                // Tomamos en cuenta las secuencias de escape.
                if (cmd[i] == '\\' && !esc)
                {
                    esc = true;
                    if (i + 1 < cmd_len)
                    {
                        continue;
                    }
                }
                
                // Tomamos en cuenta las double quote.
                if (cmd[i] == '"' && !esc)
                {
                    if (!double_qut_open)
                    {
                        lst_cursor = i + 1;
                        double_qut_open = true;
                        if (i + 1 < cmd_len)
                        {
                            continue;
                        }
                    }
                }
                
                // Si hay un corte.
                if ((!esc && !double_qut_open && isspace(cmd[i])) ||
                    (!esc && double_qut_open && cmd[i] == '\"') ||
                    i + 1 == cmd_len)
                {
                    // Si el array actual es demasiado pequeño lo agrandamos teniendo en cuenta la
                    // small string optimization realizada.
                    if (cur_argv == max_argv)
                    {
                        max_argv *= 2;
                        auto** nptr = (const char**)realloc(argv == initial_argv ? nullptr : argv,
                                                            max_argv * sizeof (const char*));
                        if (nptr == nullptr)
                        {
                            goto alloc_fail;
                        }
                        if (argv == initial_argv)
                        {
                            memcpy(nptr, argv, cur_argv * sizeof(const char*));
                        }
                        argv = nptr;
                    }
                    
                    // Si el caracter actual es el último y no es un espacio en blanco y tampoco una
                    // comilla, lo tomamos en cuenta.
                    if (i + 1 == cmd_len &&
                        (esc || !isspace(cmd[i])) &&
                        (esc || cmd[i] != '\"'))
                    {
                        ++i;
                    }

                    // Tomamos memoria para la cadena actual.
                    argv[cur_argv] = (const char*)calloc(1, i - lst_cursor + 1);
                    if (argv[cur_argv] == nullptr)
                    {
                        goto alloc_fail;
                    }
                    
                    // Realizamos la copia de la cadena de caracteres.
                    memcpy((void*)argv[cur_argv], &cmd[lst_cursor], i - lst_cursor);
                    ++cur_argv;

                    // Si es el primer argumento entonces el nombre del programa debe de ser tomado
                    // como parámetro.
                    if (first_arg)
                    {
                        argv[cur_argv] = (const char*)calloc(1, i - lst_slash);
                        if (argv[cur_argv] == nullptr)
                        {
                            goto alloc_fail;
                        }
                        memcpy((void*)argv[cur_argv], &cmd[lst_slash], i - lst_slash);
                        ++cur_argv;
                        first_arg = false;
                    }
                    
                    // No tomamos en cuenta los espacios sobrantes.
                    while (i + 1 < cmd_len && isspace(cmd[i + 1]))
                    {
                        ++i;
                    }
                    lst_cursor = i + 1;

                    double_qut_open = false;
                }
                
                // La secuencia de escape ha sido tomada en cuenta.
                esc = false;
            }
            
            // Ejecutamos el proceso.
            argv[cur_argv] = nullptr;
            ::execvp(argv[0], (char**)&argv[1]);
            
alloc_fail:
            for (std::size_t i = 0; i < max_argv; ++i)
            {
                if (argv[i] != nullptr)
                {
                    free((void*)argv[i]);
                }
            }
            if (argv != initial_argv)
            {
                free(argv);
            }
            ::_exit(-1);
        }
        
        default:
        {
            int status;
            while (::waitpid(pid, &status, 0) == -1)
            {
                if (errno != EINTR)
                {
                    system::errors::assign_system_error_code(errno, err_code);
                    return false;
                }
            }
            if (return_val != nullptr)
            {
                *return_val = WEXITSTATUS(status);
            }
            return true;
        }
    }
}

system::process::pid_t get_pid() noexcept
{
    return ::getpid();
}

system::process::ppid_t get_ppid() noexcept
{
    return ::getppid();
}

system::process::uid_t get_uid() noexcept
{
    return ::getuid();
}

system::process::gid_t get_gid() noexcept
{
    return ::getgid();
}

bool nanosleep(
        std::uint64_t sec,
        std::uint64_t nsec,
        std::error_code* err_code
) noexcept
{
    ::timespec tm;
    ::timespec rm_tm;
    
    tm.tv_sec = sec;
    tm.tv_nsec = nsec;
    
    if (::nanosleep(&tm, &rm_tm) == -1)
    {
        system::errors::assign_system_error_code(errno, err_code);
        return false;
    }
    
    return true;
}

}

#endif
