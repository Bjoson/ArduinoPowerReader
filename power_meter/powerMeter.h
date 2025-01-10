/**
 * @file
 * @brief       Handles the monitoring of the power consumption.
 *
 * @author      Björn Johansson
 * @date        2025-01-01
 * @version     1.0
 *
 * @license     MIT
 *   This project is licensed under the MIT License - see the LICENSE file for details.
 *
 * @details
 *
 */

#ifndef POWERMETER_H
#define POWERMETER_H

#include <Arduino.h>
#include "serialPrinter.h"

// Typedef used for the analog sensor state
typedef enum  { LED_STATE_LOW, LED_STATE_HIGH } AnalogState;

class PowerMeter
{
public:
    /**
     * @brief       Constructor for the PowerMeter class.
     */
    PowerMeter(SerialPrinter& serialPrinter);

    /**
     * @brief       Destructor for the PowerMeter class.
     */
    ~PowerMeter();

    /**
     * @brief       Initialize the power meter.
     * @param       analogPin   The analog pin connected to the analog sensor.
     * @param       pulsesPerKwh    The number of pulses per kilowatt-hour.*
     */
    void begin(int analogPin, int pulsesPerKwh);

    /**
     * @brief       Update the power meter.
     */
    void tick();

    /**
     * @brief       Read the current pulse count.
     * @return      The current pulse count.
     */
    unsigned long readPulseCount();

    /**
     * @brief       Read and reset the pulse count.
     * @return      The pulse count.
     */
    unsigned long readAndResetPulseCount();

private:

    /**
     * @brief       Reset the pulse count.
     */
    void resetPulseCount();

    /**
     * @brief       Calculate the next state based on the sensor value.
     *
     * @param       sensorValue     The sensor value.
     *
     * @return      The next state.
     */
    AnalogState calculateNextState(int sensorValue);

    /**
     * @brief       The serial printer used for debugging.
     */
    SerialPrinter& m_serialPrinter;

    /**
     * @brief       The analog pin connected to the analog sensor.
     */
    int m_analogPin = 0;

    /**
     * @brief       The number of pulses per kilowatt-hour.
     */
    int m_pulsesPerKwh = 1000;

    /**
     * @brief       The last time when the power led transitioned from high to low.
     */
    unsigned long m_lastLedHighToLowTimeUs = 0;

    /**
     * @brief       The current stable led state
     */
    AnalogState m_lastStableLedState = LED_STATE_LOW;

    /**
     * @brief       The last start of the debounce time to filter out jitter.
     */
    unsigned long m_lastDebounceTimeUs = 0;

    /**
     * @brief       The current pulse count.
     */
    unsigned long m_currentPulseCount = 0;






};

#endif // POWERMETER_H