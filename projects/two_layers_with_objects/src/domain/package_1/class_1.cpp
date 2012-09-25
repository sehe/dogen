/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * Copyright (C) 2012 Kitanda
 *
 * This file is distributed under the Kitanda Proprietary Software
 * Licence. See doc/LICENCE.TXT for details.
 *
 */
#include <ostream>
#include "dogen/two_layers_with_objects/domain/package_1/class_1.hpp"
#include "dogen/two_layers_with_objects/io/versioned_key_io.hpp"
#include "dogen/utility/io/jsonify_io.hpp"

namespace dogen {
namespace two_layers_with_objects {
namespace package_1 {

class_1::class_1(dogen::two_layers_with_objects::versioned_key versioned_key)
    : versioned_key_(versioned_key) { }

void class_1::to_stream(std::ostream& stream) const {
    using dogen::utility::streaming::jsonify;

    stream << " {"
           << "\"__type__\": " << "\"class_1\"" << ","
           << "\"versioned_key\": " << jsonify(versioned_key_)
           << " }";
}

void class_1::swap(class_1& other) {
    std::swap(versioned_key_, other.versioned_key_);
}

class_1& class_1::operator=(class_1 other) {
    std::swap(*this, other);
    return *this;
}

} } }