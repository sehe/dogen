/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * Copyright (C) 2012 Kitanda
 *
 * This file is distributed under the Kitanda Proprietary Software
 * Licence. See doc/LICENCE.TXT for details.
 *
 */
#ifndef DOGEN_ENABLE_FACET_HASH_HASH_UNVERSIONED_KEY_HASH_HPP
#define DOGEN_ENABLE_FACET_HASH_HASH_UNVERSIONED_KEY_HASH_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

#include <functional>
#include "dogen/enable_facet_hash/domain/unversioned_key.hpp"
#include "dogen/utility/hash/combine.hpp"

namespace std {

template<>
class hash<dogen::enable_facet_hash::unversioned_key> {
public:
    size_t operator()(dogen::enable_facet_hash::unversioned_key value) const {
        using dogen::utility::hash::combine;
        std::size_t seed(0);

        combine(seed, value.id());

        return seed;
    }
};

}
#endif
