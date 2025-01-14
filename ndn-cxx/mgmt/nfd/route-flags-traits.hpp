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

#ifndef NDN_CXX_MGMT_NFD_ROUTE_FLAGS_TRAITS_HPP
#define NDN_CXX_MGMT_NFD_ROUTE_FLAGS_TRAITS_HPP

#include "ndn-cxx/encoding/nfd-constants.hpp"

namespace ndn {
namespace nfd {

/**
 * \ingroup management
 * \brief Defines getters for each route inheritance flag.
 *
 * \tparam T class containing a RouteFlags field and implementing
 *           a `RouteFlags getFlags() const` member function
 */
template<typename T>
class RouteFlagsTraits
{
public:
  bool
  isChildInherit() const
  {
    return static_cast<const T*>(this)->getFlags() & ROUTE_FLAG_CHILD_INHERIT;
  }

  bool
  isRibCapture() const
  {
    return static_cast<const T*>(this)->getFlags() & ROUTE_FLAG_CAPTURE;
  }

protected:
  RouteFlagsTraits() = default;
};

} // namespace nfd
} // namespace ndn

#endif // NDN_CXX_MGMT_NFD_ROUTE_FLAGS_TRAITS_HPP
