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
#ifndef DOGEN_ANNOTATIONS_TYPES_PROFILE_REPOSITORY_HPP
#define DOGEN_ANNOTATIONS_TYPES_PROFILE_REPOSITORY_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

#include <string>
#include <algorithm>
#include <unordered_map>
#include "dogen/annotations/types/profile.hpp"
#include "dogen/annotations/serialization/profile_repository_fwd_ser.hpp"

namespace dogen {
namespace annotations {

class profile_repository final {
public:
    profile_repository() = default;
    profile_repository(const profile_repository&) = default;
    profile_repository(profile_repository&&) = default;
    ~profile_repository() = default;

public:
    explicit profile_repository(const std::unordered_map<std::string, dogen::annotations::profile>& profiles_by_name);

private:
    template<typename Archive>
    friend void boost::serialization::save(Archive& ar, const dogen::annotations::profile_repository& v, unsigned int version);

    template<typename Archive>
    friend void boost::serialization::load(Archive& ar, dogen::annotations::profile_repository& v, unsigned int version);

public:
    const std::unordered_map<std::string, dogen::annotations::profile>& profiles_by_name() const;
    std::unordered_map<std::string, dogen::annotations::profile>& profiles_by_name();
    void profiles_by_name(const std::unordered_map<std::string, dogen::annotations::profile>& v);
    void profiles_by_name(const std::unordered_map<std::string, dogen::annotations::profile>&& v);

public:
    bool operator==(const profile_repository& rhs) const;
    bool operator!=(const profile_repository& rhs) const {
        return !this->operator==(rhs);
    }

public:
    void swap(profile_repository& other) noexcept;
    profile_repository& operator=(profile_repository other);

private:
    std::unordered_map<std::string, dogen::annotations::profile> profiles_by_name_;
};

} }

namespace std {

template<>
inline void swap(
    dogen::annotations::profile_repository& lhs,
    dogen::annotations::profile_repository& rhs) {
    lhs.swap(rhs);
}

}

#endif
