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
#include "dogen/quilt.csharp/types/initializer.hpp"
#include "dogen/quilt.cpp/types/initializer.hpp"
#include "dogen/yarn.json/types/initializer.hpp"
#include "dogen/yarn.dia/types/initializer.hpp"
#include "dogen/yarn.upsilon/types/initializer.hpp"
#include "dogen/knit/types/initializer.hpp"

namespace dogen {
namespace knit {

void initializer::initialize() {
    dogen::yarn::json::initializer::initialize();
    dogen::yarn::dia::initializer::initialize();
    dogen::yarn::upsilon::initializer::initialize();
    dogen::quilt::csharp::initializer::initialize();
    dogen::quilt::cpp::initializer::initialize();
}

} }
