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
#ifndef DOGEN_QUILT_CPP_TYPES_FORMATTABLES_INCLUSION_DEPENDENCIES_BUILDER_FACTORY_HPP
#define DOGEN_QUILT_CPP_TYPES_FORMATTABLES_INCLUSION_DEPENDENCIES_BUILDER_FACTORY_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

#include <string>
#include <unordered_map>
#include <boost/shared_ptr.hpp>
#include "dogen/quilt.cpp/types/formattables/formattable.hpp"
#include "dogen/quilt.cpp/types/formattables/inclusion_dependencies_builder.hpp"
#include "dogen/quilt.cpp/types/formattables/inclusion_directive_group_repository.hpp"

namespace dogen {
namespace quilt {
namespace cpp {
namespace formattables {

/**
 * @brief Creates inclusion dependencies builders.
 *
 * This factory exists to avoid having to supply an inclusion
 * dependencies builder as non-const, and to avoid having some kind of
 * reset method for its state. It is also useful so that we don't have
 * to pass around all of the parameters needed to construct it, we can
 * create it at a high-level and then just pass the factory around.
 *
 * Finally, the factory pre-computes the mapping of the default
 * formatters per facet.
 */
class inclusion_dependencies_builder_factory final {
public:
    inclusion_dependencies_builder_factory(
        const inclusion_directive_group_repository& idgrp,
        const std::unordered_map<std::string, formattable>& formattables);

public:
    /**
     * @brief Create an inclusion dependencies builder.
     */
    inclusion_dependencies_builder make() const;

private:
    const inclusion_directive_group_repository& inclusion_directives_;
    const std::unordered_map<std::string, formattable>& formattables_;
};

} } } }

#endif
