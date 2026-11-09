#pragma once
#include <Arduino.h>
#include "MIDIUSB.h" 
#include <mutex> 
#include <vector>

#define LOCK_GUARD std::lock_guard<std::mutex> lock(mutex());

template <typename T>
class MIDIController
{
public:
  MIDIController() {};
  ~MIDIController() {};

  static void add(T *obj)
  {
    LOCK_GUARD;
    instances().push_back(obj);
  }

  static void remove(T *obj)
  {
    LOCK_GUARD;
    auto &v = instances();
    v.erase(std::remove(v.begin(), v.end(), obj), v.end());
  }

  // call any function on each element
  template <typename Func>
  static void forEach(Func&& f)
  {
    LOCK_GUARD;
    for (T* obj : instances()) f(*obj);
  }

  static std::size_t count()
  {
    LOCK_GUARD;
    return instances().size();
  }

private:
  static std::vector<T*>& instances() 
  {
    static std::vector<T*> v;
    return v;
  }

  static std::mutex& mutex()
  {
    static std::mutex m;
    return m;
  }
};