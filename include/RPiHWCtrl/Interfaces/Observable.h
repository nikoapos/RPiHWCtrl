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
 * @file Observable.h
 * @author Nikolaos Apostolakos <nikoapos@gmail.com>
 */

#ifndef RPIHWCTRL_INTERFACES_OBSERVABLE_H
#define RPIHWCTRL_INTERFACES_OBSERVABLE_H

#include <map>
#include <memory>
#include <functional>
#include <RPiHWCtrl/Interfaces/Observer.h>

namespace RPiHWCtrl {

/**
 * @class Observable
 * 
 * @brief Interface which can generate events of type T and notify observers
 * 
 * @details
 * Implementations of this interface should call the notifyObservers() method
 * every time they want to generate an event of type T. The Observable has the
 * logic of keeping and notifying the observers already implemented.
 * 
 * @tparam T
 *    The type of the event
 */
template <typename T>
class Observable {
  
public:
  
  virtual ~Observable() = default;
  
  /**
   * @brief Adds an observer, which will be notified for future events
   * 
   * @details
   * The returned value is the identifier of the observer. It can be used for
   * stop receiving notifications, by using the method removeObserver().
   * 
   * @param observer
   *    The observer to add
   * @return 
   *    The identifier of the observer
   */
  int addObserver(std::shared_ptr<Observer<T>> observer);
  
  /**
   * @brief Adds the given functor as an observer
   * 
   * @details
   * The given functor can be anything that behaves as a function which gets
   * an argument of type T. It will be called for future events. The returned
   * value is the identifier which can be used to stop receiving calls, by using
   * the method removeObserver().
   * 
   * @param observer
   *    A function like object to receive events
   * @return 
   *    The identifier to be used for stop receiving events
   */
  int addObserver(std::function<void(const T&)> observer);
  
  /// Removes the given observer from getting notifications
  void removeObserver(int observer_id);
  
protected:
  
  /// Method to be called by the implementations to generate events of type T
  void notifyObservers(const T& value);
  
private:
  
  int m_next_id = 0;
  std::map<int, std::shared_ptr<Observer<T>>> m_observers {};
  
};

} // end of namespace RPiHWCtrl

#endif /* RPIHWCTRL_INTERFACES_OBSERVABLE_H */

