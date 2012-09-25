/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * Copyright (C) 2012 Kitanda
 *
 * This file is distributed under the Kitanda Proprietary Software
 * Licence. See doc/LICENCE.TXT for details.
 *
 */
#include <ostream>
#include "dogen/sml/io/qualified_name_io.hpp"

namespace dogen {
namespace sml {

std::ostream& operator<<(std::ostream& stream, const dogen::sml::qualified_name& qualified_name) {
    qualified_name.to_stream(stream);
    return(stream);
}

} }
