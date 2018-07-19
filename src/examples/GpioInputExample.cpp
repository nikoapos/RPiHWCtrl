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
 * @file GpioInputExample.cpp
 * @author Nikolaos Apostolakos <nikoapos@gmail.com>
 */

/* 
 * Description
 * -----------
 * 
 * Simple example of how to use the GpioInput class. This class can be used to 
 * read binary input from one of the GPIO pins accessible via the 40 pin interface
 * (GPIOs 2-28). It interprets the input as following:
 * 
 * - ON: 3.3 Volt connected to the pin
 * - OFF: GND connected to the pin or the pin is open circuited
 * 
 * The ping state can be retrieved by three ways:
 * 
 * - Directly by using the readValue() method, which returns the current state
 * - Wait for state changes by using the blockUntilValueChange(), which returns
 *      the new state
 * - By adding observers which are notified when state change interrupts are
 *      detected
 * 
 * Hardware implementation
 * -----------------------
 * Materials:
 *   - A switch
 *   - A 10k resistor (used as pull-down)
 * 
 * Connections:
 *   - Connect the GPIO-21 pin to the one side of the switch
 *   - Connect a 3.3V pin to the other side of the switch
 *   - Connect the one side of the resistor to the GPIO-21
 *   - Connect the other side of the resistor to a GND pin
 * 
 * Execution:
 * Run the example. A message will appear indicating if the switch is ON or OFF.
 * Then the program will wait until the switch changes state and it will print
 * the new state. Finally, the program will continue running for 10 seconds.
 * During this time try to turn on and off the switch. You should see messages
 * indicating your actions, produced by an observer of the interrupts.
 */

#include <iostream> // for std::cout
#include <thread> // for std::this_thread
#include <chrono> // for std::chrono_literals
#include <RPiHWCtrl/gpio/GpioInput.h>

// We introduce the symbols from std::chrono_literals so we can write time
// like 500ms (500 milliseconds)
using namespace std::chrono_literals;

int main() {
  
  // Create the object for getting the input from the GPIO21
  RPiHWCtrl::GpioInput input {21};
  
  //
  // Getting the current state of the pin
  //
  
  // For checking if the input in is on or off you can use the readValue()
  // method, which returns a boolean
  if (input.readValue()) {
    std::cout << "GPIO 21 input is ON" << std::endl;
  } else {
    std::cout << "GPIO 21 input is OFF" << std::endl;
  }
  
  //
  // Block until state changes
  //
  
  // If you are interested on changes of the state you can use the
  // blockUntilValueChange() method, which will block until the GPIO pin state
  // changes from ON to OFF or vice versa. The method returns the new state.
  std::cout << "Waiting until the switch changes state..." << std::endl;
  auto new_state = input.blockUntilValueChange();
  std::cout << "The switch changed! It is now " << (new_state ? "ON" : "OFF") << std::endl;
  
  //
  // Listening for events
  //
  
  // The GpioInput class implements the Observable<bool> interface, which means 
  // you can add observers that will be notified for changes of the state of the
  // pin. An observer can be an implementation of the RPiHWCtrl::Observer<bool>
  // interface, or any functor which receives a constant boolean as parameter.
  // In this example a lambda expression is used.
  input.addObserver(
      [](const bool& state) {
        std::cout << "You just turned the switch " << (state ? "ON" : "OFF") << std::endl;
      }
  );
  
  // By default the GpioInput is not listening for state change interrupts. To
  // start listening you have to call the start() method.
  std::cout << "Start listening for switch changes for 10 seconds..." << std::endl;
  input.start();
  
  // Now we can sleep for 10 seconds before allowing the program to exit, so we
  // can turn the switch on and off and see the messages on the screen.
  std::this_thread::sleep_for(10s);
  
}