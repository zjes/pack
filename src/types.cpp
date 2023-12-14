/* =========================================================================================================================================
    ____ __ _ ____ __  __
   |    |  ` |    |  /  /
   | |  | |  | |__|    /
   | ___| |  | |  |    \
   |_|  |__,_|____|__\__\ DSO library

Copyright (C) 2020-2022 zJes

This program is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as
published by the Free Software Foundation; either version 3 of the License, or (at your option) any later version.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
========================================================================================================================================= */

#include "pack/types.h"
#include "pack/ustring.h"

std::ostream& operator<<(std::ostream& ss, pack::Type value)
{
    ss << pack::valueTypeName(value);
    return ss;
}
