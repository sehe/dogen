/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * Copyright (C) 2012-2015 Marco Craveiro <marco.craveiro@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 */
#include <ostream>
#include "dogen/upsilon/io/config_io.hpp"
#include "dogen/upsilon/io/output_io.hpp"
#include "dogen/upsilon/io/directory_io.hpp"
#include "dogen/upsilon/io/schema_ref_io.hpp"

namespace std {

inline std::ostream& operator<<(std::ostream& s, const std::vector<dogen::upsilon::schema_ref>& v) {
    s << "[ ";
    for (auto i(v.begin()); i != v.end(); ++i) {
        if (i != v.begin()) s << ", ";
        s << *i;
    }
    s << "] ";
    return s;
}

}

namespace std {

inline std::ostream& operator<<(std::ostream& s, const std::vector<dogen::upsilon::output>& v) {
    s << "[ ";
    for (auto i(v.begin()); i != v.end(); ++i) {
        if (i != v.begin()) s << ", ";
        s << *i;
    }
    s << "] ";
    return s;
}

}

namespace dogen {
namespace upsilon {

std::ostream& operator<<(std::ostream& s, const config& v) {
    s << " { "
      << "\"__type__\": " << "\"dogen::upsilon::config\"" << ", "
      << "\"directory\": " << v.directory() << ", "
      << "\"schema_refs\": " << v.schema_refs() << ", "
      << "\"outputs\": " << v.outputs() << ", "
      << "\"file_name\": " << "\"" << v.file_name().generic_string() << "\""
      << " }";
    return(s);
}

} }
