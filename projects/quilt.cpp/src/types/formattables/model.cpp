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
#include "dogen/quilt.cpp/types/formattables/model.hpp"

namespace dogen {
namespace quilt {
namespace cpp {
namespace formattables {

model::model(
    const std::unordered_map<std::string, dogen::quilt::cpp::annotations::streaming_annotations>& streaming_annotations,
    const std::unordered_map<std::string, std::string>& facet_directory_for_facet,
    const std::unordered_map<std::string, dogen::quilt::cpp::formattables::formattable>& formattables)
    : streaming_annotations_(streaming_annotations),
      facet_directory_for_facet_(facet_directory_for_facet),
      formattables_(formattables) { }

void model::swap(model& other) noexcept {
    using std::swap;
    swap(streaming_annotations_, other.streaming_annotations_);
    swap(facet_directory_for_facet_, other.facet_directory_for_facet_);
    swap(formattables_, other.formattables_);
}

bool model::operator==(const model& rhs) const {
    return streaming_annotations_ == rhs.streaming_annotations_ &&
        facet_directory_for_facet_ == rhs.facet_directory_for_facet_ &&
        formattables_ == rhs.formattables_;
}

model& model::operator=(model other) {
    using std::swap;
    swap(*this, other);
    return *this;
}

const std::unordered_map<std::string, dogen::quilt::cpp::annotations::streaming_annotations>& model::streaming_annotations() const {
    return streaming_annotations_;
}

std::unordered_map<std::string, dogen::quilt::cpp::annotations::streaming_annotations>& model::streaming_annotations() {
    return streaming_annotations_;
}

void model::streaming_annotations(const std::unordered_map<std::string, dogen::quilt::cpp::annotations::streaming_annotations>& v) {
    streaming_annotations_ = v;
}

void model::streaming_annotations(const std::unordered_map<std::string, dogen::quilt::cpp::annotations::streaming_annotations>&& v) {
    streaming_annotations_ = std::move(v);
}

const std::unordered_map<std::string, std::string>& model::facet_directory_for_facet() const {
    return facet_directory_for_facet_;
}

std::unordered_map<std::string, std::string>& model::facet_directory_for_facet() {
    return facet_directory_for_facet_;
}

void model::facet_directory_for_facet(const std::unordered_map<std::string, std::string>& v) {
    facet_directory_for_facet_ = v;
}

void model::facet_directory_for_facet(const std::unordered_map<std::string, std::string>&& v) {
    facet_directory_for_facet_ = std::move(v);
}

const std::unordered_map<std::string, dogen::quilt::cpp::formattables::formattable>& model::formattables() const {
    return formattables_;
}

std::unordered_map<std::string, dogen::quilt::cpp::formattables::formattable>& model::formattables() {
    return formattables_;
}

void model::formattables(const std::unordered_map<std::string, dogen::quilt::cpp::formattables::formattable>& v) {
    formattables_ = v;
}

void model::formattables(const std::unordered_map<std::string, dogen::quilt::cpp::formattables::formattable>&& v) {
    formattables_ = std::move(v);
}

} } } }
