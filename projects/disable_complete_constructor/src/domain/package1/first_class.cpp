/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * Copyright (C) 2012 Kitanda
 *
 * This file is distributed under the Kitanda Proprietary Software
 * Licence. See doc/LICENCE.TXT for details.
 *
 */
#include <ostream>
#include "dogen/disable_complete_constructor/domain/package1/first_class.hpp"
#include "dogen/disable_complete_constructor/io/versioned_key_io.hpp"
#include "dogen/utility/io/jsonify_io.hpp"

namespace dogen {
namespace disable_complete_constructor {
namespace package1 {

first_class::first_class()
    : public_attribute_(static_cast<int>(0)),
      private_attribute_(static_cast<int>(0)) { }

void first_class::to_stream(std::ostream& stream) const {
    using dogen::utility::streaming::jsonify;

    stream << " {"
           << "\"__type__\": " << "\"first_class\"" << ","
           << "\"public_attribute\": " << jsonify(public_attribute_) << ","
           << "\"private_attribute\": " << jsonify(private_attribute_) << ","
           << "\"versioned_key\": " << jsonify(versioned_key_)
           << " }";
}

void first_class::swap(first_class& other) {
    std::swap(public_attribute_, other.public_attribute_);
    std::swap(private_attribute_, other.private_attribute_);
    std::swap(versioned_key_, other.versioned_key_);
}

first_class& first_class::operator=(first_class other) {
    std::swap(*this, other);
    return *this;
}

} } }