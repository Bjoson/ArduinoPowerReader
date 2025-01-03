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

#ifndef GARAGEDOORCONTROLLER_H
#define GARAGEDOORCONTROLLER_H

#include "serialPrinter.h"

class GarageDoorController
{
public:
    /**
     * @brief       Constructor for the GarageDoorController class.
     *
     */
    GarageDoorController(SerialPrinter& serialPrinter);

    /**
     * @brief       Destructor for the GarageDoorController class.
     */
    ~GarageDoorController();

    /**
     * @brief       Initialize the garage door controller.
     * @param       relayPin    The digital pin connected to the relay.
     */
    void begin(int relayPin);

    /**
     * @brief       Press the garage door button by activating the relay.
     * @details     This is a non-blocking call. The tick will eventually deactivate the relay.
     */
    void pressButton();

    /**
     * @brief       If the relay is activated, after a certain time, deactivate it.
     */
    void tick();
private:

    /* Keep the default constructor undefined and private
    */
    GarageDoorController();

    /**
     * @brief       The serial printer used for debugging.
     */
    SerialPrinter& m_serialPrinter;

    /**
     * @brief       The time in milliseconds when the relay was last activated.
     */
    unsigned long m_activationTimeMs = 0;

    /**
     *              Flag indicating if the relay is currently active.
     */
    bool m_isRelayActive = false;

    /**
     * @brief       The digital pin connected to the relay.
     */
    int m_relayPin = 0;
};

#endif // GARAGEDOORCONTROLLER_H