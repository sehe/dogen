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
#include <set>
#include <boost/make_shared.hpp>
#include <boost/variant/get.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/throw_exception.hpp>
#include "dogen/utility/log/logger.hpp"
#include "dogen/yarn/types/name_factory.hpp"
#include "dogen/dia/types/composite.hpp"
#include "dogen/yarn.dia/types/transformation_error.hpp"
#include "dogen/yarn.dia/io/object_types_io.hpp"
#include "dogen/yarn.dia/types/processed_object.hpp"
#include "dogen/yarn.dia/io/processed_object_io.hpp"
#include "dogen/yarn.dia/io/repository_io.hpp"
#include "dogen/yarn.dia/types/validator.hpp"
#include "dogen/yarn.dia/types/repository_selector.hpp"
#include "dogen/yarn.dia/types/transformer.hpp"

namespace {

using namespace dogen::utility::log;
static logger lg(logger_factory("yarn.dia.transformer"));

const std::string empty;
const std::string duplicate_enumerator("Duplicate enumerator name: ");
const std::string empty_dia_object_name("Dia object name is empty");
const std::string type_attribute_expected(
    "Could not find type attribute. ID: ");
const std::string invalid_type_string(
    "String provided with type did not parse into Yarn names: ");
const std::string object_has_invalid_type("Invalid dia type: ");
const std::string invalid_stereotype_in_graph("Invalid stereotype: ");
const std::string immutabilty_with_inheritance(
    "Immutability not supported with inheritance: ");
const std::string multiple_inheritance(
    "Child has more than one parent, but multiple inheritance not supported:");

}

namespace dogen {
namespace yarn {
namespace dia {

transformer::transformer(const dynamic::workflow& w, const repository& rp)
    : repository_(rp), dynamic_workflow_(w) {

    BOOST_LOG_SEV(lg, debug) << "Initial repository: " << repository_;
}

yarn::generation_types transformer::generation_type(const profile& p) const {
    using yarn::generation_types;
    if (repository_.model().origin_type() != yarn::origin_types::target)
        return generation_types::no_generation;

    if (p.is_non_generatable() || p.is_service())
        return generation_types::partial_generation;

    return generation_types::full_generation;
}

void transformer::validate_dia_object_name(const std::string& n) const {
    if (n.empty()) {
        BOOST_LOG_SEV(lg, error) << empty_dia_object_name;
        BOOST_THROW_EXCEPTION(transformation_error(empty_dia_object_name));
    }
}

yarn::name transformer::to_name(const std::string& n) const {
    validate_dia_object_name(n);
    yarn::name_factory f;
    return f.build_element_in_model(repository_.model().name(), n);
}

yarn::name transformer::
to_name(const std::string& n, const yarn::name& module_n) const {
    validate_dia_object_name(n);
    yarn::name_factory f;
    return f.build_element_in_module(module_n, n);
}

yarn::attribute transformer::to_attribute(const yarn::name& owning_element,
    const processed_attribute& a) const {
    validate_dia_object_name(a.name());

    yarn::name_factory f;
    const auto n(f.build_attribute_name(owning_element, a.name()));

    yarn::attribute r;
    r.name(n);
    r.unparsed_type(a.type());
    r.documentation(a.comment().documentation());

    const auto& kvps(a.comment().key_value_pairs());
    const auto scope(dynamic::scope_types::property);
    r.extensions(dynamic_workflow_.execute(scope, kvps));

    return r;
}

yarn::enumerator transformer::to_enumerator(const processed_attribute& a,
    const unsigned int position) const {
    validate_dia_object_name(a.name());

    yarn::enumerator r;
    r.name(a.name());
    r.value(boost::lexical_cast<std::string>(position));
    r.documentation(a.comment().documentation());
    return r;
}

void transformer::update_element(const processed_object& o, const profile& p,
    yarn::element& e) {

    e.generation_type(generation_type(p));
    e.origin_type(origin_types::not_yet_determined);

    const auto package_id(o.child_node_id());
    bool is_in_package(!package_id.empty());
    if (is_in_package) {
        /*
         * Create the element name taking into account the
         * packages the element is contained in.
         */
        const_repository_selector crs(repository_);
        const auto& module(crs.module_for_id(package_id));
        e.name(to_name(o.name(), module.name()));
    } else {
        /*
         * Type is a top-level type - e.g. belongs to the
         * synthetic module for the model; do not add this
         * dependency to the name.
         */
        e.name(to_name(o.name()));
    }

    e.documentation(o.comment().documentation());

    const auto& kvps(o.comment().key_value_pairs());
    const auto scope(dynamic::scope_types::entity);
    e.extensions(dynamic_workflow_.execute(scope, kvps));
}

yarn::object
transformer::to_object(const processed_object& po, const profile& p,
    const yarn::object_types ot) {
    BOOST_LOG_SEV(lg, debug) << "Transforming dia object to object: "
                             << po.id();

    yarn::object r;
    update_element(po, p, r);

    r.is_fluent(p.is_fluent());
    r.is_immutable(p.is_immutable());
    r.object_type(ot);

    /*
     * Handle the stereotypes.
     * FIXME: hack to handle new stereotypes
     */
    if (p.is_visitable())
        r.stereotypes().insert(yarn::stereotypes::visitable);

    for (const auto us : p.unknown_stereotypes()) {
        if (us == "formatter")
            r.stereotypes().insert(yarn::stereotypes::formatter);
        else if (us == "handcrafted")
            r.stereotypes().insert(yarn::stereotypes::handcrafted);
        else {
            const auto n(to_name(us));
            r.modeled_concepts().push_back(n);
        }
    }

    for (const auto& p : po.attributes())
        r.local_attributes().push_back(to_attribute(r.name(), p));

    /*
     * If we have any parents, setup generalisation properties.
     */
    const_repository_selector crs(repository_);
    const auto parent_names(crs.parent_names_for_id(po.id()));
    if (!parent_names.empty()) {
        /*
         * Ensure we have at most one parent as we do not support
         * multiple inheritance for objects.
         */
        if (parent_names.size() > 1) {
            BOOST_LOG_SEV(lg, error) << multiple_inheritance << po.id();
            BOOST_THROW_EXCEPTION(
                transformation_error(multiple_inheritance + po.id()));
        }

        const auto parent_name(parent_names.front());
        r.parent(parent_name);
        BOOST_LOG_SEV(lg, debug) << "Set parent. Child: " << r.name().id()
                                 << " parent: " << parent_name.id();
    } else
        BOOST_LOG_SEV(lg, debug) << "Object has no parent: " << r.name().id();

    return r;
}

yarn::exception
transformer::to_exception(const processed_object& o, const profile& p) {
    BOOST_LOG_SEV(lg, debug) << "Transforming dia object to exception: "
                             << o.id();

    yarn::exception e;
    update_element(o, p, e);
    return e;
}

yarn::enumeration
transformer::to_enumeration(const processed_object& o, const profile& p) {
    BOOST_LOG_SEV(lg, debug) << "Transforming dia object to enumeration: "
                             << o.id();

    yarn::enumeration r;
    update_element(o, p, r);

    /*
     * Setup the invalid enumeration.
     */
    dogen::yarn::enumerator invalid;
    invalid.name("invalid");
    invalid.documentation("Represents an uninitialised enum");
    invalid.value("0");
    r.enumerators().push_back(invalid);

    std::set<std::string> enumerator_names;
    enumerator_names.insert(invalid.name());

    /*
     * Convert each attribute into an enumerator, ensuring the
     * enumerator names is unique within this enumeration. For each
     * enumerator we compute a position. Note that the zeroth position
     * is already taken by invalid, so we skip it.
     */
    unsigned int pos(0);
    for (const auto& attr : o.attributes()) {
        auto enumerator(to_enumerator(attr, ++pos));
        const auto i(enumerator_names.find(enumerator.name()));
        if (i != enumerator_names.end()) {
            BOOST_LOG_SEV(lg, error) << duplicate_enumerator
                                     << enumerator.name();
            BOOST_THROW_EXCEPTION(
                transformation_error(duplicate_enumerator + enumerator.name()));
        }
        enumerator_names.insert(enumerator.name());
        r.enumerators().push_back(enumerator);
    }
    return r;
}

yarn::module
transformer::to_module(const processed_object& o, const profile& p) {
    BOOST_LOG_SEV(lg, debug) << "Transforming dia object to module: " << o.id();

    yarn::module r;
    update_element(o, p, r);
    return r;
}

yarn::concept
transformer::to_concept(const processed_object& o, const profile& p) {
    BOOST_LOG_SEV(lg, debug) << "Transforming dia object to concept: "
                             << o.id();

    yarn::concept r;
    update_element(o, p, r);

    for (const auto& attr : o.attributes())
        r.local_attributes().push_back(to_attribute(r.name(), attr));

    const_repository_selector crs(repository_);
    const auto parent_names(crs.parent_names_for_id(o.id()));
    r.is_child(!parent_names.empty());

    if (parent_names.empty()) {
        BOOST_LOG_SEV(lg, debug) << "Object has no parent: " << r.name().id();
        return r;
    }

    for (const auto parent_name : parent_names)
        r.refines().push_back(parent_name);
    return r;
}

} } }
