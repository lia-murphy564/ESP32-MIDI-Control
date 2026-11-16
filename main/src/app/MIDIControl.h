#pragma once
#include <Arduino.h>
// #include "MIDIUSB.h" 
#include <mutex> 
#include <vector>
#include "Peripheral.h"
#include "MIDIOut.h"

#define LOCK_GUARD std::lock_guard<std::mutex> lock(mutex());

template <typename T>
class RegistryBase
{
public:
  RegistryBase() {};
  ~RegistryBase() {};

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

class MIDIController : private RegistryBase<Peripheral::PeripheralBase>
{
public:
  // Todo: duplicate checking
  void addControl(Peripheral::PeripheralBase &control)
  {
    return add(&control);
  }

  void removeControl(Peripheral::PeripheralBase &control)
  {
    return remove(&control);
  }

  std::size_t controlCount() const
  {
    return count();
  }

  // PeripheralHandle::PeripheralBase getControl(PeripheralHandle::PeripheralID id) const
  // {
  //   for (PeripheralHandle::PeripheralBase *c : instances())
  //   {
  //     if (c->id() == id) return *c;
  //   }
  //   return nullptr;
  // }

  void setOutput(MIDIOut &midiOut)
  {
    midiOut_ = &midiOut;
  }

  void sendMidi()
  {
    if (!midiOut_) return;
    forEach([this](Peripheral::PeripheralBase &c)
    {
      c.send(*midiOut_);
    });
  }

  void setup()
  {
    forEach([](Peripheral::PeripheralBase &c) { c.setup(); });
  }

  void update()
  {
    forEach([](Peripheral::PeripheralBase &c) { c.update(); });
  }

private:
  MIDIOut *midiOut_ = nullptr;
};



