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
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/polymorphic_iarchive.hpp>
#include <boost/archive/polymorphic_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/string.hpp>
#include "dogen/cpp/serialization/content_descriptor_ser.hpp"
#include "dogen/cpp/serialization/entity_ser.hpp"
#include "dogen/cpp/serialization/includes_ser.hpp"
#include "dogen/cpp/serialization/source_file_ser.hpp"
#include "dogen/utility/serialization/path.hpp"


namespace boost {
namespace serialization {

template<typename Archive>
void save(Archive& ar,
    const dogen::cpp::source_file& v,
    const unsigned int /*version*/) {
    ar << make_nvp("documentation", v.documentation_);
    ar << make_nvp("includes", v.includes_);
    ar << make_nvp("descriptor", v.descriptor_);
    ar << make_nvp("header_guard", v.header_guard_);
    ar << make_nvp("file_path", v.file_path_.generic_string());
    ar << make_nvp("relative_path", v.relative_path_.generic_string());
    ar << make_nvp("entity", v.entity_);
}

template<typename Archive>
void load(Archive& ar,
    dogen::cpp::source_file& v,
    const unsigned int /*version*/) {
    ar >> make_nvp("documentation", v.documentation_);
    ar >> make_nvp("includes", v.includes_);
    ar >> make_nvp("descriptor", v.descriptor_);
    ar >> make_nvp("header_guard", v.header_guard_);
    std::string file_path_tmp;
    ar >> make_nvp("file_path", file_path_tmp);
    v.file_path_ = file_path_tmp;
    std::string relative_path_tmp;
    ar >> make_nvp("relative_path", relative_path_tmp);
    v.relative_path_ = relative_path_tmp;
    ar >> make_nvp("entity", v.entity_);
}

} }

namespace boost {
namespace serialization {

template void save(archive::polymorphic_oarchive& ar, const dogen::cpp::source_file& v, unsigned int version);
template void load(archive::polymorphic_iarchive& ar, dogen::cpp::source_file& v, unsigned int version);

template void save(archive::text_oarchive& ar, const dogen::cpp::source_file& v, unsigned int version);
template void load(archive::text_iarchive& ar, dogen::cpp::source_file& v, unsigned int version);

template void save(archive::binary_oarchive& ar, const dogen::cpp::source_file& v, unsigned int version);
template void load(archive::binary_iarchive& ar, dogen::cpp::source_file& v, unsigned int version);

template void save(archive::xml_oarchive& ar, const dogen::cpp::source_file& v, unsigned int version);
template void load(archive::xml_iarchive& ar, dogen::cpp::source_file& v, unsigned int version);

} }