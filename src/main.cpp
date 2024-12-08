// Copyright (c) 2024 misetteichan
// Licensed under the MIT License. See LICENSE file for details.

#include <M5Unified.h>
#include "AtomJoyStick.h"

namespace {
AtomJoyStick js;
}

void setup() {
  M5.begin();
  js.begin();

  M5.Display.setTextSize(2.f);
}

void loop() {
  static float volatage[2] = {};
  js.update();
  for (auto i = 0; i < 2; ++i) {
    const auto v = js.battery_voltage(i);
    if (volatage[i] != v) {
      volatage[i] = v;
      M5.Display.setCursor(0, M5.Display.fontHeight() * i);
      M5.Display.printf("%d:%1.2f", i+1, volatage[i]);
    }
  }
  M5.Display.setCursor(0, M5.Display.fontHeight() * 3);
  M5.Display.printf("%04X %04X", js.left_stick().X(), js.left_stick().Y());
  M5.Display.setCursor(0, M5.Display.fontHeight() * 4);
  M5.Display.printf("%04X %04X", js.right_stick().X(), js.right_stick().Y());
  for (auto i = 0; i < 4; ++i) {
    M5.Display.setCursor(M5.Display.fontWidth() * 2 * i, M5.Display.fontHeight() * 5);
    M5.Display.printf("%c", js.button(i).isPressed() ? '*' : ' ');
  }
}
