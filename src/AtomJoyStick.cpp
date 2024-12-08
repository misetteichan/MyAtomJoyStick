// Copyright (c) 2024 misetteichan
// Licensed under the MIT License. See LICENSE file for details.

#include "AtomJoyStick.h"

namespace {

constexpr int I2C_ADDR = 0x59;
// registers
constexpr uint8_t ADDR_LEFT_STICK_X = 0x00;
constexpr uint8_t ADDR_RIGHT_STICK_X = 0x20;
constexpr uint8_t ADDR_LEFT_BUTTON = 0x70;
constexpr uint8_t ADDR_BATTERY_VOLTAGE = 0x60;

bool read_registers(uint8_t addr, uint8_t buffer[], size_t size, TwoWire& wire) {
  uint16_t data = 0;
  wire.beginTransmission(I2C_ADDR);
  wire.write(addr);
  wire.endTransmission(false);
  const auto n = wire.requestFrom(I2C_ADDR, size);
  if (n != size) {
    return false;
  }
  for (auto i = 0; i < size; ++i) {
    buffer[i] = wire.read();
  }
  return true;
}

uint16_t combine(const uint8_t data[2]) {
  return data[1] * 256 + data[0];
}

uint16_t read_registers(uint8_t addr, TwoWire& wire) {
  uint8_t data[2];
  read_registers(addr, data, 2, wire);
  return combine(data);
}

}

void AtomJoyStick::begin() {
  _wire.begin(38, 39);
  _wire.setClock(400 * 1000);
}

void AtomJoyStick::update() {
  uint8_t buffer[4];
  if (read_registers(ADDR_BATTERY_VOLTAGE, buffer, 4, _wire)) {
    _battery_voltage[0] = combine(buffer);
    _battery_voltage[1] = combine(&buffer[2]);
  }
  if (read_registers(ADDR_LEFT_STICK_X, buffer, 4, _wire)) {
    _left_stick.update(buffer);
  }
  if (read_registers(ADDR_RIGHT_STICK_X, buffer, 4, _wire)) {
    _right_stick.update(buffer);
  }
  if (read_registers(ADDR_LEFT_BUTTON, buffer, 4, _wire)) {
    for (auto i = 0; i < 4; ++i) {
      _button[i].setRawState(::millis(), (buffer[i] & 1) == 0);
    }
  }
}

void AtomJoyStick::AnalogStick::update(uint8_t data[]) {
  _pos[0] = combine(data);
  _pos[1] = combine(&data[2]);
}
