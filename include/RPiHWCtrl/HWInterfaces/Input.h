/*
 * Copyright (C) 2017 Nikolaos Apostolakos <nikoapos@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/* 
 * @file Input.h
 * @author Nikolaos Apostolakos <nikoapos@gmail.com>
 */

#ifndef PIHWCTRLCPP_HWINTERFACES_INPUT_H
#define PIHWCTRLCPP_HWINTERFACES_INPUT_H

namespace PiHWCtrlCpp {

/**
 * @class Input
 * 
 * @brief
 * Interface representing an input
 * 
 * @details
 * Input is anything that provides an arithmetic value. This interface defines
 * a method for accessing this value.
 * 
 * @tparam T
 *    The type of the value. Must be arithmetic.
 */
template <typename T>
class Input {
  static_assert(std::is_arithmetic<T>::value, "The type T of the Input interface must be arithmetic");
  
public:
    
  /// Default destructor
  virtual ~Input() = default;
  
  /// Must be implemented by the subclasses to return the value of the input
  virtual T readValue() = 0;
  
};

} // end of namespace PiHWCtrlCpp

#endif /* PIHWCTRLCPP_HWINTERFACES_INPUT_H */

