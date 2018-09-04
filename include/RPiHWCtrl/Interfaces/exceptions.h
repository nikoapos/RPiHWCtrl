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
#include <cstring>

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

class I2CException : public Exception {
};

class I2CBusOpenFailure : public I2CException {
public:
  I2CBusOpenFailure(std::string bus_name) : bus_name(bus_name), err_code(errno) {
    appendMessage("Failed to open I2C bus " + bus_name + ": ");
    appendMessage(std::strerror(err_code));
  }
  std::string bus_name;
  int err_code;
};

class I2CDeviceConnectionFailure : public I2CException {
public:
  I2CDeviceConnectionFailure(int address) : address(address), err_code(errno) {
    appendMessage("Failed to connect to I2C address ");
    std::stringstream address_str;
    address_str << std::hex << address;
    appendMessage(address_str.str() + ": ");
    appendMessage(std::strerror(err_code));
  }
  int address;
  int err_code;
};

class I2CActionOutOfTransaction : public I2CException {
};

class I2CReadRegisterException : public I2CException {
public:
  I2CReadRegisterException(std::int8_t register_address) 
          : register_address(register_address), err_code(errno) {
    appendMessage("Failed to read register ");
    std::stringstream address_str;
    address_str << std::hex << (int)register_address;
    appendMessage(address_str.str() + ": ");
    appendMessage(std::strerror(err_code));
  }
  std::int8_t register_address;
  int err_code;
};

template <typename T>
class I2CWriteRegisterException : public I2CException {
public:
  I2CWriteRegisterException(std::int8_t register_address, T value)
          : register_address(register_address), err_code(errno), value(value) {
    std::stringstream message;
    message << "Failed to write " << value << " in register "
            << (int)register_address << ": ";
    appendMessage(message.str());
    appendMessage(std::strerror(err_code));
  }
  std::int8_t register_address;
  int err_code;
  T value;
};

class I2CWrongModule : public I2CException {
};

} // end of namespace RPiHWCtrl

#endif // RPIHWCTRL_INTERFACES_EXCEPTIONS_H

