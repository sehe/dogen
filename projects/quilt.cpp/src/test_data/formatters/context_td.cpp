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
#include <sstream>
#include "dogen/quilt.cpp/test_data/settings/bundle_td.hpp"
#include "dogen/quilt.cpp/test_data/formatters/context_td.hpp"
#include "dogen/quilt.cpp/test_data/formattables/formatter_properties_td.hpp"

namespace {

dogen::quilt::cpp::settings::bundle
create_dogen_quilt_cpp_settings_bundle(const unsigned int position) {
    return dogen::quilt::cpp::settings::bundle_generator::create(position);
}

std::string create_std_string(const unsigned int position) {
    std::ostringstream s;
    s << "a_string_" << position;
    return s.str();
}

dogen::quilt::cpp::formattables::formatter_properties
create_dogen_quilt_cpp_formattables_formatter_properties(const unsigned int position) {
    return dogen::quilt::cpp::formattables::formatter_properties_generator::create(position);
}

std::unordered_map<std::string, dogen::quilt::cpp::formattables::formatter_properties> create_std_unordered_map_std_string_dogen_quilt_cpp_formattables_formatter_properties(unsigned int position) {
    std::unordered_map<std::string, dogen::quilt::cpp::formattables::formatter_properties> r;
    for (unsigned int i(0); i < 4; ++i) {
        r.insert(std::make_pair(create_std_string(position + i), create_dogen_quilt_cpp_formattables_formatter_properties(position + i)));
    }
    return r;
}

}

namespace dogen {
namespace quilt {
namespace cpp {
namespace formatters {

context_generator::context_generator() : position_(0) { }

void context_generator::
populate(const unsigned int position, result_type& v) {
    v.bundle(create_dogen_quilt_cpp_settings_bundle(position + 0));
    v.formatter_properties(create_std_unordered_map_std_string_dogen_quilt_cpp_formattables_formatter_properties(position + 1));
}

context_generator::result_type
context_generator::create(const unsigned int position) {
    context r;
    context_generator::populate(position, r);
    return r;
}

context_generator::result_type*
context_generator::create_ptr(const unsigned int position) {
    context* p = new context();
    context_generator::populate(position, *p);
    return p;
}

context_generator::result_type
context_generator::operator()() {
    return create(position_++);
}

} } } }