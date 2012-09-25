/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * Copyright (C) 2012 Kitanda
 *
 * This file is distributed under the Kitanda Proprietary Software
 * Licence. See doc/LICENCE.TXT for details.
 *
 */
#ifndef DOGEN_CLASSES_WITHOUT_PACKAGE_DOMAIN_CLASS_5_HPP
#define DOGEN_CLASSES_WITHOUT_PACKAGE_DOMAIN_CLASS_5_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

#include <algorithm>
#include <iosfwd>
#include "dogen/classes_without_package/domain/versioned_key.hpp"

namespace dogen {
namespace classes_without_package {

class class_5_serializer;

class class_5 {
public:
    class_5() = default;
    class_5(const class_5&) = default;
    ~class_5() = default;
    class_5(class_5&&) = default;

public:
    explicit class_5(dogen::classes_without_package::versioned_key versioned_key);

public:
    friend class dogen::classes_without_package::class_5_serializer;

public:
    void to_stream(std::ostream& stream) const;

public:
    dogen::classes_without_package::versioned_key versioned_key() const {
        return versioned_key_;
    }

    void versioned_key(dogen::classes_without_package::versioned_key value) {
        versioned_key_ = value;
    }

public:
    bool operator==(const class_5& rhs) const {
        return versioned_key_ == rhs.versioned_key_;
    }

    bool operator!=(const class_5 rhs) const {
        return !this->operator==(rhs);
    }

public:
    void swap(class_5& other);
    class_5& operator=(class_5 other);

private:
    dogen::classes_without_package::versioned_key versioned_key_;
};

} }

namespace std {

template<>
inline void swap(
    dogen::classes_without_package::class_5& lhs,
    dogen::classes_without_package::class_5& rhs) {
    lhs.swap(rhs);
}

}

#endif
