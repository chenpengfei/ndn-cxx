/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2013-2022 Regents of the University of California.
 *
 * This file is part of ndn-cxx library (NDN C++ library with eXperimental eXtensions).
 *
 * ndn-cxx library is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later version.
 *
 * ndn-cxx library is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.
 *
 * You should have received copies of the GNU General Public License and GNU Lesser
 * General Public License along with ndn-cxx, e.g., in COPYING.md file.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * See AUTHORS.md for complete list of ndn-cxx authors and contributors.
 */

#ifndef NDN_CXX_LP_FIELD_HPP
#define NDN_CXX_LP_FIELD_HPP

#include "ndn-cxx/detail/common.hpp"
#include "ndn-cxx/encoding/encoding-buffer.hpp"
#include "ndn-cxx/util/concepts.hpp"

namespace ndn {
namespace lp {

/**
 * \brief Indicates where a field may occur.
 */
namespace field_location_tags {

class Base
{
};

/**
 * \brief A header field.
 */
class Header : public Base
{
};

/**
 * \brief The Fragment field.
 */
class Fragment : public Base
{
};

} // namespace field_location_tags

/**
 * \brief Concept check for fields.
 */
template<class X>
struct Field
{
  static_assert(std::is_base_of<field_location_tags::Base, typename X::FieldLocation>::value, "");
  static_assert(std::is_same<typename X::TlvType::value_type, uint64_t>::value, "");
  static_assert(std::is_same<typename X::IsRepeatable::value_type, bool>::value, "");
  static_assert(std::is_default_constructible<typename X::ValueType>::value, "");
  static_assert(std::is_copy_constructible<typename X::ValueType>::value, "");
  BOOST_CONCEPT_USAGE(Field)
  {
    Block wire;
    X j;
    typename X::ValueType decoded = j.decode(wire);
    EncodingBuffer enc;
    j.encode(enc, decoded);
  }
};

} // namespace lp
} // namespace ndn

#endif // NDN_CXX_LP_FIELD_HPP
