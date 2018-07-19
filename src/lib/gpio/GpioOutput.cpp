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
 * @file GpioOutput.cpp
 * @author Nikolaos Apostolakos <nikoapos@gmail.com>
 */

#include <fstream>
#include <RPiHWCtrl/gpio/GpioOutput.h>

namespace RPiHWCtrl {

GpioOutput::GpioOutput(int m_gpio_no) : GpioInput(m_gpio_no) {
  // Set the GPIO as output by writing to the direction file
  std::ofstream direction_file {m_gpio_dir + "/direction"};
  direction_file << "out";
}

void GpioOutput::writeValue(const bool& value) {
  std::ofstream out {m_value_file};
  out << value;
}

} // end of namespace RPiHWCtrl
