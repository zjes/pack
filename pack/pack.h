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

#include "pack/formatter.h"     // IWYU pragma: keep
#include "pack/serialization.h" // IWYU pragma: keep
#include "pack/types/binary.h"  // IWYU pragma: keep
#include "pack/types/enum.h"    // IWYU pragma: keep
#include "pack/types/list.h"
#include "pack/types/map.h"
#include "pack/types/node.h" // IWYU pragma: keep
#include "pack/types/string.h"
#include "pack/types/value.h"
#include "pack/types/variant.h" // IWYU pragma: keep

namespace pack {

using Int32  = NumericValue<Type::Int32>;
using Int64  = NumericValue<Type::Int64>;
using UInt32 = NumericValue<Type::UInt32>;
using UInt64 = NumericValue<Type::UInt64>;
using Float  = NumericValue<Type::Float>;
using Double = NumericValue<Type::Double>;
using Bool   = Value<Type::Bool>;
using UChar  = Value<Type::UChar>;

// =========================================================================================================================================

using Int32List  = List<Int32>;
using Int64List  = List<Int64>;
using UInt32List = List<UInt32>;
using UInt64List = List<UInt64>;
using FloatList  = List<Float>;
using DoubleList = List<Double>;
using BoolList   = List<Bool>;
using StringList = List<String>;
using BinaryList = List<Binary>;

// =========================================================================================================================================

using Int32Map  = Map<Int32>;
using Int64Map  = Map<Int64>;
using UInt32Map = Map<UInt32>;
using UInt64Map = Map<UInt64>;
using FloatMap  = Map<Float>;
using DoubleMap = Map<Double>;
using BoolMap   = Map<Bool>;
using StringMap = Map<String>;
using BinaryMap = Map<Binary>;

} // namespace pack

// =========================================================================================================================================
