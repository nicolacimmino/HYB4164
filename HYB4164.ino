// Tester for HYB4164 DRAM Chips.
//  Copyright (C) 2023 Nicola Cimmino
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//   This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see http://www.gnu.org/licenses/.
//
//

#include "src/test_harness.h"
#include "src/monitor.h"

void setup()
{
  Serial.begin(9600);

  setupTestHarness();
  powerUpHYB4164();
  enterMonitor();
}

void loop();
