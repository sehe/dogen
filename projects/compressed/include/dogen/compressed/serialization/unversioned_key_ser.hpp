/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * Copyright (C) 2012 Kitanda
 *
 * This file is distributed under the Kitanda Proprietary Software
 * Licence. See doc/LICENCE.TXT for details.
 *
 */
#ifndef DOGEN_COMPRESSED_SERIALIZATION_UNVERSIONED_KEY_SER_HPP
#define DOGEN_COMPRESSED_SERIALIZATION_UNVERSIONED_KEY_SER_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

#include <boost/serialization/nvp.hpp>
#include "dogen/compressed/domain/unversioned_key.hpp"

namespace dogen {
namespace compressed {

class unversioned_key_serializer {
public:
    template<typename Archive>
    void serialize(Archive & archive,
        dogen::compressed::unversioned_key& value,
        const unsigned int /*version*/) {
        using boost::serialization::make_nvp;

        archive & make_nvp("id", value.id_);
    }
};

} }

namespace boost {
namespace serialization {

template<class Archive>
inline void serialize(Archive & archive,
    dogen::compressed::unversioned_key& value,
    const unsigned int version) {
    dogen::compressed::unversioned_key_serializer serializer;
    serializer.serialize<Archive>(archive, value, version);
}

} }

#endif
