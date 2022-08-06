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
#include "utils.h"
#include "pack/formatter.h" // IWYU pragma: keep
#ifdef WITH_QTSTRING
#include <QFile>
#endif
#include <fstream>


namespace pack {

Expected<string_t> read(const string_t& filename)
{
#ifdef WITH_QTSTRING
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly)) {
        return QString::fromUtf8(file.readAll());
    }
#else
    std::ifstream st(filename);
    if (st.is_open()) {
        return Expected<std::string>({std::istreambuf_iterator<char>(st), std::istreambuf_iterator<char>()});
    }
#endif
    return unexpected("Cannot read file {}"_s, filename);
}

Expected<void> write(const string_t& filename, const string_t& content)
{
#ifdef WITH_QTSTRING
    std::ofstream st(filename.toStdString());
#else
    std::ofstream st(filename);
#endif
    if (st.is_open()) {
        st << content;
        st.close();
        return {};
    }
    return unexpected("Cannot read file {}"_s, filename);
}

} // namespace pack
