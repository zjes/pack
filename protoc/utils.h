/* =========================================================================================================================================
    ____ __ _ ____ __  __
   |    |  ` |    |  /  /
   | |  | |  | |__|    /
   | ___| |  | |  |    \
   |_|  |__,_|____|__\__\ DSO library

   Copyright (C) 2020 Eaton
   Copyright (C) 2020-2022 zJes

   This program is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as
   published by the Free Software Foundation; either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
========================================================================================================================================= */
#pragma once

#include <google/protobuf/descriptor.h>

namespace google::protobuf::compiler::pack {

inline std::string genFileName(const FileDescriptor* file)
{
    std::string name  = file->name();
    size_t      index = name.find_last_of('.');
    if (index != std::string::npos) {
        return name.substr(0, index) + ".h";
    }
    return name;
}

} // namespace google::protobuf::compiler::pack
