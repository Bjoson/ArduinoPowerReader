/**
 * @file
 * @brief       Controls a relay to open and close a garage door.
 *
 * @author      Björn Johansson
 * @date        2025-01-01
 * @version     1.0
 *
 * @license     MIT
 *   This project is licensed under the MIT License - see the LICENSE file for details.
 *
 * @details
 * The relay is controlled by a digital pin on the Arduino board. The relay is
 * activated for a short period of time to simulate a button press on the
 * garage door opener.
 */
#include "garageDoorController.h"

// How long the relay is activated for, in milliseconds
const unsigned long RELAY_ACTIVATION_TIME_MS = 500;

GarageDoorController::GarageDoorController(SerialPrinter& serialPrinter)
    : m_serialPrinter(serialPrinter)
{
}

GarageDoorController::~GarageDoorController()
{
    // Nothing to do
}

void GarageDoorController::begin(int relayPin)
{
    m_activationTimeMs = 0;
    m_isRelayActive = false;
    m_relayPin = relayPin;
    pinMode(m_relayPin, OUTPUT);
    digitalWrite(m_relayPin, LOW);
}

void GarageDoorController::pressButton()
{
    digitalWrite(m_relayPin, HIGH);
    m_activationTimeMs = millis();
    m_isRelayActive = true;
    m_serialPrinter.println("GarageDoor: Relay activated.");
}

void GarageDoorController::tick()
{
    if (m_isRelayActive)
    {
        unsigned long currentTime = millis();
        if ((currentTime - m_activationTimeMs) > RELAY_ACTIVATION_TIME_MS)
        {
            digitalWrite(m_relayPin, LOW);
            m_isRelayActive = false;
            m_serialPrinter.println("GarageDoor: Relay deactivated.");
        }
    }
}

