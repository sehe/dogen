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
#include <istream>
#include <sstream>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/test/unit_test.hpp>
#include "dogen/utility/test/asserter.hpp"
#include "dogen/sml/types/meta_data_writer.hpp"
#include "dogen/utility/test/logging.hpp"
#include "dogen/utility/filesystem/path.hpp"
#include "dogen/utility/io/list_io.hpp"
#include "dogen/utility/test_data/dia_sml.hpp"
#include "dogen/sml/test/mock_model_factory.hpp"
#include "dogen/sml/types/object.hpp"
#include "dogen/sml/types/tags.hpp"
#include "dogen/sml/io/model_io.hpp"
#include "dogen/om/io/file_io.hpp"
#include "dogen/om/types/workflow.hpp"

namespace {

const std::string test_module("om");
const std::string test_suite("workflow_spec");
const std::string prj_dir("__project_directory__");
const std::string src_dir("__source_directory__");
const std::string inc_dir("__include_directory__");

using dogen::sml::test::mock_model_factory;
const mock_model_factory::flags flags(true/*tagged*/, false/*resolved*/,
    false/*merged*/, false/*concepts_indexed*/, false/*properties_indexed*/);
const mock_model_factory factory(flags);

const std::string type_with_no_properties(
    R"(/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * SAMPLE_MARKER
 *
 * Copyright (C) 2012 Person <name@company.co.uk>
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
#ifndef TYPES_SOME_TYPE_0
#define TYPES_SOME_TYPE_0

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

namespace some_model_0 {

/**
 * @brief Some documentation
 */
class some_type_0 final {
};

}

#endif
)");

const std::string model_with_comments(R"(/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * SAMPLE_MARKER
 *
 * Copyright (C) 2012 Person <name@company.co.uk>
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
#ifndef TYPES_SOME_MODEL_0
#define TYPES_SOME_MODEL_0

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

/**
 * @brief Some documentation
 */
namespace some_model_0 {
}

#endif
)");

}

using namespace dogen::om;
using namespace dogen::utility::test;
using dogen::utility::test::asserter;

BOOST_AUTO_TEST_SUITE(workflow)

BOOST_AUTO_TEST_CASE(empty_mock_model_results_in_expected_files) {
    SETUP_TEST_LOG_SOURCE("empty_mock_results_in_expected_files");

    using namespace dogen::utility::filesystem;
    const std::list<boost::filesystem::path> d = { data_files_directory() };
    dogen::om::workflow w(prj_dir, src_dir, inc_dir, d);

    auto m(factory.build_empty_model());
    BOOST_LOG_SEV(lg, debug) << "model: " << m;

    const auto r(w.execute(m));
    BOOST_LOG_SEV(lg, debug) << "result: " << r;

    BOOST_REQUIRE(r.size() == 1);
    const auto rp(r.front().relative_path().generic_string());
    BOOST_CHECK(asserter::assert_equals(rp,
            factory.types_header_filename(m.name())));

    const auto& c(r.front().contents());
    BOOST_CHECK(asserter::assert_equals_marker(model_with_comments, c));
}

BOOST_AUTO_TEST_CASE(single_type_model_results_in_expected_files) {
    SETUP_TEST_LOG_SOURCE("single_type_results_in_expected_files");

    using namespace dogen::utility::filesystem;
    const std::list<boost::filesystem::path> d = { data_files_directory() };
    dogen::om::workflow w(prj_dir, src_dir, inc_dir, d);
    auto m(factory.build_single_type_model());

    BOOST_REQUIRE(m.objects().size() == 1);
    auto& o(m.objects().begin()->second);
    BOOST_LOG_SEV(lg, debug) << "model: " << m;

    const auto r(w.execute(m));
    BOOST_LOG_SEV(lg, debug) << "result: " << r;
    BOOST_REQUIRE(r.size() == 2);

    bool found_model(false), found_type(false);
    for (const auto& f : r) {
        const auto rp(f.relative_path().generic_string());
        if (factory.types_header_filename(m.name()) == rp) {
            found_model = true;
            const auto& c(f.contents());
            BOOST_CHECK(asserter::assert_equals_marker(model_with_comments, c));
        } else if (factory.types_header_filename(o.name()) == rp) {
            found_type = true;
            const auto& c(f.contents());
            BOOST_CHECK(
                asserter::assert_equals_marker(type_with_no_properties, c));
        }
        else
            BOOST_FAIL("Unexpected file: " << f.relative_path());
    }
    BOOST_CHECK(found_model);
    BOOST_CHECK(found_type);
}

BOOST_AUTO_TEST_SUITE_END()
