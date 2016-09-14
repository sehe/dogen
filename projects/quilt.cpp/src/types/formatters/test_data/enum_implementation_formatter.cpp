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
#include <boost/make_shared.hpp>
#include "dogen/yarn/types/enumeration.hpp"
#include "dogen/quilt.cpp/types/formatters/traits.hpp"
#include "dogen/quilt.cpp/types/formatters/types/traits.hpp"
#include "dogen/quilt.cpp/types/formatters/test_data/traits.hpp"
#include "dogen/quilt.cpp/types/formatters/inclusion_constants.hpp"
#include "dogen/quilt.cpp/types/formatters/assistant.hpp"
#include "dogen/quilt.cpp/types/formatters/test_data/enum_implementation_formatter_stitch.hpp"
#include "dogen/quilt.cpp/types/formatters/test_data/enum_implementation_formatter.hpp"

namespace dogen {
namespace quilt {
namespace cpp {
namespace formatters {
namespace test_data {

namespace {

class provider final :
        public properties::provider_interface<yarn::enumeration> {
public:
    std::string formatter_name() const override;

    std::list<std::string> provide_inclusion_dependencies(
        const properties::inclusion_dependencies_builder_factory& f,
        const yarn::enumeration& e) const override;

    properties::path_derivatives provide_path_derivatives(
        const properties::path_derivatives_factory& f,
        const yarn::name& n) const override;
};

std::string provider::formatter_name() const {
    return enum_implementation_formatter::static_formatter_name();
}

std::list<std::string>
provider::provide_inclusion_dependencies(
    const properties::inclusion_dependencies_builder_factory& f,
    const yarn::enumeration& o) const {
  auto builder(f.make());

  const auto ch_fn(traits::class_header_formatter_name());
  builder.add(o.name(), ch_fn);
  return builder.build();
}

properties::path_derivatives provider::provide_path_derivatives(
    const properties::path_derivatives_factory& f,
    const yarn::name& n) const {
    const auto r(f.make_cpp_implementation(n, formatter_name()));
    return r;
}

}

std::string enum_implementation_formatter::static_formatter_name() {
    return traits::enum_implementation_formatter_name();
}

std::string enum_implementation_formatter::id() const {
    static auto r(ownership_hierarchy().formatter_name());
    return r;
}

dynamic::ownership_hierarchy enum_implementation_formatter::
ownership_hierarchy() const {
    static dynamic::ownership_hierarchy
        r(formatters::traits::model_name(), traits::facet_name(),
            enum_implementation_formatter::static_formatter_name(),
            formatters::traits::implementation_formatter_group_name());
    return r;
}

file_types enum_implementation_formatter::file_type() const {
    return file_types::cpp_implementation;
}

properties::origin_types
enum_implementation_formatter::formattable_origin_type() const {
    return properties::origin_types::external;
}

void enum_implementation_formatter::register_inclusion_dependencies_provider(
    properties::registrar& rg) const {
    rg.register_provider(boost::make_shared<provider>());
}

dogen::formatters::file enum_implementation_formatter::
format(const context& ctx, const yarn::enumeration& e) const {
    assistant a(ctx, ownership_hierarchy(), file_type(), e.name().id());
    const auto r(enum_implementation_formatter_stitch(a, e));
    return r;
}

} } } } }
