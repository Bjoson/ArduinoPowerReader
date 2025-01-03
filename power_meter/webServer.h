/**
 * @file
 * @brief       Handles the web server functionality.
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

#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <Arduino.h>
#include <WiFiNINA.h>
#include "garageDoorController.h"
#include "serialPrinter.h"
#include "powerMeter.h"

class WebServer
{
public:
    /**
     * @brief       Constructor for the WebServer class.
     */
    WebServer(SerialPrinter& serialPrinter,
              GarageDoorController& garageDoorController,
              PowerMeter& powerMeter);

    /**
     * @brief       Destructor for the WebServer class.
     */
    ~WebServer();

    /**
     * @brief       Initialize the web server.
     */
    void begin();

    /**
     * @brief       tick the server, e.g. handle incoming requests.
     */
    void tick();

private:

    /**
     * @brief       Try to connect to the WiFi
     */
    void connectToWiFi();

    /**
     * @brief       Ensure that the WiFi is connected.
     */
    void ensureWiFiConnectedTick();

    /**
     * @brief       Handle incoming HTTP requests.
     */
    void handleRequestsNonBlocking();

    /**
     * @brief       The serial printer used for debugging.
     */
    SerialPrinter& m_serialPrinter;

    /**
     * @brief       The WiFi server.
     */
    WiFiServer m_wifiServer; // HTTP server on port 80

    /**
     * @brief       The garage door controller.
     */
    GarageDoorController& m_garageDoorController;

    /**
     * @brief       The power meter.
     */
    PowerMeter& m_powerMeter;

    /**
     * @brief       The last time the WiFi was checked.
     */
    unsigned long m_lastWiFiCheckTime = 0;
};


#endif //  WEBSERVER_H