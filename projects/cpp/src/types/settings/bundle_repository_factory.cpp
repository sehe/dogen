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
#include <boost/throw_exception.hpp>
#include "dogen/utility/log/logger.hpp"
#include "dogen/yarn/types/name_factory.hpp"
#include "dogen/yarn/types/object.hpp"
#include "dogen/yarn/types/primitive.hpp"
#include "dogen/yarn/types/enumeration.hpp"
#include "dogen/yarn/types/concept.hpp"
#include "dogen/yarn/types/module.hpp"
#include "dogen/yarn/types/element_visitor.hpp"
#include "dogen/cpp/io/settings/bundle_repository_io.hpp"
#include "dogen/cpp/types/settings/bundle_factory.hpp"
#include "dogen/cpp/types/settings/building_error.hpp"
#include "dogen/cpp/types/settings/bundle_repository_factory.hpp"

namespace {

using namespace dogen::utility::log;
static logger lg(logger_factory("cpp.settings.bundle_repository_factory"));

const std::string registrar_name("registrar");
const std::string duplicate_name("Duplicate name: ");

}

namespace dogen {
namespace cpp {
namespace settings {

namespace {

/**
 * @brief Generates all inclusion dependencies.
 */
class generator final : public yarn::element_visitor {
public:
    explicit generator(const bundle_factory& f) : factory_(f) {}

private:
    /**
     * @brief Generates all of the inclusion dependencies for the
     * formatters and qualified name.
     */
    template<typename YarnEntity>
    void generate(const YarnEntity& e) {
        if (e.generation_type() == yarn::generation_types::no_generation)
            return;

        const auto b(factory_.make(e.extensions()));
        const auto pair(std::make_pair(e.name(), b));
        auto& deps(result_.bundles_by_name());
        const auto res(deps.insert(pair));
        if (!res.second) {
            const auto qn(e.name().qualified());
            BOOST_LOG_SEV(lg, error) << duplicate_name << qn;
            BOOST_THROW_EXCEPTION(building_error(duplicate_name + qn));
        }
    }

public:
    using yarn::element_visitor::visit;
    void visit(const dogen::yarn::object& o) { generate(o); }
    void visit(const dogen::yarn::enumeration& e) { generate(e); }
    void visit(const dogen::yarn::primitive& p) { generate(p); }
    void visit(const dogen::yarn::module& m) { generate(m); }
    void visit(const dogen::yarn::concept& c) { generate(c); }

public:
    const bundle_repository& result() const { return result_; }

private:
    const bundle_factory& factory_;
    bundle_repository result_;
};

}

bundle_repository bundle_repository_factory::
make(const dynamic::repository& rp, const dynamic::object& root_object,
    const dogen::formatters::general_settings_factory& gsf,
    const opaque_settings_builder& osb,
    const yarn::model& m) const {

    BOOST_LOG_SEV(lg, debug) << "Creating settings bundle repository.";

    const bundle_factory f(rp, root_object, gsf, osb);
    generator g(f);
    for (const auto& pair : m.elements()) {
        const auto& e(*pair.second);
        e.accept(g);
    }
    auto r(g.result());

    // FIXME: hack to handle registars.
    yarn::name_factory nf;
    const auto n(nf.build_element_in_model(m.name(), registrar_name));
    const auto pair(std::make_pair(n, f.make()));
    auto& deps(r.bundles_by_name());
    const auto res(deps.insert(pair));
    if (!res.second) {
        const auto qn(n.qualified());
        BOOST_LOG_SEV(lg, error) << duplicate_name << qn;
        BOOST_THROW_EXCEPTION(building_error(duplicate_name + qn));
    }

    BOOST_LOG_SEV(lg, debug) << "Finished creating settings bundle repository."
                             << r;
    return r;
}

} } }
