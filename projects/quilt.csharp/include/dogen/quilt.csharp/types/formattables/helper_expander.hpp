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
#ifndef DOGEN_QUILT_CSHARP_TYPES_FORMATTABLES_HELPER_EXPANDER_HPP
#define DOGEN_QUILT_CSHARP_TYPES_FORMATTABLES_HELPER_EXPANDER_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

#include <unordered_set>
#include <unordered_map>
#include "dogen/annotations/types/type_repository.hpp"
#include "dogen/quilt.csharp/types/formatters/repository.hpp"
#include "dogen/quilt.csharp/types/formattables/helper_configuration.hpp"
#include "dogen/quilt.csharp/types/formattables/formattable.hpp"
#include "dogen/quilt.csharp/types/formattables/model.hpp"

namespace dogen {
namespace quilt {
namespace csharp {
namespace formattables {

class helper_expander final {
private:
    struct type_group {
        annotations::type family;
    };

    type_group make_type_group(const annotations::type_repository& atrp) const;

    helper_configuration
    make_configuration(const type_group& tg, const model& fm) const;

public:
    typedef std::unordered_map<std::string, std::unordered_set<std::string>>
    facets_for_family_type;

private:
    facets_for_family_type
        facets_for_family(const formatters::repository& frp) const;

    void populate_helper_properties(
        const helper_configuration& cfg, const formatters::repository& frp,
        std::unordered_map<std::string, formattable>& formattables) const;

public:
    void expand(const annotations::type_repository& atrp,
        const formatters::repository& frp, model& fm) const;

};

} } } }

#endif
