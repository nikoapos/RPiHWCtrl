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
 * @file Output.h
 * @author Nikolaos Apostolakos <nikoapos@gmail.com>
 */

#ifndef RPIHWCTRL_INTERFACES_OUTPUT_H
#define RPIHWCTRL_INTERFACES_OUTPUT_H

namespace RPiHWCtrl {

/**
 * @class Output
 * 
 * @brief
 * Interface representing an output
 * 
 * @details
 * Output is anything that can receive an arithmetic values. This interface
 * defines a method for sending this value to the output.
 * 
 * @tparam T
 *    The type of the value. Must be arithmetic.
 */
template <typename T>
class Output {
  static_assert(std::is_arithmetic<T>::value, "The type T of the Input interface must be arithmetic");
  
public:
    
  /// Default destructor
  virtual ~Output() = default;
  
  /// Must be implemented by the subclasses to consume the given value
  virtual void writeValue(const T& value) = 0;
  
};

} // end of namespace RPiHWCtrl

#endif /* RPIHWCTRL_INTERFACES_OUTPUT_H */

