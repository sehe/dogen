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
#include <istream>
#include <boost/make_shared.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/throw_exception.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "dogen/utility/log/logger.hpp"
#include "dogen/annotations/types/scribble_group.hpp"
#include "dogen/yarn/io/name_io.hpp"
#include "dogen/yarn/types/module.hpp"
#include "dogen/yarn/types/object.hpp"
#include "dogen/yarn/types/builtin.hpp"
#include "dogen/yarn/types/name_builder.hpp"
#include "dogen/yarn/types/name_factory.hpp"
#include "dogen/yarn.json/types/hydration_error.hpp"
#include "dogen/yarn.json/types/hydrator.hpp"

namespace {

using namespace dogen::utility::log;
auto lg(logger_factory("yarn.json.hydrator"));

const std::string empty;
const std::string is_default_enumeration_type_key(
    "is_default_enumeration_type");
const std::string is_floating_point_key("is_floating_point");
const std::string in_global_module_key("in_global_module");
const std::string name_key("name");
const std::string parents_key("parents");
const std::string refines_key("refines");
const std::string model_name_key("model_name");
const std::string bool_true("true");
const std::string bool_false("false");

const std::string documentation_key("documentation");
const std::string elements_key("elements");
const std::string attributes_key("attributes");
const std::string enumerators_key("enumerators");
const std::string stereotypes_key("stereotypes");

const std::string meta_type_key("meta_type");
const std::string meta_type_object_value("object");
const std::string meta_type_builtin_value("builtin");
const std::string meta_type_module_value("module");
const std::string meta_type_enumeration_value("enumeration");
const std::string meta_type_primitive_value("primitive");
const std::string meta_type_exception_value("exception");
const std::string meta_type_concept_value("concept");

const std::string unparsed_type_key("unparsed_type");
const std::string simple_key("simple");
const std::string external_modules_key("external_modules");
const std::string internal_modules_key("internal_modules");
const std::string annotations_key("annotation");

const std::string object_type_key("object_type");
const std::string object_type_smart_pointer_value("smart_pointer");
const std::string object_type_associative_container_value(
    "associative_container");
const std::string object_type_sequence_container_value("sequence_container");

const std::string invalid_json_file("Failed to parse JSON file");
const std::string invalid_option_in_json_file(
    "Failed to read option in JSON file: ");
const std::string invalid_path("Failed to find JSON path: ");
const std::string invalid_meta_type("Invalid value for meta type: ");
const std::string missing_module("Could not find module: ");
const std::string failed_to_open_file("Failed to open file: ");
const std::string invalid_object_type("Invalid or unsupported object type: ");
const std::string duplicate_element_id("Duplicate element id: ");
const std::string missing_name("JSON element name is mandatory.");

}

namespace dogen {
namespace yarn {
namespace json {

std::list<std::pair<std::string, std::string>>
hydrator::read_kvps(const boost::property_tree::ptree& pt) const {

    std::list<std::pair<std::string, std::string>> r;
    const auto i(pt.find(annotations_key));
    if (i == pt.not_found())
        return r;

    for (auto j(i->second.begin()); j != i->second.end(); ++j) {
        const auto field_name(j->first);
        const auto field_value(j->second.get_value<std::string>());
        r.push_back(std::make_pair(field_name, field_value));
    }
    return r;
}

annotations::scribble_group
hydrator::read_scribble_group(const boost::property_tree::ptree& pt,
    const annotations::scope_types st) const {

    annotations::scribble_group r;
    const auto kvps(read_kvps(pt));
    if (kvps.empty())
        return r;

    annotations::scribble sbl;
    sbl.entries(kvps);
    sbl.scope(st);
    r.parent(sbl);

    return r;
}

void hydrator::insert_scribble_group(const yarn::name& owner,
    const annotations::scribble_group& sg, intermediate_model& im) const {

    if (sg.parent().entries().empty() && sg.children().empty())
        return;

    const auto id(owner.id());
    const auto pair(std::make_pair(id, sg));
    const bool inserted(im.indices().scribble_groups().insert(pair).second);
    if (!inserted) {
        BOOST_LOG_SEV(lg, error) << duplicate_element_id << id;
        BOOST_THROW_EXCEPTION(hydration_error(duplicate_element_id + id));
    }
}

void hydrator::read_and_insert_scribble_group(const yarn::name& owner,
    const annotations::scope_types st, const boost::property_tree::ptree& pt,
    intermediate_model& im) const {
    const auto sg(read_scribble_group(pt, st));
    insert_scribble_group(owner, sg, im);
}

void hydrator::read_and_insert_scribble(const yarn::name& owner,
    const annotations::scope_types st, const boost::property_tree::ptree& pt,
    annotations::scribble_group& sg) const {

    const auto kvps(read_kvps(pt));
    if (kvps.empty())
        return;

    annotations::scribble sbl;
    sbl.entries(kvps);
    sbl.scope(st);

    /*
     * Note: we are inserting by simple name here rather than ID
     * because the dia frontend is also inserting by simple name and
     * its non-trivial to change it to insert by ID.
     * FIXME: use ID consistently.
     */
    const auto sn(owner.simple());
    const auto pair(std::make_pair(sn, sbl));
    const bool inserted(sg.children().insert(pair).second);
    if (!inserted) {
        BOOST_LOG_SEV(lg, error) << duplicate_element_id << sn;
        BOOST_THROW_EXCEPTION(hydration_error(duplicate_element_id + sn));
    }
}

std::vector<std::string> hydrator::
read_stereotypes(const boost::property_tree::ptree& pt) const {
    std::vector<std::string> r;
    const auto i(pt.find(stereotypes_key));
    if (i == pt.not_found() || i->second.empty())
        return r;

    for (auto j(i->second.begin()); j != i->second.end(); ++j)
        r.push_back(j->second.get_value<std::string>());

    return r;
}

name hydrator::read_name(const boost::property_tree::ptree& pt) const {
    yarn::name_builder b;
    const auto sn(pt.get<std::string>(simple_key));
    b.simple_name(sn);

    const auto im(pt.get<std::string>(internal_modules_key, empty));
    if (!im.empty())
        b.internal_modules(im);

    const auto r(b.build());
    return r;
}

name hydrator::read_name(const boost::property_tree::ptree& pt,
    const name& model_name, const bool in_global_module) const {

    /*
     * If we're not in the global module, we must be be in the
     * model. Note that we only handle element name's here, not the
     * model name itself.
     */
    yarn::name_builder b;
    if (!in_global_module) {
        b.model_name(model_name.location());
        b.external_modules(model_name.location().external_modules());
    }

    const auto sn(pt.get<std::string>(simple_key));
    b.simple_name(sn);

    const auto im(pt.get<std::string>(internal_modules_key, empty));
    if (!im.empty())
        b.internal_modules(im);

    const auto r(b.build());
    return r;
}

std::list<name>
hydrator::read_names(const boost::property_tree::ptree& pt) const {
    std::list<name> r;
    for (auto i(pt.begin()); i != pt.end(); ++i) {
        const auto& apt(i->second);
        r.push_back(read_name(apt));
    }
    return r;
}

std::string
hydrator::read_documentation(const boost::property_tree::ptree& pt) const {
    const auto opt(pt.get_optional<std::string>(documentation_key));
    if (!opt)
        return empty;

    auto r(*opt);
    boost::trim(r);
    return r;
}

std::vector<enumerator>
hydrator::read_enumerators(const boost::property_tree::ptree& pt,
    annotations::scribble_group& sg) const {

    std::vector<enumerator> r;
    for (auto i(pt.begin()); i != pt.end(); ++i) {
        const auto& apt(i->second);
        enumerator en;
        const auto j(apt.find(name_key));
        if (j == apt.not_found()) {
            BOOST_LOG_SEV(lg, error) << missing_name;
            BOOST_THROW_EXCEPTION(hydration_error(missing_name));
        }
        en.name(read_name(j->second));
        en.documentation(read_documentation(apt));

        const auto st(annotations::scope_types::property);
        read_and_insert_scribble(en.name(), st, apt, sg);
        r.push_back(en);
    }
    return r;
}

std::list<attribute>
hydrator::read_attributes(const boost::property_tree::ptree& pt,
    annotations::scribble_group& sg) const {
    std::list<attribute> r;

    for (auto i(pt.begin()); i != pt.end(); ++i) {
        const auto& apt(i->second);
        attribute a;

        const auto j(apt.find(name_key));
        if (j == apt.not_found()) {
            BOOST_LOG_SEV(lg, error) << missing_name;
            BOOST_THROW_EXCEPTION(hydration_error(missing_name));
        }
        a.name(read_name(j->second));

        a.unparsed_type(apt.get<std::string>(unparsed_type_key));
        a.documentation(read_documentation(apt));

        const auto st(annotations::scope_types::property);
        read_and_insert_scribble(a.name(), st, apt, sg);
        r.push_back(a);
    }
    return r;
}

void hydrator::populate_element(const boost::property_tree::ptree& pt,
    yarn::intermediate_model& im, yarn::element& e) const {

    const auto in_global_module(pt.get(in_global_module_key, false));
    const auto i(pt.find(name_key));
    if (i == pt.not_found()) {
        BOOST_LOG_SEV(lg, error) << missing_name;
        BOOST_THROW_EXCEPTION(hydration_error(missing_name));
    }

    yarn::name n(read_name(i->second, im.name(), in_global_module));
    const auto id(n.id());

    BOOST_LOG_SEV(lg, debug) << "Processing element: " << n.id();
    e.name(n);
    e.origin_type(origin_types::not_yet_determined);
    e.in_global_module(in_global_module);
    e.documentation(read_documentation(pt));
    e.stereotypes(read_stereotypes(pt));
}

void hydrator::read_object(const boost::property_tree::ptree& pt,
    yarn::intermediate_model& im) const {

    yarn::object o;
    populate_element(pt, im, o);

    const auto ot(pt.get_optional<std::string>(object_type_key));
    o.object_type(to_object_type(ot));

    const auto st(annotations::scope_types::entity);
    auto sg(read_scribble_group(pt, st));

    auto i(pt.find(attributes_key));
    if (i != pt.not_found())
        o.local_attributes(read_attributes(i->second, sg));

    insert_scribble_group(o.name(), sg, im);

    i = pt.find(parents_key);
    if (i != pt.not_found())
        o.parents(read_names(i->second));

    const auto id(o.name().id());
    const auto pair(std::make_pair(id, o));
    const bool inserted(im.objects().insert(pair).second);
    if (!inserted) {
        BOOST_LOG_SEV(lg, error) << duplicate_element_id << id;
        BOOST_THROW_EXCEPTION(hydration_error(duplicate_element_id + id));
    }
}

void hydrator::read_builtin(const boost::property_tree::ptree& pt,
    yarn::intermediate_model& im) const {

    yarn::builtin p;
    populate_element(pt, im, p);
    const auto st(annotations::scope_types::entity);
    read_and_insert_scribble_group(p.name(), st, pt, im);

    const auto dit(pt.get(is_default_enumeration_type_key, false));
    p.is_default_enumeration_type(dit);

    const auto ifp(pt.get(is_floating_point_key, false));
    p.is_floating_point(ifp);

    const auto id(p.name().id());
    const auto pair(std::make_pair(id, p));
    const bool inserted(im.builtins().insert(pair).second);
    if (!inserted) {
        BOOST_LOG_SEV(lg, error) << duplicate_element_id << id;
        BOOST_THROW_EXCEPTION(hydration_error(duplicate_element_id + id));
    }
}

void hydrator::read_module(const boost::property_tree::ptree& pt,
    yarn::intermediate_model& im) const {

    yarn::module m;
    populate_element(pt, im, m);
    const auto st(annotations::scope_types::entity);
    read_and_insert_scribble_group(m.name(), st, pt, im);

    const auto id(m.name().id());
    const auto pair(std::make_pair(id, m));
    const bool inserted(im.modules().insert(pair).second);
    if (!inserted) {
        BOOST_LOG_SEV(lg, error) << duplicate_element_id << id;
        BOOST_THROW_EXCEPTION(hydration_error(duplicate_element_id + id));
    }
}

void hydrator::read_enumeration(const boost::property_tree::ptree& pt,
    yarn::intermediate_model& im) const {

    yarn::enumeration e;
    populate_element(pt, im, e);
    const auto st(annotations::scope_types::entity);
    auto sg(read_scribble_group(pt, st));

    const auto i(pt.find(enumerators_key));
    if (i != pt.not_found())
        e.enumerators(read_enumerators(i->second, sg));

    insert_scribble_group(e.name(), sg, im);

    const auto id(e.name().id());
    const auto pair(std::make_pair(id, e));
    const bool inserted(im.enumerations().insert(pair).second);
    if (!inserted) {
        BOOST_LOG_SEV(lg, error) << duplicate_element_id << id;
        BOOST_THROW_EXCEPTION(hydration_error(duplicate_element_id + id));
    }
}

void hydrator::read_primitive(const boost::property_tree::ptree& pt,
    yarn::intermediate_model& im) const {

    yarn::primitive p;
    populate_element(pt, im, p);
    const auto st(annotations::scope_types::entity);
    read_and_insert_scribble_group(p.name(), st, pt, im);

    const auto id(p.name().id());
    const auto pair(std::make_pair(id, p));
    const bool inserted(im.primitives().insert(pair).second);
    if (!inserted) {
        BOOST_LOG_SEV(lg, error) << duplicate_element_id << id;
        BOOST_THROW_EXCEPTION(hydration_error(duplicate_element_id + id));
    }
}

void hydrator::read_exception(const boost::property_tree::ptree& pt,
    yarn::intermediate_model& im) const {

    yarn::exception e;
    populate_element(pt, im, e);
    const auto st(annotations::scope_types::entity);
    read_and_insert_scribble_group(e.name(), st, pt, im);

    const auto id(e.name().id());
    const auto pair(std::make_pair(id, e));
    const bool inserted(im.exceptions().insert(pair).second);
    if (!inserted) {
        BOOST_LOG_SEV(lg, error) << duplicate_element_id << id;
        BOOST_THROW_EXCEPTION(hydration_error(duplicate_element_id + id));
    }
}

void hydrator::read_concept(const boost::property_tree::ptree& pt,
    yarn::intermediate_model& im) const {

    yarn::concept c;
    populate_element(pt, im, c);
    const auto st(annotations::scope_types::entity);
    auto sg(read_scribble_group(pt, st));

    auto i(pt.find(attributes_key));
    if (i != pt.not_found())
        c.local_attributes(read_attributes(i->second, sg));

    insert_scribble_group(c.name(), sg, im);

    i = pt.find(refines_key);
    if (i != pt.not_found()) {
        for (auto j(i->second.begin()); j != i->second.end(); ++j)
            c.refines().push_back(read_name(j->second, im.name()));
    }

    const auto id(c.name().id());
    const auto pair(std::make_pair(id, c));
    const bool inserted(im.concepts().insert(pair).second);
    if (!inserted) {
        BOOST_LOG_SEV(lg, error) << duplicate_element_id << id;
        BOOST_THROW_EXCEPTION(hydration_error(duplicate_element_id + id));
    }
}

void hydrator::dispatch_meta_type(const boost::property_tree::ptree& pt,
    yarn::intermediate_model& im) const {

    const auto s(pt.get<std::string>(meta_type_key));
    if (s == meta_type_object_value)
        read_object(pt, im);
    else if (s == meta_type_builtin_value)
        read_builtin(pt, im);
    else if (s == meta_type_module_value)
        read_module(pt, im);
    else if (s == meta_type_enumeration_value)
        read_enumeration(pt, im);
    else if (s == meta_type_primitive_value)
        read_primitive(pt, im);
    else if (s == meta_type_exception_value)
        read_exception(pt, im);
    else if (s == meta_type_concept_value)
        read_concept(pt, im);
    else {
        BOOST_LOG_SEV(lg, error) << invalid_meta_type << s;
        BOOST_THROW_EXCEPTION(
            hydration_error(invalid_meta_type + s));
    }
}

yarn::intermediate_model
hydrator::read_stream(std::istream& s, const bool is_target) const {
    yarn::intermediate_model r;
    boost::property_tree::ptree pt;
    read_json(s, pt);

    yarn::name_factory nf;
    const auto mn(pt.get<std::string>(model_name_key));
    const auto em(pt.get<std::string>(external_modules_key, empty));
    r.name(nf.build_model_name(mn, em));
    BOOST_LOG_SEV(lg, debug) << "Processing model: " << r.name().id();

    const auto tg(origin_types::target);
    const auto nyd(origin_types::not_yet_determined);
    const auto ot(is_target ? tg : nyd);
    r.origin_type(ot);

    yarn::module m;
    const auto st(annotations::scope_types::root_module);
    read_and_insert_scribble_group(r.name(), st, pt, r);

    m.documentation(read_documentation(pt));
    m.name(r.name());
    m.origin_type(origin_types::not_yet_determined);
    r.modules().insert(std::make_pair(m.name().id(), m));

    const auto i(pt.find(elements_key));
    if (i == pt.not_found() || i->second.empty()) {
        BOOST_LOG_SEV(lg, warn) << "Did not find any elements in model";
    } else {
        for (auto j(i->second.begin()); j != i->second.end(); ++j)
            dispatch_meta_type(j->second, r);
    }

    return r;
}

yarn::object_types hydrator::
to_object_type(const boost::optional<std::string>& s) const {
    if (!s)
        return yarn::object_types::invalid;

    const auto ot(*s);
    if (ot == object_type_smart_pointer_value)
        return yarn::object_types::smart_pointer;
    else if (ot == object_type_associative_container_value)
        return yarn::object_types::associative_container;
    else if (ot == object_type_sequence_container_value)
        return yarn::object_types::sequence_container;

    BOOST_LOG_SEV(lg, error) << invalid_object_type << ot;
    BOOST_THROW_EXCEPTION(hydration_error(invalid_object_type + ot));
}

intermediate_model
hydrator::hydrate(std::istream& s, const bool is_target) const {
    BOOST_LOG_SEV(lg, debug) << "Parsing JSON stream.";
    using namespace boost::property_tree;
    try {
        auto r(read_stream(s, is_target));
        BOOST_LOG_SEV(lg, debug) << "Parsed JSON stream successfully.";
        return r;
    } catch (const json_parser_error& e) {
        BOOST_LOG_SEV(lg, error) << invalid_json_file << ": " << e.what();
        BOOST_THROW_EXCEPTION(hydration_error(invalid_json_file + e.what()));
    } catch (const ptree_bad_data& e) {
        BOOST_LOG_SEV(lg, error) << invalid_option_in_json_file << ": "
                                 << e.what();
        BOOST_THROW_EXCEPTION(
            hydration_error(invalid_option_in_json_file + e.what()));
    } catch (const ptree_bad_path& e) {
        BOOST_LOG_SEV(lg, error) << invalid_path << ": " << e.what();
        BOOST_THROW_EXCEPTION(hydration_error(invalid_path + e.what()));
    }
    BOOST_LOG_SEV(lg, debug) << "Parsed JSON stream successfully.";
}

intermediate_model hydrator::
hydrate(const boost::filesystem::path& p, const bool is_target) const {
    const auto gs(p.generic_string());
    BOOST_LOG_SEV(lg, debug) << "Parsing JSON file: " << gs;
    boost::filesystem::ifstream s(p);

    if (s.fail()) {
        BOOST_LOG_SEV(lg, error) << failed_to_open_file << ": " << gs;
        BOOST_THROW_EXCEPTION(hydration_error(failed_to_open_file + gs));
    }

    const auto r(hydrate(s, is_target));
    BOOST_LOG_SEV(lg, debug) << "Parsed JSON file successfully.";
    return r;
}

} } }
