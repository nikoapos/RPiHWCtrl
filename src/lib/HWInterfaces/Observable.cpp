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
 * @file Observable.cpp
 * @author Nikolaos Apostolakos <nikoapos@gmail.com>
 */

#include <RPiHWCtrl/HWInterfaces/Observable.h>

namespace RPiHWCtrl {

template<typename T>
int Observable<T>::addObserver(std::shared_ptr<Observer<T> > observer) {
  ++m_next_id;
  m_observers[m_next_id] = observer;
  return m_next_id;
}

namespace {

template<typename T>
class FunctionObserver : public Observer<T> {
  
public:
  
  FunctionObserver(std::function<void(const T&)> m_func) : m_func(m_func) {
  }

  virtual ~FunctionObserver() = default;
  
  void event(const T& value) override {
    m_func(value);
  }

private:
  
  std::function<void(const T&)> m_func;
  
};

}

template<typename T>
int Observable<T>::addObserver(std::function<void(const T&)> observer) {
  return addObserver(std::make_shared<FunctionObserver>(observer));
}

template<typename T>
void Observable<T>::removeObserver(int observer_id) {
  m_observers.erase(observer_id);
}

template<typename T>
void Observable<T>::notifyObservers(const T& value) {
  for (auto& obs : m_observers) {
    obs.second->event(value);
  }
}

} // end of namespace RPiHWCtrl