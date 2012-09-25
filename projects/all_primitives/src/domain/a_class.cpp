/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * Copyright (C) 2012 Kitanda
 *
 * This file is distributed under the Kitanda Proprietary Software
 * Licence. See doc/LICENCE.TXT for details.
 *
 */
#include <boost/io/ios_state.hpp>
#include <ostream>
#include "dogen/all_primitives/domain/a_class.hpp"
#include "dogen/all_primitives/io/versioned_key_io.hpp"
#include "dogen/utility/io/jsonify_io.hpp"

namespace dogen {
namespace all_primitives {

a_class::a_class()
    : bool_property_(static_cast<bool>(0)),
      char_property_(static_cast<char>(0)),
      uchar_property_(static_cast<unsigned char>(0)),
      int_property_(static_cast<int>(0)),
      uint_property_(static_cast<unsigned int>(0)),
      long_property_(static_cast<long>(0)),
      ulong_property_(static_cast<unsigned long>(0)),
      long_long_property_(static_cast<long long>(0)),
      ulong_long_property_(static_cast<unsigned long long>(0)),
      short_property_(static_cast<short>(0)),
      ushort_property_(static_cast<unsigned short>(0)) { }

a_class::a_class(
    bool bool_property,
    char char_property,
    unsigned char uchar_property,
    int int_property,
    unsigned int uint_property,
    long long_property,
    unsigned long ulong_property,
    long long long_long_property,
    unsigned long long ulong_long_property,
    short short_property,
    unsigned short ushort_property,
    dogen::all_primitives::versioned_key versioned_key)
    : bool_property_(bool_property),
      char_property_(char_property),
      uchar_property_(uchar_property),
      int_property_(int_property),
      uint_property_(uint_property),
      long_property_(long_property),
      ulong_property_(ulong_property),
      long_long_property_(long_long_property),
      ulong_long_property_(ulong_long_property),
      short_property_(short_property),
      ushort_property_(ushort_property),
      versioned_key_(versioned_key) { }

void a_class::to_stream(std::ostream& stream) const {
    boost::io::ios_flags_saver ifs(stream);
    stream << std::boolalpha;

    using dogen::utility::streaming::jsonify;

    stream << " {"
           << "\"__type__\": " << "\"a_class\"" << ","
           << "\"bool_property\": " << jsonify(bool_property_) << ","
           << "\"char_property\": " << jsonify(char_property_) << ","
           << "\"uchar_property\": " << jsonify(uchar_property_) << ","
           << "\"int_property\": " << jsonify(int_property_) << ","
           << "\"uint_property\": " << jsonify(uint_property_) << ","
           << "\"long_property\": " << jsonify(long_property_) << ","
           << "\"ulong_property\": " << jsonify(ulong_property_) << ","
           << "\"long_long_property\": " << jsonify(long_long_property_) << ","
           << "\"ulong_long_property\": " << jsonify(ulong_long_property_) << ","
           << "\"short_property\": " << jsonify(short_property_) << ","
           << "\"ushort_property\": " << jsonify(ushort_property_) << ","
           << "\"versioned_key\": " << jsonify(versioned_key_)
           << " }";
}

void a_class::swap(a_class& other) {
    std::swap(bool_property_, other.bool_property_);
    std::swap(char_property_, other.char_property_);
    std::swap(uchar_property_, other.uchar_property_);
    std::swap(int_property_, other.int_property_);
    std::swap(uint_property_, other.uint_property_);
    std::swap(long_property_, other.long_property_);
    std::swap(ulong_property_, other.ulong_property_);
    std::swap(long_long_property_, other.long_long_property_);
    std::swap(ulong_long_property_, other.ulong_long_property_);
    std::swap(short_property_, other.short_property_);
    std::swap(ushort_property_, other.ushort_property_);
    std::swap(versioned_key_, other.versioned_key_);
}

a_class& a_class::operator=(a_class other) {
    std::swap(*this, other);
    return *this;
}

} }