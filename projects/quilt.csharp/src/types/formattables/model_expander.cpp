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
#include "dogen/quilt.csharp/types/formattables/decoration_expander.hpp"
#include "dogen/quilt.csharp/types/formattables/reducer.hpp"
#include "dogen/quilt.csharp/types/formattables/file_path_expander.hpp"
#include "dogen/quilt.csharp/types/formattables/model_expander.hpp"

namespace dogen {
namespace quilt {
namespace csharp {
namespace formattables {

void model_expander::expand_decoration(
    const dogen::formatters::decoration_properties_factory& dpf,
    model& fm) const {
    decoration_expander ex;
    ex.expand(dpf, fm);
}

void model_expander::expand_file_paths(
    const formatters::repository& frp, const locator& l, model& fm) const {
    file_path_expander ex;
    ex.expand(frp, l, fm);
}

void model_expander::reduce(model& fm) const {
    reducer rd;
    rd.reduce(fm);
}

void model_expander::expand(
    const annotations::type_repository& /*atrp*/,
    const annotations::annotation& /*ra*/,
    const dogen::formatters::decoration_properties_factory& dpf,
    const formatters::repository& frp, const locator& l, model& fm) const {

    reduce(fm);
    expand_decoration(dpf, fm);
    expand_file_paths(frp, l, fm);
}

} } } }
