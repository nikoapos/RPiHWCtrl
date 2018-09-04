/*
 * Copyright (C) 2017 nikoapos
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
 * @file i2c/I2CBus.h
 * @author nikoapos
 */

#ifndef RPIHWCTRL_I2C_I2CBUS_H
#define RPIHWCTRL_I2C_I2CBUS_H

#include <memory>
#include <cstdint>
#include <mutex>
#include <array>
#include <unistd.h> // for read() and write()
#include <RPiHWCtrl/i2c/I2CTransaction.h>
#include <RPiHWCtrl/Interfaces/exceptions.h>

namespace RPiHWCtrl {

class I2CBus {
  
public:
  
  static std::shared_ptr<I2CBus> getSingleton();
  
  virtual ~I2CBus();
  
  I2CTransaction startTransaction(std::uint8_t address);
  
  
  template <std::size_t Size>
  std::array<std::uint8_t, Size> readRegisterAsArray(std::uint8_t register_address) {
    
    // First check that the mutex is locked. If it is not means that we are not in
    // a valid transaction.
    if (m_bus_mutex.try_lock()) {
      m_bus_mutex.unlock();
      throw I2CActionOutOfTransaction();
    }
    
    // Write to the bus the register we want to read
    if (write(m_bus_file, &register_address, 1) != 1) {
      throw I2CReadRegisterException(register_address);
    }
    
    // Read the register in the array
    std::array<std::uint8_t, Size> buffer;
    if (read(m_bus_file, buffer.begin(), Size) != Size) {
      throw I2CReadRegisterException(register_address);
    }
    
    return buffer;
  }
  
  
  template <typename T>
  T readRegister(std::uint8_t register_address, bool invert=false) {
    
    //Read the register as an array
    auto buffer = readRegisterAsArray<sizeof(T)>(register_address);
    
    // Convert the 8-bit unsigned integers to the type to return
    T result {};
    std::uint8_t* result_ptr = reinterpret_cast<std::uint8_t*>(&result);
    for (int i = 0; i < buffer.size(); ++i) {
      if (invert) {
        result_ptr[i] = buffer[i];
      } else {
        result_ptr[i] = buffer[buffer.size() - i - 1];
      }
    }
    
    return result;
  }
  
  
  template <typename T>
  void writeRegister(std::uint8_t register_address, T value, bool invert=false) {
    
    // First check that the mutex is locked. If it is not means that we are not in
    // a valid transaction.
    if (m_bus_mutex.try_lock()) {
      m_bus_mutex.unlock();
      throw I2CActionOutOfTransaction();
    }
    
    // Construct the array to send to the bus
    std::array<std::uint8_t, sizeof(T) + 1> buffer;
    buffer[0] = register_address;
    std::uint8_t* value_ptr = reinterpret_cast<std::uint8_t*>(&value);
    for (int i = 0; i < sizeof(T); ++i) {
      if (invert) {
        buffer[i+1] = *(value_ptr + i);
      } else {
        buffer[i+1] = *(value_ptr + sizeof(T)-i-1);
      }
    }
    
    // Write the message to the bus
    if (write(m_bus_file, buffer.begin(), sizeof(buffer)) != sizeof(buffer)) {
      throw I2CWriteRegisterException<T>(register_address, value);
    }
    
  }
  
private:
  
  I2CBus();
  
  int m_bus_file;
  std::mutex m_bus_mutex;

};

} // end of namespace RPiHWCtrl

#endif /* RPIHWCTRL_I2C_I2CBUS_H */

