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
#ifndef DOGEN_UPSILON_TYPES_ENUMERATION_HPP
#define DOGEN_UPSILON_TYPES_ENUMERATION_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

#include <iosfwd>
#include <string>
#include <vector>
#include <algorithm>
#include "dogen/upsilon/types/type.hpp"
#include "dogen/upsilon/serialization/enumeration_fwd_ser.hpp"

namespace dogen {
namespace upsilon {

class enumeration final : public dogen::upsilon::type {
public:
    enumeration() = default;
    enumeration(const enumeration&) = default;
    enumeration(enumeration&&) = default;

    virtual ~enumeration() noexcept { }

public:
    enumeration(
        const dogen::upsilon::name& name,
        const dogen::upsilon::name& extends,
        const std::string& comment,
        const std::vector<std::string>& tag_refs,
        const std::string& pof_id,
        const std::vector<std::string>& values,
        const std::string& default_value);

private:
    template<typename Archive>
    friend void boost::serialization::save(Archive& ar, const dogen::upsilon::enumeration& v, unsigned int version);

    template<typename Archive>
    friend void boost::serialization::load(Archive& ar, dogen::upsilon::enumeration& v, unsigned int version);

public:
    using type::accept;

    virtual void accept(const type_visitor& v) const override;
    virtual void accept(type_visitor& v) const override;
    virtual void accept(const type_visitor& v) override;
    virtual void accept(type_visitor& v) override;
public:
    void to_stream(std::ostream& s) const override;

public:
    const std::vector<std::string>& values() const;
    std::vector<std::string>& values();
    void values(const std::vector<std::string>& v);
    void values(const std::vector<std::string>&& v);

    const std::string& default_value() const;
    std::string& default_value();
    void default_value(const std::string& v);
    void default_value(const std::string&& v);

public:
    bool operator==(const enumeration& rhs) const;
    bool operator!=(const enumeration& rhs) const {
        return !this->operator==(rhs);
    }

public:
    bool equals(const dogen::upsilon::type& other) const override;

public:
    void swap(enumeration& other) noexcept;
    enumeration& operator=(enumeration other);

private:
    std::vector<std::string> values_;
    std::string default_value_;
};

} }

namespace std {

template<>
inline void swap(
    dogen::upsilon::enumeration& lhs,
    dogen::upsilon::enumeration& rhs) {
    lhs.swap(rhs);
}

}

#endif
