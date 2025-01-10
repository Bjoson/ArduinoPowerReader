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

// The debounce delay in microseconds, could be increased to 5-10 ms if unstable readings,
// however the webserver requires up to 12 ms to send the values.
const unsigned long DEBOUNCE_DELAY_US = 2*1000;

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
    AnalogState possibleLedState = calculateNextState(sensorValue);
    unsigned long currentTimeUs = micros();

    if (possibleLedState != m_lastStableLedState)
    {
        if ((currentTimeUs - m_lastDebounceTimeUs) > DEBOUNCE_DELAY_US)
        {
            m_lastStableLedState = possibleLedState;
            if (m_lastStableLedState == LED_STATE_HIGH)
            {
                m_currentPulseCount++;
            }
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
    return m_currentPulseCount;
}


void PowerMeter::resetPulseCount()
{
    m_currentPulseCount = 0;
}

unsigned long PowerMeter::readAndResetPulseCount()
{
    unsigned long pulseCount = m_currentPulseCount;
    m_currentPulseCount = 0;
    return pulseCount;
}