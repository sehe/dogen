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
#include <ostream>
#include "dogen/generator/generation_failure.hpp"
#include "dogen/generator/backends/cpp/formatters/cpp_licence.hpp"
#include "dogen/generator/backends/cpp/formatters/cpp_header_guards.hpp"
#include "dogen/generator/backends/cpp/formatters/cpp_namespace.hpp"
#include "dogen/generator/backends/cpp/formatters/cpp_includes.hpp"
#include "dogen/generator/backends/cpp/formatters/cpp_namespace_helper.hpp"
#include "dogen/generator/backends/cpp/formatters/cpp_qualified_name.hpp"
#include "dogen/generator/backends/cpp/formatters/cpp_indenter.hpp"
#include "dogen/generator/backends/cpp/formatters/cpp_generator_implementation.hpp"

namespace {

const std::string int_type("int");
const std::string bool_type("bool");
const std::string string_type("std::string");
const std::string invalid_sequence_container(
    "Sequence containers have exactly one type argument");
const std::string invalid_associative_container(
    "Associative containers have exactly two type arguments");
const std::string missing_class_view_model(
    "Meta type is pod but class view model is empty");
const std::string missing_enumeration_view_model(
    "Meta type is enumeration but enumeration view model is empty");

}

namespace dogen {
namespace generator {
namespace backends {
namespace cpp {
namespace formatters {

generator_implementation::
generator_implementation(std::ostream& stream) :
    stream_(stream),
    utility_(stream_, indenter_) { }

file_formatter::shared_ptr
generator_implementation::create(std::ostream& stream) {
    return file_formatter::shared_ptr(new generator_implementation(stream));
}

void generator_implementation::
sequence_container_helper(const std::string& container_identifiable_type_name,
    const std::string& container_type_name,
    unsigned int quantity,
    const std::string& containee_identifiable_type_name) {
    stream_ << indenter_ << container_type_name << " create_"
            << container_identifiable_type_name
            << "(unsigned int position) ";

    utility_.open_scope();
    {
        cpp_positive_indenter_scope s(indenter_);
        stream_ << indenter_ << container_type_name << " r;"
                << std::endl;
        stream_ << indenter_ << "for (unsigned int i(0); i < " << quantity
                << "; ++i) ";
        utility_.open_scope();
        {
            cpp_positive_indenter_scope s(indenter_);
            stream_ << indenter_ << "r.push_back(create_"
                    << containee_identifiable_type_name
                    << "(position + i));" << std::endl;
        }
        utility_.close_scope();
        stream_ << indenter_ << "return r;" << std::endl;
    }
    utility_.close_scope();
    utility_.blank_line();
}

void generator_implementation::
associative_container_helper(const std::string& container_identifiable_type_name,
    const std::string& container_type_name,
    unsigned int quantity,
    const std::string& containee_identifiable_type_name) {
    stream_ << indenter_ << container_type_name << " create_"
            << container_identifiable_type_name
            << "(unsigned int position) ";

    utility_.open_scope();
    {
        cpp_positive_indenter_scope s(indenter_);
        stream_ << indenter_ << container_type_name << " r;"
                << std::endl;
        stream_ << indenter_ << "for (unsigned int i(0); i < " << quantity
                << "; ++i) ";
        utility_.open_scope();
        {
            cpp_positive_indenter_scope s(indenter_);
            stream_ << indenter_ << "r.insert(create_"
                    << containee_identifiable_type_name
                    << "(position + i));" << std::endl;
        }
        utility_.close_scope();
        stream_ << indenter_ << "return r;" << std::endl;
    }
    utility_.close_scope();
    utility_.blank_line();
}

// void generator_implementation::
// associative_container_helper(const std::string& identifiable_type_name,
//     const std::string& type_name, unsigned int quantity) {
//     stream_ << indenter_ << type_name << std::endl
//             << "create_" << identifiable_type_name
//             << "(const unsigned int position) ";

//     utility_.open_scope();
//     {
//         cpp_positive_indenter_scope s(indenter_);
//         stream_ << indenter_ << "return " << type_name
//                 << "_generator::create(position);" << std::endl;
//     }
//     utility_.close_scope();
// }

void generator_implementation::
domain_type_helper(const std::string& identifiable_type_name,
    const std::string& type_name) {
    stream_ << indenter_ << type_name << std::endl
            << "create_" << identifiable_type_name
            << "(const unsigned int position) ";

    utility_.open_scope();
    {
        cpp_positive_indenter_scope s(indenter_);
        stream_ << indenter_ << "return " << type_name
                << "_generator::create(position);" << std::endl;
    }
    utility_.close_scope();
}

void generator_implementation::bool_helper() {
    stream_ << indenter_ << "bool create_bool(const unsigned int position) ";
    utility_.open_scope();
    {
        cpp_positive_indenter_scope s(indenter_);
        stream_ << indenter_ << "return (position % 2) == 0;" << std::endl;
    }
    utility_.close_scope();
}

void generator_implementation::string_helper() {
    stream_ << indenter_ << "std::string create_std_string"
            << "(const unsigned int position) ";

    utility_.open_scope();
    {
        cpp_positive_indenter_scope s(indenter_);
        stream_ << indenter_ << "std::ostringstream s;" << std::endl
                << indenter_ << "s << " << utility_.quote("a_string_")
                << " << position;" << std::endl;
        stream_ << indenter_ << "return s.str();" << std::endl;
    }
    utility_.close_scope();
}

void generator_implementation::
char_like_helper(const std::string& identifiable_type_name,
    const std::string& type_name) {
    stream_ << indenter_ << type_name << " create_" << identifiable_type_name
            << "(const unsigned int position) ";

    utility_.open_scope();
    {
        cpp_positive_indenter_scope s(indenter_);
        stream_ << indenter_
                << "return static_cast<" << type_name
                << ">(((position % 95) + 32) == 34) ? 35 :"
                << " ((position % 95) + 32);"
                << std::endl;
    }
    utility_.close_scope();
}

void generator_implementation::
int_like_helper(const std::string& identifiable_type_name,
    const std::string& type_name) {
    stream_ << indenter_ << type_name << " create_" << identifiable_type_name
            << "(const unsigned int position) ";

    utility_.open_scope();
    {
        cpp_positive_indenter_scope s(indenter_);
        if (type_name == int_type)
            stream_ << indenter_ << "return position;";
        else
            stream_ << indenter_ << "return static_cast<" << type_name
                    << ">(position);";

        stream_ << std::endl;
    }
    utility_.close_scope();
}

void generator_implementation::
recursive_helper_method_creator(const nested_type_view_model& vm,
    std::unordered_set<std::string>& types_done) {

    if (types_done.find(vm.complete_identifiable_name()) != types_done.end())
        return;

    const auto children(vm.children());
    for (const auto c : children)
        recursive_helper_method_creator(c, types_done);

    if (vm.is_primitive()) {
        if (vm.is_char_like()) {
            char_like_helper(vm.identifiable_name(), vm.name());
            utility_.blank_line();
        } else if (vm.is_int_like()) {
            int_like_helper(vm.identifiable_name(), vm.name());
            utility_.blank_line();
        } else if (vm.name() == bool_type) {
            bool_helper();
            utility_.blank_line();
        }
    } else if (vm.is_sequence_container()) {
        if (children.size() != 1)
            throw generation_failure(invalid_sequence_container);

        const auto containee_vm(children.front());
        sequence_container_helper(
            vm.complete_identifiable_name(), vm.complete_name(), 10,
            containee_vm.complete_identifiable_name());
    } else if (vm.is_associative_container()) {
        if (children.size() == 1) {
            const auto containee_vm(children.front());
            associative_container_helper(
                vm.complete_identifiable_name(), vm.complete_name(), 10,
                containee_vm.complete_identifiable_name());
        }
    } else {
        if (vm.name() == string_type) {
            string_helper();
            utility_.blank_line();
        } else {
            domain_type_helper(vm.identifiable_name(), vm.name());
            utility_.blank_line();
        }
    }
    types_done.insert(vm.complete_identifiable_name());
}

void generator_implementation::
create_helper_methods(const class_view_model& vm) {
    const auto props(vm.properties());
    if (props.empty())
        return;

    namespace_helper ns_helper(stream_, std::list<std::string> { });
    std::unordered_set<std::string> types_done;

    utility_.blank_line();
    for (const auto p : props)
        recursive_helper_method_creator(p.type(), types_done);
}

void generator_implementation::populate_method(const class_view_model& vm) {
    const auto props(vm.properties());
    if (props.empty())
        return;

    const std::string name(vm.name() + "_generator");
    stream_ << indenter_ << "void " << name << "::" << std::endl
            << "populate(const unsigned int position, result_type& v) " ;

    utility_.open_scope();
    {
        cpp_positive_indenter_scope s(indenter_);
        unsigned int j(0);
        for (const auto p : props) {
            stream_ << indenter_ << "v." << p.name() << "("
                    << "create_" << p.type().complete_identifiable_name()
                    << "(position + " << j << "));" << std::endl;
            ++j;
        }
    }
    utility_.close_scope();
}

void generator_implementation::create_method(const class_view_model& vm) {
    if (vm.is_parent())
        return;

    const bool has_properties(!vm.properties().empty());
    const auto parents(vm.parents());
    const bool has_parents(!parents.empty());
    const bool has_properties_or_parents(has_properties || has_parents);

    const std::string name(vm.name() + "_generator");
    stream_ << indenter_ << name << "::result_type" << std::endl
            << name << "::create(const unsigned int"
            << (has_properties_or_parents ? " position" : "/*position*/")
            << ") ";

    utility_.open_scope();
    {
        cpp_positive_indenter_scope s(indenter_);
        stream_ << indenter_ << vm.name() << " r;" << std::endl;

        for (const auto p : parents) {
            stream_ << indenter_;
            cpp_qualified_name qualified_name(stream_);
            qualified_name.format(p);
            stream_ << "_generator::populate(position, r);"
                    << std::endl;
        }

        if (has_properties) {
            stream_ << indenter_ << name << "::populate(position, r);"
                    << std::endl;
        }
        stream_ << indenter_ << "return r;" << std::endl;
    }
    utility_.close_scope();
}

void generator_implementation::function_operator(const class_view_model& vm) {
    if (vm.is_parent())
        return;

    const std::string name(vm.name() + "_generator");
    stream_ << indenter_ << name << "::result_type" << std::endl
            << name << "::operator()() ";

    utility_.open_scope();
    {
        cpp_positive_indenter_scope s(indenter_);
        stream_ << indenter_ << "return create("
                << utility_.as_member_variable("position") << "++);"
                << std::endl;
    }
    utility_.close_scope();
}

void generator_implementation::default_constructor(const class_view_model& vm) {
    if (vm.is_parent())
        return;

    const std::string name(vm.name() + "_generator");
    stream_ << indenter_ << name << "::" << name << "() : position_(0) { }";
    utility_.blank_line();
}

void generator_implementation::format_class(const file_view_model& vm) {
    boost::optional<view_models::class_view_model> o(vm.class_vm());
    if (!o)
        throw generation_failure(missing_class_view_model);

    const class_view_model& cvm(*o);
    create_helper_methods(cvm);
    utility_.blank_line(2);

    {
        std::list<std::string> ns(cvm.namespaces());
        namespace_helper ns_helper(stream_, ns);

        utility_.blank_line();
        default_constructor(cvm);
        utility_.blank_line();
        populate_method(cvm);
        utility_.blank_line();
        create_method(cvm);
        utility_.blank_line();
        function_operator(cvm);
        utility_.blank_line();
    }
    utility_.blank_line();
}

void generator_implementation::format_enumeration(const file_view_model& vm) {
    const auto o(vm.enumeration_vm());
    if (!o)
        throw generation_failure(missing_enumeration_view_model);

    const auto evm(*o);
    {
        std::list<std::string> ns(evm.namespaces());
        namespace_helper ns_helper(stream_, ns);

        utility_.blank_line();
        const std::string name(evm.name() + "_generator");
        stream_ << indenter_ << name << "::" << name << "() : position_(0) { }";
        utility_.blank_line();

        stream_ << indenter_ << "void " << name << "::" << std::endl
                << "populate(const unsigned int position, result_type& v) " ;

        utility_.open_scope();
        {
            cpp_positive_indenter_scope s(indenter_);
            stream_ << indenter_ << "v = static_cast<" << evm.name() << ">"
                    << "(position % " << evm.enumerators().size() << ");"
                    << std::endl;
        }
        utility_.close_scope();
        utility_.blank_line();

        stream_ << indenter_ << name << "::result_type" << std::endl
                << name << "::create(const unsigned int  position) ";
        utility_.open_scope();
        {
            {
                cpp_positive_indenter_scope s(indenter_);
                stream_ << indenter_ << "result_type r;" << std::endl;
                stream_ << indenter_ << name << "::populate(position, r);"
                        << std::endl;
                stream_<< indenter_ << "return r;" << std::endl;
            }
        }
        utility_.close_scope();
        utility_.blank_line();

        stream_ << indenter_ << name << "::result_type" << std::endl
                << name << "::operator()() ";

        utility_.open_scope();
        {
            cpp_positive_indenter_scope s(indenter_);
            stream_ << indenter_ << "return create("
                    << utility_.as_member_variable("position") << "++);"
                    << std::endl;
        }
        utility_.close_scope();
        utility_.blank_line();
    }
}

void generator_implementation::format(const file_view_model& vm) {
    licence licence(stream_);
    licence.format();

    cpp_includes includes(stream_);
    includes.format(vm);

    if (vm.meta_type() == sml::meta_types::enumeration)
        format_enumeration(vm);
    else if (vm.meta_type() == sml::meta_types::pod)
        format_class(vm);
}

} } } } }
