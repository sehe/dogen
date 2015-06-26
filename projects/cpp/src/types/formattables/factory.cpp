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
#include "dogen/cpp/types/formattables/factory.hpp"

namespace dogen {
namespace cpp {
namespace formattables {

std::shared_ptr<formattable> factory::
make_registrar_info(const sml::model& /*m*/) const {
    /*
      BOOST_LOG_SEV(lg, debug) << "Transforming model into registrar: "
      << sml::string_converter::convert(model_.name());

      auto ri(boost::make_shared<cpp::formattables::registrar_info>());
      ri->namespaces(to_namespace_list(model_.name()));

      for (const auto& pair : model_.references()) {
      if (pair.second != sml::origin_types::system) {
      const auto l(to_namespace_list(pair.first));
      const auto s(boost::algorithm::join(l, namespace_separator));
      ri->model_dependencies().push_back(s);
      }
      }

      for (const auto& l : model_.leaves())
      ri->leaves().push_back(to_qualified_name(l));
      ri->leaves().sort();

      sml::qname qn;
      qn.simple_name(registrar_name);
      qn.model_name(model_.name().model_name());
      qn.external_module_path(model_.name().external_module_path());
      context_.registrars().insert(std::make_pair(qn, ri));

      BOOST_LOG_SEV(lg, debug) << "Transformed model into registrar.";
    */
    std::shared_ptr<registrar_info> r(new registrar_info());
    return r;
}

std::forward_list<std::shared_ptr<formattable> > factory::
make_includers(const formattables::path_derivatives_repository& /*rp*/) const {
    std::shared_ptr<includers_info> inc(new includers_info());
    std::forward_list<std::shared_ptr<formattable> > r;
    r.push_front(inc);
    return r;
}

std::forward_list<std::shared_ptr<formattable> > factory::
make_cmakelists(const config::cpp_options& /*o*/, const sml::model& /*m*/) const
{
    /*
    std::forward_list<formatters::file> r;
    if (options_.cpp().disable_cmakelists()) {
        BOOST_LOG_SEV(lg, info) << "CMakeLists generation disabled.";
        return r;
    }

    {
        const auto path(p.src_cmakelists().file_path());
        BOOST_LOG_SEV(lg, debug) << "Formatting: " << path.string();

        std::ostringstream s;
        cpp_formatters::src_cmakelists fmt(s);
        fmt.format(p.src_cmakelists());

        formatters::file file;
        file.path(path);
        file.content(s.str());
        r.push_front(file);
    }

    if (p.include_cmakelists()) {
        const auto path(p.include_cmakelists()->file_path());
        BOOST_LOG_SEV(lg, debug) << "Formatting: " << path.string();

        const auto fct(options_.cpp().enabled_facets());
        const auto i(fct.find(config::cpp_facet_types::odb));
        const bool is_odb_enabled(i != fct.end());
        const auto fct_folder(options_.cpp().odb_facet_folder());

        std::ostringstream s;
        cpp_formatters::include_cmakelists fmt(s, is_odb_enabled, fct_folder);
        fmt.format(*p.include_cmakelists());

        formatters::file file;
        file.path(path);
        file.content(s.str());
        r.push_front(file);
    }

    return r;

    */
    std::shared_ptr<cmakelists_info> cm(new cmakelists_info());
    std::forward_list<std::shared_ptr<formattable> > r;
    r.push_front(cm);
    return r;
}

std::shared_ptr<formattable> factory::make_odb_options(
    const config::cpp_options& /*o*/, const sml::model& /*m*/) const {
    /*
      const auto fcts(options_.cpp().enabled_facets());
      const auto i(fcts.find(config::cpp_facet_types::odb));
      const bool is_odb_enabled(i != fcts.end());
      if (!is_odb_enabled) {
      BOOST_LOG_SEV(lg, info) << "ODB options file generation disabled.";
      return r;
      }

      const auto path(p.odb_options().file_path());
      BOOST_LOG_SEV(lg, debug) << "Formatting:" << path.string();

      std::ostringstream s;
      cpp_formatters::odb_options f(s);
      f.format(p.odb_options());

      formatters::file file;
      file.path(path);
      file.content(s.str());
      r.push_front(file);


     */
    std::shared_ptr<odb_options_info> r(new odb_options_info());
    return r;
}

} } }