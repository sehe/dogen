/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * Copyright (C) 2012 Kitanda <info@kitanda.co.uk>
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
#include "dogen/stitch/types/scriptlet_line.hpp"

namespace dogen {
namespace stitch {

scriptlet_line::scriptlet_line(const std::string& content)
    : content_(content) { }

void scriptlet_line::swap(scriptlet_line& other) noexcept {
    using std::swap;
    swap(content_, other.content_);
}

bool scriptlet_line::operator==(const scriptlet_line& rhs) const {
    return content_ == rhs.content_;
}

scriptlet_line& scriptlet_line::operator=(scriptlet_line other) {
    using std::swap;
    swap(*this, other);
    return *this;
}

const std::string& scriptlet_line::content() const {
    return content_;
}

std::string& scriptlet_line::content() {
    return content_;
}

void scriptlet_line::content(const std::string& v) {
    content_ = v;
}

void scriptlet_line::content(const std::string&& v) {
    content_ = std::move(v);
}

} }