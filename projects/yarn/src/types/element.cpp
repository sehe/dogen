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
#include <boost/io/ios_state.hpp>
#include <boost/algorithm/string.hpp>
#include "dogen/yarn/io/name_io.hpp"
#include "dogen/yarn/types/element.hpp"
#include "dogen/yarn/io/origin_types_io.hpp"
#include "dogen/annotations/io/annotation_io.hpp"

inline std::string tidy_up_string(std::string s) {
    boost::replace_all(s, "\r\n", "<new_line>");
    boost::replace_all(s, "\n", "<new_line>");
    boost::replace_all(s, "\"", "<quote>");
    boost::replace_all(s, "\\", "<backslash>");
    return s;
}

namespace boost {

inline std::ostream& operator<<(std::ostream& s, const boost::optional<dogen::yarn::name>& v) {
    s << "{ " << "\"__type__\": " << "\"boost::optional\"" << ", ";

    if (v)
        s << "\"data\": " << *v;
    else
        s << "\"data\": ""\"<null>\"";
    s << " }";
    return s;
}

}

namespace std {

inline std::ostream& operator<<(std::ostream& s, const std::vector<std::string>& v) {
    s << "[ ";
    for (auto i(v.begin()); i != v.end(); ++i) {
        if (i != v.begin()) s << ", ";
        s << "\"" << tidy_up_string(*i) << "\"";
    }
    s << "] ";
    return s;
}

}

namespace dogen {
namespace yarn {

element::element()
    : origin_type_(static_cast<dogen::yarn::origin_types>(0)),
      in_global_module_(static_cast<bool>(0)),
      is_element_extension_(static_cast<bool>(0)) { }

element::element(element&& rhs)
    : documentation_(std::move(rhs.documentation_)),
      annotation_(std::move(rhs.annotation_)),
      name_(std::move(rhs.name_)),
      origin_type_(std::move(rhs.origin_type_)),
      contained_by_(std::move(rhs.contained_by_)),
      in_global_module_(std::move(rhs.in_global_module_)),
      stereotypes_(std::move(rhs.stereotypes_)),
      is_element_extension_(std::move(rhs.is_element_extension_)) { }

element::element(
    const std::string& documentation,
    const dogen::annotations::annotation& annotation,
    const dogen::yarn::name& name,
    const dogen::yarn::origin_types origin_type,
    const boost::optional<dogen::yarn::name>& contained_by,
    const bool in_global_module,
    const std::vector<std::string>& stereotypes,
    const bool is_element_extension)
    : documentation_(documentation),
      annotation_(annotation),
      name_(name),
      origin_type_(origin_type),
      contained_by_(contained_by),
      in_global_module_(in_global_module),
      stereotypes_(stereotypes),
      is_element_extension_(is_element_extension) { }

void element::to_stream(std::ostream& s) const {
    boost::io::ios_flags_saver ifs(s);
    s.setf(std::ios_base::boolalpha);
    s.setf(std::ios::fixed, std::ios::floatfield);
    s.precision(6);
    s.setf(std::ios::showpoint);

    s << " { "
      << "\"__type__\": " << "\"dogen::yarn::element\"" << ", "
      << "\"documentation\": " << "\"" << tidy_up_string(documentation_) << "\"" << ", "
      << "\"annotation\": " << annotation_ << ", "
      << "\"name\": " << name_ << ", "
      << "\"origin_type\": " << origin_type_ << ", "
      << "\"contained_by\": " << contained_by_ << ", "
      << "\"in_global_module\": " << in_global_module_ << ", "
      << "\"stereotypes\": " << stereotypes_ << ", "
      << "\"is_element_extension\": " << is_element_extension_
      << " }";
}

void element::swap(element& other) noexcept {
    using std::swap;
    swap(documentation_, other.documentation_);
    swap(annotation_, other.annotation_);
    swap(name_, other.name_);
    swap(origin_type_, other.origin_type_);
    swap(contained_by_, other.contained_by_);
    swap(in_global_module_, other.in_global_module_);
    swap(stereotypes_, other.stereotypes_);
    swap(is_element_extension_, other.is_element_extension_);
}

bool element::compare(const element& rhs) const {
    return documentation_ == rhs.documentation_ &&
        annotation_ == rhs.annotation_ &&
        name_ == rhs.name_ &&
        origin_type_ == rhs.origin_type_ &&
        contained_by_ == rhs.contained_by_ &&
        in_global_module_ == rhs.in_global_module_ &&
        stereotypes_ == rhs.stereotypes_ &&
        is_element_extension_ == rhs.is_element_extension_;
}

const std::string& element::documentation() const {
    return documentation_;
}

std::string& element::documentation() {
    return documentation_;
}

void element::documentation(const std::string& v) {
    documentation_ = v;
}

void element::documentation(const std::string&& v) {
    documentation_ = std::move(v);
}

const dogen::annotations::annotation& element::annotation() const {
    return annotation_;
}

dogen::annotations::annotation& element::annotation() {
    return annotation_;
}

void element::annotation(const dogen::annotations::annotation& v) {
    annotation_ = v;
}

void element::annotation(const dogen::annotations::annotation&& v) {
    annotation_ = std::move(v);
}

const dogen::yarn::name& element::name() const {
    return name_;
}

dogen::yarn::name& element::name() {
    return name_;
}

void element::name(const dogen::yarn::name& v) {
    name_ = v;
}

void element::name(const dogen::yarn::name&& v) {
    name_ = std::move(v);
}

dogen::yarn::origin_types element::origin_type() const {
    return origin_type_;
}

void element::origin_type(const dogen::yarn::origin_types v) {
    origin_type_ = v;
}

const boost::optional<dogen::yarn::name>& element::contained_by() const {
    return contained_by_;
}

boost::optional<dogen::yarn::name>& element::contained_by() {
    return contained_by_;
}

void element::contained_by(const boost::optional<dogen::yarn::name>& v) {
    contained_by_ = v;
}

void element::contained_by(const boost::optional<dogen::yarn::name>&& v) {
    contained_by_ = std::move(v);
}

bool element::in_global_module() const {
    return in_global_module_;
}

void element::in_global_module(const bool v) {
    in_global_module_ = v;
}

const std::vector<std::string>& element::stereotypes() const {
    return stereotypes_;
}

std::vector<std::string>& element::stereotypes() {
    return stereotypes_;
}

void element::stereotypes(const std::vector<std::string>& v) {
    stereotypes_ = v;
}

void element::stereotypes(const std::vector<std::string>&& v) {
    stereotypes_ = std::move(v);
}

bool element::is_element_extension() const {
    return is_element_extension_;
}

void element::is_element_extension(const bool v) {
    is_element_extension_ = v;
}

} }
