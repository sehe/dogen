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
#include "dogen/utility/log/logger.hpp"
#include "dogen/formatters/types/indent_filter.hpp"
#include "dogen/formatters/types/comment_formatter.hpp"
#include "dogen/yarn/types/name_flattener.hpp"
#include "dogen/quilt.csharp/types/formatters/formatting_error.hpp"
#include "dogen/quilt.csharp/types/formatters/assistant.hpp"

namespace {

using namespace dogen::utility::log;
static logger lg(logger_factory("quilt.csharp.formatters.assistant"));

const bool start_on_first_line(true);
const bool use_documentation_tool_markup(true);
const bool last_line_is_blank(true);
const bool documenting_previous_identifier(true);

const std::string artefact_properties_missing(
    "Could not find formatter configuration for formatter: ");

}

namespace dogen {
namespace quilt {
namespace csharp {
namespace formatters {

assistant::
assistant(const context& ctx, const annotations::archetype_location& al,
    const std::string& id) :
    element_id_(id), context_(ctx),
    artefact_properties_(obtain_artefact_properties(al.archetype())),
    archetype_location_(al) {

    BOOST_LOG_SEV(lg, debug) << "Processing element: " << element_id_
                             << " for archetype: " << al.archetype();

    dogen::formatters::indent_filter::push(filtering_stream_, 4);
    filtering_stream_.push(stream_);
}

const formattables::artefact_properties& assistant::
obtain_artefact_properties(const std::string& archetype) const {
    const auto& eprops(context_.element_properties());
    const auto i(eprops.artefact_properties().find(archetype));
    if (i == eprops.artefact_properties().end()) {
        BOOST_LOG_SEV(lg, error) << artefact_properties_missing
                                 << archetype;
        BOOST_THROW_EXCEPTION(
            formatting_error(artefact_properties_missing + archetype));
    }
    return i->second;
}

dogen::formatters::csharp::scoped_boilerplate_formatter
assistant::make_scoped_boilerplate_formatter() {
    const auto& ep(context_.element_properties());
    const auto& dp(ep.decoration_properties());

    const auto& art_props(artefact_properties_);
    const auto& deps(art_props.using_dependencies());

    using dogen::formatters::csharp::scoped_boilerplate_formatter;
    return scoped_boilerplate_formatter(stream(), dp, deps);
}

dogen::formatters::csharp::scoped_namespace_formatter
assistant::make_scoped_namespace_formatter(const std::list<std::string>& ns) {
    return dogen::formatters::csharp::scoped_namespace_formatter(
        stream(), ns, true/*add_new_line*/);
}

std::list<std::string> assistant::make_namespaces(const yarn::name& n) const {
    yarn::name_flattener nf;
    return nf.flatten(n);
}

void assistant::comment(const std::string& c) {
    if (c.empty())
        return;

    {
        dogen::formatters::positive_indenter_scope pis(stream());
        dogen::formatters::comment_formatter f(
            start_on_first_line,
            use_documentation_tool_markup,
            !documenting_previous_identifier,
            dogen::formatters::comment_styles::csharp_style,
            !last_line_is_blank);
        f.format(stream(), c);
    }
}

std::ostream& assistant::stream() {
    return filtering_stream_;
}

dogen::formatters::artefact assistant::make_artefact() const {
    dogen::formatters::artefact r;
    r.content(stream_.str());
    r.path(artefact_properties_.file_path());
    r.overwrite(artefact_properties_.overwrite());

    return r;
}

} } } }