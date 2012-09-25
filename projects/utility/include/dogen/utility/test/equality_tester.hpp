/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * Copyright (C) 2012 Kitanda
 *
 * This file is distributed under the Kitanda Proprietary Software
 * Licence. See doc/LICENCE.TXT for details.
 *
 */
#ifndef DOGEN_ORG_SPEC_EQUALITY_HPP
#define DOGEN_ORG_SPEC_EQUALITY_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

#include <boost/test/unit_test.hpp>
#include "dogen/utility/log/logger.hpp"
#include "dogen/utility/io/jsonify_io.hpp"

namespace dogen {
namespace utility {
namespace test {

/**
 * @brief Provides tests for different aspects of equality of domain
 * objects.
 */
template<typename Entity>
class equality_tester {
private:
    typedef Entity entity_type;

public:
    static void
    identical_objects_are_equal(const entity_type& a, const entity_type& b) {
        using dogen::utility::streaming::jsonify;
        using namespace dogen::utility::log;
        logger lg(logger_factory("equality_tester"));
        BOOST_LOG_SEV(lg, debug) << "a: " << jsonify(a);
        BOOST_LOG_SEV(lg, debug) << "b: " << jsonify(b);

        BOOST_CHECK(a == b);
        BOOST_CHECK(b == a);
        BOOST_CHECK(!(a != b));
    }

    static void an_object_is_equal_to_itself(const entity_type& a) {
        using dogen::utility::streaming::jsonify;
        using namespace dogen::utility::log;
        logger lg(logger_factory("equality_tester"));
        BOOST_LOG_SEV(lg, debug) << "a: " << jsonify(a);

        BOOST_CHECK(a == a);
        BOOST_CHECK(!(a != a));
    }

    /**
     * @brief Objects with distinct states are unequal.
     *
     * This cannot be proved for the general case, but we're trying to
     * catch silly errors here.
     */
    static void
    distinct_objects_are_unequal(const entity_type& a, const entity_type& b) {
        using dogen::utility::streaming::jsonify;
        using namespace dogen::utility::log;
        logger lg(logger_factory("equality_tester"));
        BOOST_LOG_SEV(lg, debug) << "a: " << jsonify(a);
        BOOST_LOG_SEV(lg, debug) << "b: " << jsonify(b);

        BOOST_CHECK(!(a == b));
        BOOST_CHECK(a != b);
    }
};

} } }

#endif
