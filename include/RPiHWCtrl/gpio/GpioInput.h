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
 * @file GpioInput.h
 * @author Nikolaos Apostolakos <nikoapos@gmail.com>
 */

#ifndef RPIHWCTRL_GPIO_GPIOINPUT_H
#define RPIHWCTRL_GPIO_GPIOINPUT_H

#include <string>
#include <atomic>
#include <thread>
#include <memory>
#include <RPiHWCtrl/Interfaces/Input.h>
#include <RPiHWCtrl/Interfaces/Observable.h>

namespace RPiHWCtrl {

/**
 * @class GpioInput
 * 
 * @brief Class for using the GPIO as input
 * 
 * @details
 * This class can be used to access the GPIOs 2-28 of the Rasberry Pi, via the
 * 40 pin interface. The input is interpreted as following:
 * 
 * - ON: 3.3 Volt connected to the pin
 * - OFF: GND connected to the pin or the pin is open circuited
 * 
 * The GPIO status can be accessed with one of the following ways:
 * 
 * - readValue(): Returns the current status of the GPIO
 * - blockUntilValueChange(): Blocks until the status of the GPIO is changed and
 *      returns the new status
 * - Via interrupts: By adding observers using the addObserver() and then
 *      calling the start() method.
 * 
 * Internally the class uses the linux driver via sysfs.
 */
class GpioInput : public Input<bool>, public Observable<bool> {
  
public:

  /**
   * @brief Creates a GpioInput for the requested pin
   * 
   * @param m_gpio_no
   *    The number of the GPIO to use as the input
   * 
   * @throws GpioAlreadyResearved
   *    If the requested GPIO is already reserved
   * @throws BadGpioNumber
   *    If the given number is out of the range 2-28
   * @throws GpioException
   *    If there was any problem with the communication with the driver
   */
  GpioInput(int m_gpio_no);
  
  GpioInput(const GpioInput&) = delete;
  GpioInput& operator=(const GpioInput&) = delete;
  GpioInput(GpioInput&&) = default;
  GpioInput& operator=(GpioInput&&) = default;

  /// Releases the physical GPIO
  virtual ~GpioInput();

  /// Returns true if the input is ON (as described at the class documentation)
  /// and false otherwise
  bool readValue() override;

  /// Blocks until the input changes and returns the new status
  bool blockUntilValueChange();

  /// Start listening for value changes interrupts and notify the observers
  void start();

  /// Stop listening for interrupts
  void stop();
  
protected:
  
  std::string m_gpio_dir;
  std::string m_value_file;
  
private:
  
  int m_gpio_no;
  std::unique_ptr<std::atomic<bool>> m_observing_flag = std::make_unique<std::atomic<bool>>(false);
  std::unique_ptr<std::thread> m_observing_thread {};
  
};

} // end of namespace RPiHWCtrl

#endif // RPIHWCTRL_GPIO_GPIOINPUT_H

