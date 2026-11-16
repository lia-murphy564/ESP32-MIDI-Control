#pragma once

#include <Arduino.h>
#include "Peripheral.h"

template <size_t steps>
class Sequencer
{
public:
  Sequencer() {};
  ~Sequencer() {};

  void start()
  {}

  void stop()
  {}

private:
  ControlBase::Button button[steps];
};