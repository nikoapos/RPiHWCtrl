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
 * @file GpioOutput.h
 * @author Nikolaos Apostolakos <nikoapos@gmail.com>
 */

#ifndef RPIHWCTRL_GPIO_GPIOOUTPUT_H
#define RPIHWCTRL_GPIO_GPIOOUTPUT_H

#include <RPiHWCtrl/Interfaces/Output.h>
#include <RPiHWCtrl/gpio/GpioInput.h>

namespace RPiHWCtrl {

/**
 * @class GpioOutput
 * 
 * @brief Class for using the GPIO as output
 * 
 * @details
 * This class can be used to control the GPIOs 2-28 of the 40 pin interface of
 * the Raspberry Pi as ON (3.3 Volt) / OFF (GND) switches. The class can
 * also be used as a GpioInput instance, to allow its current state to
 * be retrieved from the code side. For more details see the documentation of
 * the GpioInput class.
 */
class GpioOutput : public GpioInput, public Output<bool> {
  
public:
  
  /**
   * @brief Creates a GpioOutput for the requested pin
   * 
   * @param m_gpio_no
   *    The number of the GPIO to use as the output
   * 
   * @throws GpioAlreadyResearved
   *    If the requested GPIO is already reserved
   * @throws BadGpioNumber
   *    If the given number is out of the range 2-28
   * @throws GpioException
   *    If there was any problem with the communication with the driver
   */
  GpioOutput(int m_gpio_no);

  /// Releases the physical GPIO
  virtual ~GpioOutput() = default;
  
  /// Writes the given value to the GPIO
  void writeValue(const bool& value) override;

};

} // end of namespace RPiHWCtrl

#endif // RPIHWCTRL_GPIO_GPIOOUTPUT_H

