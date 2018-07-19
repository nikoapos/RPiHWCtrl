/*
 * Copyright (C) 2018 Nikolaos Apostolakos <nikoapos@gmail.com>
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
 * @file exceptions.h
 * @author Nikolaos Apostolakos <nikoapos@gmail.com>
 */

#ifndef RPIHWCTRL_INTERFACES_EXCEPTIONS_H
#define RPIHWCTRL_INTERFACES_EXCEPTIONS_H

#include <exception>
#include <sstream>
#include <type_traits>

namespace RPiHWCtrl {

class Exception : public std::exception {
  
public:
  
  const char * what() const noexcept override {
    return m_message.c_str();
  }
  
  template <typename T>
  void appendMessage(const T& message) {
    std::stringstream new_message;
    new_message << m_message << message;
    m_message = new_message.str();
  }
  
private:
  
  std::string m_message {};
  
};

template <typename Ex, typename T, typename=typename std::enable_if<std::is_base_of<Exception, typename std::remove_reference<Ex>::type>::value>::type>
auto operator<<(Ex&& ex, const T& message) -> decltype(std::forward<Ex>(ex)) {
  ex.appendMessage(message);
  return std::forward<Ex>(ex);
}

class GpioException : public Exception {
  
};

class BadGpioNumber : public GpioException {
public:
  BadGpioNumber(int gpio) : gpio(gpio) {
    appendMessage("Bad GPIO number ");
    appendMessage(gpio);
  }
  int gpio;
};

class GpioAlreadyReserved : public GpioException {
public:
  GpioAlreadyReserved(int gpio) : gpio(gpio) {
    appendMessage("GPIO ");
    appendMessage(gpio);
    appendMessage(" already reserved");
  }
  int gpio;
};

} // end of namespace RPiHWCtrls

#endif // RPIHWCTRL_INTERFACES_EXCEPTIONS_H

