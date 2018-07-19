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
 * @file GpioInput.cpp
 * @author Nikolaos Apostolakos <nikoapos@gmail.com>
 */

#include <poll.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <fstream>
#include <chrono> // for std::chrono_literals
#include <thread> // for std::this_thread
#include <boost/filesystem.hpp>
#include "RPiHWCtrl/Interfaces/exceptions.h"
#include <RPiHWCtrl/gpio/GpioInput.h>

// We introduce the symbols from std::chrono_literals so we can write time
// like 500ms (500 milliseconds)
using namespace std::chrono_literals;

namespace RPiHWCtrl {

namespace {

const std::string gpio_path {"/sys/class/gpio"};

}

GpioInput::GpioInput(int gpio_no) : m_gpio_no(gpio_no) {
  
  // Check that we have a GPIO number in the valid range
  if (m_gpio_no < 2 || m_gpio_no > 28) {
    throw BadGpioNumber(m_gpio_no);
  }
  
  // Construct the GPIO dir path and check that it is not already exported
  m_gpio_dir = gpio_path + "/gpio" + std::to_string(m_gpio_no);
  if (boost::filesystem::exists(m_gpio_dir)) {
    throw GpioAlreadyReserved(gpio_no);
  }
  
  // Export the GPIO by writing its number to the export file
  {
    std::ofstream export_file {gpio_path + "/export"};
    export_file << m_gpio_no;
  }
  
  // Give some time to the driver to initialize everything
  std::this_thread::sleep_for(50ms);
  
  // Check that the GPIO is exported correctly
  if (!boost::filesystem::exists(m_gpio_dir)) {
    throw GpioException() << "Failed to export GPIO " << m_gpio_no;
  }
  
  // Set the GPIO as input by writing to the direction file
  {
    std::ofstream direction_file {m_gpio_dir + "/direction"};
    direction_file << "in";
  }
  
  // Set that both rising and falling edges will generate interrupts, which will
  // make the poll() method to return
  {
    std::ofstream direction_file {m_gpio_dir + "/edge"};
    direction_file << "both";
  }
  
  // Create the value file string
  m_value_file =m_gpio_dir + "/value";
  
}

GpioInput::~GpioInput() {
  
  // Stop the observing thread if it is running
  stop();
  
  // Unexport the GPIO by writing its number to the unexport file
  std::ofstream export_file {gpio_path + "/unexport"};
  export_file << m_gpio_no;
}

bool GpioInput::readValue() {
  std::ifstream in {m_value_file};
  auto value = in.get();
  return (value == '0') ? false : true;
}

bool GpioInput::blockUntilValueChange() {
  pollfd pfd {open(m_value_file.c_str(), O_RDONLY), POLLPRI, 0};
  
  // First read everything so that any pending events are cleared
  int count;
  ioctl(pfd.fd, FIONREAD, &count);
  for (int i = 0; i < count; ++i) {
      char ch;
      read(pfd.fd, &ch, 1);
  }
  
  // Run poll to block until the next event is available
  poll(&pfd, 1, -1);
  
  // Close the file descriptor we used for detecting the event and use the
  // readValue() method to get the latest value
  close(pfd.fd);
  return readValue();
}

namespace {

class ValueChangeEventGenerator {
  
public:
  ValueChangeEventGenerator(const std::string& value_file, std::atomic<bool>& observing_flag,
                            std::function<void(const bool&)> notify_func)
          : m_value_file(value_file), m_observing_flag(observing_flag), m_notify_func(notify_func) {
  }
  
  void operator()() {
    pollfd pfd {open(m_value_file.c_str(), O_RDONLY), POLLPRI, 0};
  
    // First read everything so that any pending events are cleared
    int count;
    ioctl(pfd.fd, FIONREAD, &count);
    for (int i = 0; i < count; ++i) {
        char ch;
        read(pfd.fd, &ch, 1);
    }
    
    // Start the observing loop
    for (;;) {
      
      // Wait for half second for an event
      auto event = poll(&pfd, 1, 500);
      
      // Check if we should terminate the thread. We do this before we notify
      // the observers, as the thread has been canceled before the
      // event happened, while we were waiting.
      if (m_observing_flag.get() == false) {
        break;
      }
      
      // If there was no event continue with the next iteration
      if (event == 0) {
        continue;
      }
      
      // Read the next event
      unsigned char value;
      read(pfd.fd, &value, 1);
      lseek(pfd.fd, 0, SEEK_SET);
      
      // If we do not get 0 or 1 from the file just ignore the event
      if (value == '0') {
        m_notify_func(false);
      } else if (value == '1') {
        m_notify_func(true);
      }
    }
  }

private:
  
  std::string m_value_file;
  std::reference_wrapper<std::atomic<bool>> m_observing_flag;
  std::function<void(const bool&)> m_notify_func;
  
};

}

void GpioInput::start() {
  m_observing_flag = true;
  
  auto notify_func = [this](const bool& value) {
    notifyObservers(value);
  };
  
  ValueChangeEventGenerator task {m_value_file, m_observing_flag, notify_func};
  std::thread t {task};
  t.detach();
}

void GpioInput::stop() {
  // Stop the observing thread by setting the flag to false. We wait for the
  // thread to finish.
  m_observing_flag = false;
  if (m_observing_thread.joinable()) {
    m_observing_thread.join();
  }
}

} // end of namespace RPiHWCtrl