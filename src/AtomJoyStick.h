// Copyright (c) 2024 misetteichan
// Licensed under the MIT License. See LICENSE file for details.

#include <Wire.h>
#include <utility/Button_Class.hpp>

class AtomJoyStick {
 public:
  AtomJoyStick(TwoWire& wire = Wire1) : _wire(wire) {}

  void begin();
  void update();

  class AnalogStick {
    uint16_t _pos[2] = {};
   public:
    uint16_t X() const { return _pos[0]; }
    uint16_t Y() const { return _pos[1]; }
    void update(uint8_t data[]);
  };

  const AnalogStick& left_stick() const { return _left_stick; }
  const AnalogStick& right_stick() const { return _right_stick; }
  const m5::Button_Class& left_button() const { return _button[0]; }
  const m5::Button_Class& right_button() const { return _button[1]; }
  const m5::Button_Class& left_stick_button() const { return _button[2]; }
  const m5::Button_Class& right_stick_button() const { return _button[3]; }
  const m5::Button_Class& button(int index) const { return _button[index]; }
  float battery_voltage(int slot) const {
    if (slot < 0 || slot >= 2) { return 0.f; }
    return static_cast<float>(_battery_voltage[slot] / 1000.f);
  }
 private: 
  TwoWire& _wire;
  AnalogStick _left_stick{}, _right_stick{};
  m5::Button_Class _button[4] = {};
  uint16_t _battery_voltage[2] = {};
};
