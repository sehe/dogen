/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * Copyright (C) 2012 Kitanda
 *
 * This file is distributed under the Kitanda Proprietary Software
 * Licence. See doc/LICENCE.TXT for details.
 *
 */
#ifndef DOGEN_DIA_SERIALIZATION_OBJECT_HPP
#define DOGEN_DIA_SERIALIZATION_OBJECT_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

#include <string>
#include <boost/serialization/string.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/optional.hpp>
#include "dogen/dia/domain/object.hpp"
#include "dogen/dia/serialization/attribute_ser.hpp"
#include "dogen/dia/serialization/child_node_ser.hpp"

namespace dogen {
namespace dia {
namespace serialization {
namespace detail {

class object_serializer {
public:
    template<typename Archive>
    void serialize(Archive & archive,
        dogen::dia::object& value,
        const unsigned int /*version*/) {
        using boost::serialization::make_nvp;
        archive & make_nvp("type", value.type_);
        archive & make_nvp("version", value.version_);
        archive & make_nvp("id", value.id_);
        archive & make_nvp("attributes", value.attributes_);
        archive & make_nvp("child_node", value.child_node_);
    }
};

} } } }

namespace boost {
namespace serialization {

template<class Archive>
inline void serialize(Archive & archive,
    dogen::dia::object& value,
    const unsigned int version) {
    dogen::dia::serialization::detail::object_serializer serializer;
    serializer.serialize<Archive>(archive, value, version);
}

} }

#endif
