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
#include "dogen/quilt.cpp/types/formatters/hash/primitive_implementation_formatter.hpp"
#include "dogen/quilt.cpp/types/formatters/hash/traits.hpp"
#include "dogen/quilt.cpp/types/formatters/types/traits.hpp"
#include "dogen/quilt.cpp/types/formatters/formatting_error.hpp"
#include "dogen/quilt.cpp/types/formatters/inclusion_constants.hpp"
#include "dogen/quilt.cpp/types/formatters/traits.hpp"
#include "dogen/quilt.cpp/types/traits.hpp"
#include "dogen/quilt.cpp/types/formatters/assistant.hpp"
#include "dogen/formatters/types/sequence_formatter.hpp"
#include "dogen/yarn/types/primitive.hpp"
#include "dogen/utility/log/logger.hpp"
#include <boost/throw_exception.hpp>
#include <boost/make_shared.hpp>
#include <typeinfo>

namespace dogen {
namespace quilt {
namespace cpp {
namespace formatters {
namespace hash {

std::string primitive_implementation_formatter::static_artefact() {
    return traits::primitive_implementation_archetype();
}

std::string primitive_implementation_formatter::formatter_name() const {
    static auto r(archetype_location().archetype());
    return r;
}

annotations::archetype_location
primitive_implementation_formatter::archetype_location() const {
    static annotations::archetype_location
        r(cpp::traits::family(),  cpp::traits::kernel(),
          traits::facet(),
          primitive_implementation_formatter::static_artefact());
    return r;
}

std::type_index primitive_implementation_formatter::element_type_index() const {
    static auto r(std::type_index(typeid(yarn::primitive)));
    return r;
}

inclusion_support_types primitive_implementation_formatter::inclusion_support_type() const {
    return inclusion_support_types::not_supported;
}

boost::filesystem::path primitive_implementation_formatter::inclusion_path(
    const formattables::locator& /*l*/, const yarn::name& n) const {

    using namespace dogen::utility::log;
    static logger lg(
        logger_factory(primitive_implementation_formatter::static_artefact()));
    static const std::string not_supported("Inclusion path is not supported: ");

    BOOST_LOG_SEV(lg, error) << not_supported << n.id();
    BOOST_THROW_EXCEPTION(formatting_error(not_supported + n.id()));
}

boost::filesystem::path primitive_implementation_formatter::full_path(
    const formattables::locator& l, const yarn::name& n) const {
    return l.make_full_path_for_cpp_implementation(n, static_artefact());
}

std::list<std::string> primitive_implementation_formatter::inclusion_dependencies(
    const formattables::inclusion_dependencies_builder_factory& f,
    const yarn::element& e) const {

    const auto& p(assistant::as<yarn::primitive>(static_artefact(), e));
    const auto carch(traits::canonical_archetype());
    auto builder(f.make());
    builder.add(p.name(), carch);

    return builder.build();
}

dogen::formatters::artefact primitive_implementation_formatter::
format(const context& ctx, const yarn::element& e) const {
    const auto id(e.name().id());
    assistant a(ctx, archetype_location(), false/*requires_header_guard*/, id);
    const auto& p(a.as<yarn::primitive>(static_artefact(), e));

    const auto sn(p.name().simple());
    const auto qn(a.get_qualified_name(p.name()));
    {
        auto sbf(a.make_scoped_boilerplate_formatter());
a.stream() << std::endl;
a.stream() << "namespace {" << std::endl;
a.stream() << std::endl;
a.stream() << "template <typename HashableType>" << std::endl;
a.stream() << "inline void combine(std::size_t& seed, const HashableType& value) {" << std::endl;
a.stream() << "    std::hash<HashableType> hasher;" << std::endl;
a.stream() << "    seed ^= hasher(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);" << std::endl;
a.stream() << "}" << std::endl;

        a.add_helper_methods(p.name().id());
a.stream() << std::endl;
a.stream() << "}" << std::endl;
a.stream() << std::endl;
        {
            const auto ns(a.make_namespaces(p.name()));
            auto snf(a.make_scoped_namespace_formatter(ns));
            const auto sn(p.name().simple());
            const auto qn(a.get_qualified_name(p.name()));
            const auto attr(p.value_attribute());
a.stream() << std::endl;
a.stream() << "std::size_t " << sn << "_hasher::hash(const " << sn << "& v) {" << std::endl;
a.stream() << "    std::size_t seed(0);" << std::endl;
            if (a.requires_hashing_helper_method(attr))
a.stream() << "    combine(seed, hash_" << attr.parsed_type().identifiable() << "(v." << attr.name().simple() << "()));" << std::endl;
            else
a.stream() << "    combine(seed, v." << attr.name().simple() << "());" << std::endl;
a.stream() << "    return seed;" << std::endl;
a.stream() << "}" << std::endl;
a.stream() << std::endl;
        } // snf
    } // sbf
    return a.make_artefact();
}
} } } } }
