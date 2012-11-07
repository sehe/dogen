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
#ifndef DOGEN_STD_MODEL_DOMAIN_CLASS_G_HPP
#define DOGEN_STD_MODEL_DOMAIN_CLASS_G_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

#include <algorithm>
#include <list>
#include <unordered_map>
#include <vector>
#include "dogen/std_model/domain/class_a.hpp"
#include "dogen/std_model/domain/class_a_unversioned_key.hpp"
#include "dogen/std_model/domain/class_a_versioned_key.hpp"
#include "dogen/std_model/domain/pkg1/class_c.hpp"
#include "dogen/std_model/hash/class_a_hash.hpp"
#include "dogen/std_model/hash/class_a_unversioned_key_hash.hpp"
#include "dogen/std_model/hash/class_a_versioned_key_hash.hpp"
#include "dogen/std_model/hash/pkg1/class_c_hash.hpp"
#include "dogen/std_model/serialization/class_g_fwd_ser.hpp"

namespace dogen {
namespace std_model {

class class_g final {
public:
    class_g() = default;
    class_g(const class_g&) = default;
    class_g(class_g&&) = default;
    ~class_g() = default;

public:
    class_g(
        const std::unordered_map<dogen::std_model::class_a, std::vector<dogen::std_model::pkg1::class_c> >& prop_0,
        const std::unordered_map<dogen::std_model::pkg1::class_c, std::list<dogen::std_model::class_a> >& prop_1,
        const std::unordered_map<dogen::std_model::class_a_unversioned_key, dogen::std_model::class_a>& prop_2,
        const std::unordered_map<dogen::std_model::class_a_versioned_key, dogen::std_model::class_a>& prop_3);

private:
    template<typename Archive>
    friend void boost::serialization::save(Archive& ar, const class_g& v, unsigned int version);

    template<typename Archive>
    friend void boost::serialization::load(Archive& ar, class_g& v, unsigned int version);

public:
    std::unordered_map<dogen::std_model::class_a, std::vector<dogen::std_model::pkg1::class_c> > prop_0() const {
        return prop_0_;
    }

    void prop_0(const std::unordered_map<dogen::std_model::class_a, std::vector<dogen::std_model::pkg1::class_c> >& v) {
        prop_0_ = v;
    }

    std::unordered_map<dogen::std_model::pkg1::class_c, std::list<dogen::std_model::class_a> > prop_1() const {
        return prop_1_;
    }

    void prop_1(const std::unordered_map<dogen::std_model::pkg1::class_c, std::list<dogen::std_model::class_a> >& v) {
        prop_1_ = v;
    }

    std::unordered_map<dogen::std_model::class_a_unversioned_key, dogen::std_model::class_a> prop_2() const {
        return prop_2_;
    }

    void prop_2(const std::unordered_map<dogen::std_model::class_a_unversioned_key, dogen::std_model::class_a>& v) {
        prop_2_ = v;
    }

    std::unordered_map<dogen::std_model::class_a_versioned_key, dogen::std_model::class_a> prop_3() const {
        return prop_3_;
    }

    void prop_3(const std::unordered_map<dogen::std_model::class_a_versioned_key, dogen::std_model::class_a>& v) {
        prop_3_ = v;
    }

public:
    bool operator==(const class_g& rhs) const;
    bool operator!=(const class_g& rhs) const {
        return !this->operator==(rhs);
    }

public:
    void swap(class_g& other) noexcept;
    class_g& operator=(class_g other);

private:
    std::unordered_map<dogen::std_model::class_a, std::vector<dogen::std_model::pkg1::class_c> > prop_0_;
    std::unordered_map<dogen::std_model::pkg1::class_c, std::list<dogen::std_model::class_a> > prop_1_;
    std::unordered_map<dogen::std_model::class_a_unversioned_key, dogen::std_model::class_a> prop_2_;
    std::unordered_map<dogen::std_model::class_a_versioned_key, dogen::std_model::class_a> prop_3_;
};

} }

namespace std {

template<>
inline void swap(
    dogen::std_model::class_g& lhs,
    dogen::std_model::class_g& rhs) {
    lhs.swap(rhs);
}

}

#endif
