/**
 * @file
 * @brief       Handles the monitoring of the power consumption.
 *
 * @author      Björn Johansson
 * @date        2025-01-02
 * @version     1.0
 *
 * @license     MIT
 *   This project is licensed under the MIT License - see the LICENSE file for details.
 *
 * @details
 *
 */

#include "powerMeter.h"

// The threshold value for the analog sensor it needs to exceed to indicate led on state
static const int RISING_THRESHOLD = 600;

// The threshold value for the analog sensor it needs to fall below to indicate led off state
static const int FALLING_THRESHOLD = 400;

// The debounce delay in microseconds
const unsigned long DEBOUNCE_DELAY_US = 20*1000;

PowerMeter::PowerMeter(SerialPrinter& serialPrinter)
    : m_serialPrinter(serialPrinter)
{

}

PowerMeter::~PowerMeter()
{
    // Nothing to do
}

void PowerMeter::begin(int analogPin, int pulsesPerKwh)
{
    m_analogPin = analogPin;
    m_pulsesPerKwh = pulsesPerKwh;
    pinMode(m_analogPin, INPUT);
}

AnalogState PowerMeter::calculateNextState(int sensorValue)
{
    AnalogState nextLedState = m_lastStableLedState;
    switch(m_lastStableLedState)
    {
        case LED_STATE_LOW:
        {
            if (sensorValue > RISING_THRESHOLD)
            {
                nextLedState = LED_STATE_HIGH;
            }
            break;
        }
        case LED_STATE_HIGH:
        {
            if (sensorValue < FALLING_THRESHOLD)
            {
                nextLedState = LED_STATE_LOW;
            }
            break;
        }
        default:
        {
            // Do nothing
            break;
        }
    }
    return nextLedState;
}

void PowerMeter::tick()
{
    int sensorValue = analogRead(m_analogPin);
    AnalogState candidateLedState = calculateNextState(sensorValue);
    unsigned long currentTimeUs = micros();

    if (candidateLedState != m_lastStableLedState)
    {
        if ((currentTimeUs - m_lastDebounceTimeUs) > DEBOUNCE_DELAY_US)
        {
            m_lastStableLedState = candidateLedState;
        }
    }
    else
    {
        // Reset the debounce timer when reading the same as the last stable value
        m_lastDebounceTimeUs = currentTimeUs;
    }
}

unsigned long PowerMeter::readPulseCount()
{
    return m_pulseCount;
}