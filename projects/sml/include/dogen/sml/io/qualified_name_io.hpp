/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * Copyright (C) 2012 Kitanda
 *
 * This file is distributed under the Kitanda Proprietary Software
 * Licence. See doc/LICENCE.TXT for details.
 *
 */
#ifndef DOGEN_SML_IO_QUALIFIED_NAME_IO_HPP
#define DOGEN_SML_IO_QUALIFIED_NAME_IO_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

#include <ostream>
#include "dogen/sml/domain/qualified_name.hpp"

namespace dogen {
namespace sml {

std::ostream& operator<<(std::ostream& stream, const dogen::sml::qualified_name& qualified_name);

} }

#endif
