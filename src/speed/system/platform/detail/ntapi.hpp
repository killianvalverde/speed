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
 * @file ntapi.hpp
 * @brief Windows NT API compatibility declarations and definitions.
 * @author Killian Valverde
 * @date 2026-05-26
 */

#pragma once

#include "../platform.hpp"

#ifdef SPEED_WINAPI

#include <windows.h>
#include <winternl.h>

/** @cond */
#ifndef STATUS_NO_MORE_FILES
#define STATUS_NO_MORE_FILES ((NTSTATUS)0x80000006L)
#endif

extern "C" NTSYSCALLAPI NTSTATUS NTAPI NtOpenFile(
    PHANDLE FileHandle,
    ACCESS_MASK DesiredAccess,
    POBJECT_ATTRIBUTES ObjectAttributes,
    PIO_STATUS_BLOCK IoStatusBlock,
    ULONG ShareAccess,
    ULONG OpenOptions
);

extern "C" NTSYSCALLAPI NTSTATUS NTAPI NtClose(
    HANDLE Handle
);

extern "C" NTSYSCALLAPI NTSTATUS NTAPI NtQueryDirectoryFile(
    HANDLE FileHandle,
    HANDLE Event,
    PIO_APC_ROUTINE ApcRoutine,
    PVOID ApcContext,
    PIO_STATUS_BLOCK IoStatusBlock,
    PVOID FileInformation,
    ULONG Length,
    FILE_INFORMATION_CLASS FileInformationClass,
    BOOLEAN ReturnSingleEntry,
    PUNICODE_STRING FileName,
    BOOLEAN RestartScan
);

extern "C" NTSYSCALLAPI ULONG NTAPI RtlNtStatusToDosError(
    NTSTATUS Status
);

namespace speed::system::filesystem::detail {

constexpr int FileIdBothDirectoryInformation_ = 37;

struct file_id_both_dir_information
{
    ULONG NextEntryOffset;
    ULONG FileIndex;
    LARGE_INTEGER CreationTime;
    LARGE_INTEGER LastAccessTime;
    LARGE_INTEGER LastWriteTime;
    LARGE_INTEGER ChangeTime;
    LARGE_INTEGER EndOfFile;
    LARGE_INTEGER AllocationSize;
    ULONG FileAttributes;
    ULONG FileNameLength;
    ULONG EaSize;
    CCHAR ShortNameLength;
    WCHAR ShortName[12];
    LARGE_INTEGER FileId;
    WCHAR FileName[1];
};

struct reparse_data_buffer
{
    ULONG ReparseTag;
    USHORT ReparseDataLength;
    USHORT Reserved;
};

}
/** @endcond */

#endif
