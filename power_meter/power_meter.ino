/*
  Project:        Power Meter Server
  Author:         Björn Johansson
  Date Created:   2025-01-01
  Description:    Read power meter pulses and report consumption via WiFi,
                  also control a relay so that the garage door can be controlled.

  Version:        1.0.0
  Board:          MKR WiFi 1010
  License:        This project is licensed under the MIT License.
                  See the LICENSE file for details.

  Additional Notes:
  - Install the WiFiNINA library.
  - See connection diagram for wiring instructions.
  - The WiFi SSID and password are stored in a separate file called secrets.h.
  - The server listens on port 80 for incoming HTTP requests.
    - The server can handle two types of requests:
        1) GET /consumption - Returns the current consumption in kWh.
        2) GET /door - Toggles the garage door relay.
 -
*/

#include <WiFiNINA.h>
#include "serialPrinter.h"
#include "garageDoorController.h"
#include "powerMeter.h"
#include "webServer.h"

//Kamstrup DK-8660 meter spec: 1000 pulses = 1 kWh
const int PULSES_PER_KWH = 1000;
const int POWER_METER_ANALOG_PIN = A0;

// Relay pin for garage door controller
const int GARAGE_DOOR_RELAY_PIN = 5;

//Static module objects
SerialPrinter serialPrinter(Serial);
GarageDoorController garageDoorController(serialPrinter);
PowerMeter powerMeter(serialPrinter);
WebServer webServer(serialPrinter, garageDoorController, powerMeter);

/*!
    @brief  Initialize the project.
*/
void setup()
{
  serialPrinter.begin();

  // Setup the garage door controller
  garageDoorController.begin(GARAGE_DOOR_RELAY_PIN);

  // Setup the power meter
  powerMeter.begin(POWER_METER_ANALOG_PIN, PULSES_PER_KWH);

  // Setup the web server and connect the Wifi
  webServer.begin();

}

/*!
    @brief  Main loop of the project.
*/
void loop()
{
  powerMeter.tick();
  garageDoorController.tick();
  webServer.tick();
}

