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
 * @file FunWithLEDs.cpp
 * @author Nikolaos Apostolakos <nikoapos@gmail.com>
 */

/*
 * Description
 * -----------
 * 
 * This example controls a row of 5 LED lights, having on 1 LED at each time,
 * moving from left to right and back. A switch is used as input to show or not
 * the light.
 * 
 * Hardware implementation
 * -----------------------
 * Materials:
 *   - A switch
 *   - 5 LEDs (with forward voltage less than 3.3 Volt)
 *   - 5 330 ohm resistors (if you want to properly calculate their value see
 *     here: http://www.evilmadscientist.com/2012/resistors-for-leds)
 * 
 * Connections:
 *   Switch
 *   - Connect the GPIO-21 pin to the one side of the switch
 *   - Connect a 3.3 Volt pin to the other side of the switch
 *   LEDs
 *   - Connect the GPIO-16 pin to the anode of the LED (longer leg)
 *   - Connect the cathode of the LED (shorter leg) to the one side of the
 *     resistor
 *   - Connect the other side of the resistor to the ground (GND)
 *   - Repeat the last three steps for the GPIOs 5, 25, 22 and 4
 *   - Take care that your LEDs are positioned physically at the correct order:
 *         LED:  1  2  3  4  5
 *        GPIO: 16  5 25 22  4
 *   
 * Execution:
 * Run the example. When the switch is OFF all LEDs should be OFF. When you turn
 * on the switch the LEDs should light from left to right and back. Try to play
 * by turning the switch on and off again. The program will stop after 10 seconds.
 */

#include <vector> // for std::vector
#include <chrono> // for std::chrono_literals
#include <thread> // for std::this_thread
#include <RPiHWCtrl/gpio/GpioInput.h>
#include <RPiHWCtrl/gpio/GpioOutput.h>

// We introduce the symbols from std::chrono_literals so we can write time
// like 500ms (500 milliseconds)
using namespace std::chrono_literals;

int main() {
  
  // Create the object we will use to check if the switch is on or off
  RPiHWCtrl::GpioInput on_off {21};
  
  // Create a vector with the controllers for the GPIOs 16, 5, 25, 22 and 4
  std::vector<RPiHWCtrl::GpioOutput> leds {};
  leds.emplace_back(16);
  leds.emplace_back(5);
  leds.emplace_back(25);
  leds.emplace_back(22);
  leds.emplace_back(4);
  
  // The i identifies the index of the current LED in the vector
  unsigned int i = 0;
  
  // The direction of the movement on the LED vector
  int direction = 1;
  
  // Each iteration lasts 100ms, which is a total of 10s
  for (int l = 0; l < 100; ++l) {
    
    // First turn off all the LEDs
    for (auto& led : leds) {
      led.writeValue(false);
    }
    
    // Move i to the next LED in the direction we are moving
    i += direction;
    // If we are out of the vector bounds change the direction and move the i
    // at the opposite direction
    if (i == -1 || i == leds.size()) {
      direction = -1 * direction;
      i += 2 * direction;
    }
    
    // If the switch is on, light the LED at position i, otherwise let it off
    if (on_off.readValue()) {
      leds[i].writeValue(true);
    }
    
    // Sleep for 100 ms
    std::this_thread::sleep_for(100ms);
  }
  
}

