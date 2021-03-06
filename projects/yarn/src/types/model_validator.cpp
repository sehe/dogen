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
#include <regex>
#include <array>
#include <algorithm>
#include <unordered_set>
#include <boost/throw_exception.hpp>
#include "dogen/utility/log/logger.hpp"
#include "dogen/utility/io/list_io.hpp"
#include "dogen/utility/string/splitter.hpp"
#include "dogen/yarn/io/name_io.hpp"
#include "dogen/yarn/types/decomposer.hpp"
#include "dogen/yarn/types/validation_error.hpp"
#include "dogen/yarn/types/model_validator.hpp"

typedef boost::error_info<struct owner, std::string>
errmsg_validation_owner;

namespace {

using namespace dogen::utility::log;
auto lg(logger_factory("yarn.model_validator"));

/*
 * FIXME: we've removed the following keywords for now because yarn
 * uses these terms: module, concept. We need to first rename it to
 * package or somesuch other name and then add it back.
 */
std::array<std::string, 81> cpp_reserved = { {
    "alignas", "alignof", "and", "and_eq", "asm", "atomic_cancel",
    "atomic_commit", "atomic_noexcept", "auto", "bitand", "bitor",
    "break", "case", "catch", "class", "compl", "const",
    "const_cast", "constexpr", "continue", "decltype", "default", "delete",
    "do", "dynamic_cast", "else", "enum", "explicit", "export", "extern",
    "false", "for", "friend", "goto", "if", "import", "inline",
    "mutable", "namespace", "new", "noexcept", "not",
    "not_eq", "nullptr", "operator", "or", "or_eq", "private", "protected",
    "public", "register", "reinterpret_cast", "requires", "return", "sizeof",
    "static", "static_assert", "static_cast", "struct", "switch",
    "synchronized", "template", "this", "thread_local", "throw", "true", "try",
    "typedef", "typeid", "typename", "union", "using", "virtual", "void",
    "volatile", "wchar_t", "while", "xor", "xor_eq", "xor_eqalignas"
    } };

std::array<std::string, 11> cpp_builtins = { {
    "char16_t", "char32_t", "unsigned", "bool", "char", "double", "float",
    "int", "long", "short", "signed"
    }
};

const std::string duplicate_element("Element has an ID already defined: ");
const std::string invalid_string(
    "String is not valid according to the identifier regular expression: ");
const std::string reserved_keyword(
    "String matches a resvered keyword on one of the supported languages: ");
const std::string builtin_name("String matches the name of a built in type: ");
const std::string abstract_instance("Attempt to instantiate an abstract type: ");
const std::string space(" ");

}

namespace dogen {
namespace yarn {

bool model_validator::allow_spaces_in_built_in_types(const languages l) const {
    return l == languages::cpp;
}

decomposition_result model_validator::decompose_model(const model& m) const {
    BOOST_LOG_SEV(lg, debug) << "Decomposing model: " << m.name().id();

    /*
     * Collect the names of all elements and attributes.
     */
    decomposer dc;
    for (const auto& ptr : m.elements()) {
        const auto& e(*ptr);
        e.accept(dc);
    }

    /*
     * Note that we do not add the model name itself; this is because
     * we will validate the model's module, which is generated from
     * the model name.
     */
    return dc.result();
}

void model_validator::
validate_string(const std::string& s, bool check_not_builtin) const {
    static std::regex name_regex("^[a-zA-Z_][a-zA-Z0-9_]*$");
    if (!std::regex_match(s, name_regex)) {
        BOOST_LOG_SEV(lg, error) << invalid_string << "'" << s << "'";
        BOOST_THROW_EXCEPTION(validation_error(invalid_string + s));
    }

    const auto i(std::find(cpp_reserved.begin(), cpp_reserved.end(), s));
    if (i != cpp_reserved.end()) {
        BOOST_LOG_SEV(lg, error) << reserved_keyword << s;
        BOOST_THROW_EXCEPTION(validation_error(reserved_keyword + s));
    }

    if (check_not_builtin) {
        const auto j(std::find(cpp_builtins.begin(), cpp_builtins.end(), s));
        if (j != cpp_builtins.end()) {
            BOOST_LOG_SEV(lg, error) << builtin_name << s;
            BOOST_THROW_EXCEPTION(validation_error(builtin_name + s));
        }
    }

    BOOST_LOG_SEV(lg, debug) << "String passed all sanity checks: " << s;
}

void model_validator::
validate_strings(const std::list<std::string>& strings) const {
    for (const auto& s : strings)
        validate_string(s);
}

void model_validator::
validate_name(const name& n, const bool allow_spaces_in_built_in_types) const {
    /*
     * Built-in types are defined at the global namespace level; if we
     * are at the global namespace level, then built-ins are valid
     * element names. In this case, we should not check the name
     * against the built-in list.
     */
    const auto& l(n.location());
    const bool at_global_namespace(l.external_modules().empty() &&
        l.model_modules().empty() &&
        l.internal_modules().empty());
    const bool check_not_builtin(!at_global_namespace);

    /*
     * Sanity check all of the components of the name. Bit of a hack
     * here: if we are at global namespace and our model is in a
     * language where built-ins can have spaces, we split the name on
     * space and validate each component separately.
     */
    BOOST_LOG_SEV(lg, debug) << "at_global_namespace: "
                             << at_global_namespace
                             << " allow_spaces_in_built_in_types: "
                             << allow_spaces_in_built_in_types;

    if (at_global_namespace && allow_spaces_in_built_in_types) {
        using utility::string::splitter;
        const auto splitted(splitter::split_delimited(n.simple(), space));
        BOOST_LOG_SEV(lg, debug) << "Splitted simple name: " << splitted;
        for (const auto& s : splitted)
            validate_string(s, check_not_builtin);
    } else
        validate_string(n.simple(), check_not_builtin);

    validate_strings(l.external_modules());
    validate_strings(l.model_modules());
    validate_strings(l.internal_modules());

    if (!l.element().empty())
        validate_string(l.element());
}

void model_validator::
validate_names(const std::list<std::pair<std::string, name>>& names,
    const languages l) const {
    BOOST_LOG_SEV(lg, debug) << "Sanity checking all names.";
    std::unordered_set<std::string> ids_done;

    for (const auto& pair : names) {
        const auto& owner(pair.first);
        const auto& n(pair.second);
        const auto& id(n.id());
        BOOST_LOG_SEV(lg, debug) << "Validating: '" << id << "'";

        try {
            /*
             * Element identifier must be unique across all model
             * elements.
             */
            const auto inserted(ids_done.insert(id).second);
            if (!inserted) {
                BOOST_LOG_SEV(lg, error) << duplicate_element << id;
                BOOST_THROW_EXCEPTION(validation_error(duplicate_element + id));
            }

            /*
             * Element name must pass all sanity checks.
             */
            const bool allow_spaces(allow_spaces_in_built_in_types(l));
            validate_name(n, allow_spaces);

            BOOST_LOG_SEV(lg, debug) << "Name is valid.";
        } catch (boost::exception& e) {
            e << errmsg_validation_owner(owner);
            throw;
        }
    }
    BOOST_LOG_SEV(lg, debug) << "Finished validating all names.";
}

void model_validator::
validate_name_tree(const std::unordered_set<std::string>& abstract_elements,
    const languages l, const name_tree& nt,
    const bool inherit_opaqueness_from_parent) const {

    const auto& ae(abstract_elements);
    const auto id(nt.current().id());
    const bool is_abstract(ae.find(id) != ae.end());
    if (is_abstract && !inherit_opaqueness_from_parent) {
        BOOST_LOG_SEV(lg, error) << abstract_instance << id;
        BOOST_THROW_EXCEPTION(validation_error(abstract_instance + id));
    }

    for (const auto& c : nt.children())
        validate_name_tree(ae, l, c, nt.are_children_opaque());
}

void model_validator::validate_name_trees(
    const std::unordered_set<std::string>& abstract_elements, const languages l,
    const std::list<std::pair<std::string, name_tree>>& nts) const {

    /*
     * The only validation we perform on name trees at present is done
     * just for c++, so we can ignore all other languages.
     */
    if (l != languages::cpp)
        return;

    for (const auto& pair : nts) {
        const auto& owner(pair.first);
        const auto& nt(pair.second);
        BOOST_LOG_SEV(lg, trace) << "Validating: '" << nt.identifiable() << "'";

        try {
            validate_name_tree(abstract_elements, l, nt);
        } catch (boost::exception& e) {
            e << errmsg_validation_owner(owner);
            throw;
        }
    }
}

void model_validator::validate(const model& m) const {
    BOOST_LOG_SEV(lg, debug) << "Started validation. Model: " << m.name().id();

    const auto l(m.input_language());
    const auto dr(decompose_model(m));
    validate_names(dr.names(), l);
    validate_name_trees(dr.abstract_elements(), l, dr.name_trees());

    BOOST_LOG_SEV(lg, debug) << "Finished validation.";
}

} }
