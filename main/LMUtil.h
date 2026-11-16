#pragma once

#include <concepts>

namespace LMUtil
{
  template <typename T>
  constexpr T clamp(T v, T l, T h) {
    return v < l ? l : (h < v ? h : v);
  }

  template <std::integral T>
  constexpr int32_t lerp(T a, T b, T t) {
    return static_cast<T>(a + (((b - a) * t) >> 8));
  }


  constexpr float lerpf(float a, float b, float t) {
    return a + t * (b - a);
  }

  template <typename T>
  constexpr T ramp(T curr, T target, T step) {
      if (curr < target) {
          return (target - curr > step) ? curr + step : target;
      }
      if (curr > target) {
          return (curr - target > step) ? curr - step : target;
      }
      return curr;
  }

  template <typename T>
  constexpr T gamma(T v)
  {
    return (T)v * v * v / (255 * 255);
  }

  // Cubic brightness curve for 8-bit values (0..255 -> 0..255)
  constexpr uint8_t gamma8(uint8_t v) {
    return static_cast<uint8_t>((uint32_t(v) * v * v + 32512) / 65025);  // +half for rounding
  }
}