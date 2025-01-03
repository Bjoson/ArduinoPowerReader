/**
 * @file
 * @brief       Handles the wifi web server functionality.
 *
 * @author      Björn Johansson
 * @date        2025-01-01
 * @version     1.0
 *
 * @license     MIT
 *   This project is licensed under the MIT License - see the LICENSE file for details.
 *
 * @details
 * The web server listens for incoming HTTP requests on port 80
 * and responds accordingly.
 *
 */
#include "webServer.h"
#include "secrets2.h"

const unsigned long WiFiCheckInterval = 10000; // 10 seconds

#define PULSES_PER_KWH 1000

WebServer::WebServer(SerialPrinter& serialPrinter,
                     GarageDoorController& garageDoorController,
                     PowerMeter& powerMeter)
    : m_serialPrinter(serialPrinter)
    , m_garageDoorController(garageDoorController)
    , m_powerMeter(powerMeter)
    , m_wifiServer(80)
{
    // Nothing to do
}

WebServer::~WebServer()
{
    // Nothing to do
}

void WebServer::begin()
{
    connectToWiFi();
}


void WebServer::connectToWiFi()
{
  WiFi.begin(SECRET_WIFI_SSID, SECRET_WIFI_PASS);

  // block until timeout
  int timeoutSeconds = 30; // seconds
  unsigned long startAttemptTime = millis();

  while (WiFi.status() != WL_CONNECTED &&
         millis() - startAttemptTime < (timeoutSeconds * 1000))
 {
    delay(500);
    m_serialPrinter.print(".");
  }

  // Check final status
  if (WiFi.status() == WL_CONNECTED)
  {
    m_wifiServer.begin();
    m_serialPrinter.println("\nConnected!");
    m_serialPrinter.print("IP address: ");
    String ipAdress = WiFi.localIP().toString();
    m_serialPrinter.println(ipAdress);
  }
  else
  {
    m_serialPrinter.println("\nFailed to connect (timeout).");
  }
}


void WebServer::ensureWiFiConnectedTick()
{
  unsigned long currentMillis = millis();
  if (currentMillis - m_lastWiFiCheckTime  >= WiFiCheckInterval)
  {
    m_lastWiFiCheckTime  = currentMillis;
    if (WiFi.status() != WL_CONNECTED) {
      m_serialPrinter.println("WiFi not connected. Attempting to reconnect...");
      connectToWiFi();
    } else {
        m_serialPrinter.println("WiFi is still connected.");
        String ipAdress = WiFi.localIP().toString();
        m_serialPrinter.println(ipAdress);
    }
  }
}


void WebServer::tick()
{
    ensureWiFiConnectedTick();
    handleRequestsNonBlocking();
}


void WebServer::handleRequestsNonBlocking()
{
  // 2) Handle any incoming HTTP requests
  WiFiClient client = m_wifiServer.available();  // Wait for a client (non-blocking)
  if (client) {
    // Wait until the client sends some data
    while(!client.available()) {
      delay(1);
    }

    // Read the incoming HTTP request
    String req = client.readStringUntil('\r');
    m_serialPrinter.println(req);
    client.flush();

    // Simple request parsing
    // For example: GET /consumption or GET /door

    // A) /consumption
    if (req.indexOf("GET /consumption") >= 0) {
      // Current pulses, or pulses since last read, or total usage
      // Let's say we read the current count (without resetting)
      noInterrupts();
      unsigned long pulsesNow = 12;
      interrupts();

      float kwhNow = (float)pulsesNow / PULSES_PER_KWH;

      // Send response
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/plain");
      client.println("Connection: close");
      client.println();
      client.print("Pulses so far: ");
      client.print(pulsesNow);
      client.print("\nEquivalent kWh: ");
      client.println(kwhNow), 5;
    }
    // B) /door -> toggle or press the door button
    else if (req.indexOf("GET /door") >= 0) {
      m_garageDoorController.pressButton();

      // Send response
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/plain");
      client.println("Connection: close");
      client.println();
      client.println("Door toggled via relay!");
    }
    // C) Everything else
    else {
      // Simple 404
      client.println("HTTP/1.1 404 Not Found");
      client.println("Content-Type: text/plain");
      client.println("Connection: close");
      client.println();
      client.println("Not Found");
    }

    // Close connection
    delay(1);
    client.stop();
  }
}