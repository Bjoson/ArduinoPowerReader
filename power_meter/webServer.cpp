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
 * The web server listens for incoming socket requests on port 6438
 * and responds accordingly.
 *
 */
#include "webServer.h"
#include "secrets2.h"

const unsigned long WiFiCheckInterval = 10000; // 10 seconds

//The port to listen for incoming requests
const int WiFiPort = 6438;


WebServer::WebServer(SerialPrinter& serialPrinter,
                     GarageDoorController& garageDoorController,
                     PowerMeter& powerMeter)
    : m_serialPrinter(serialPrinter)
    , m_garageDoorController(garageDoorController)
    , m_powerMeter(powerMeter)
    , m_wifiServer(WiFiPort)
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


/**
 * @brief Reads a line (until '\n') from the client with a simple timeout.
 */
String WebServer::readLine(WiFiClient &client)
{
  String line;
  unsigned long start = millis();
  while (millis() - start < 10) {  // 10 ms second timeout
    if (client.available()) {
      char c = client.read();
      if (c == '\n') break;
      line += c;
    }
  }
  return line;
}

void WebServer::handleRequestsNonBlocking()
{
  unsigned long currentMillis = millis();
  bool bHandledRequest = false;
  // Handle any incoming HTTP requests
  WiFiClient client = m_wifiServer.available();  // Wait for a client (non-blocking)
  if (client) {
    // Wait until the client sends some data
    while(!client.available()) {
      //delay(1);
      client.stop();
      return;
    }

    // Read the incoming HTTP request
    String req = readLine(client);
    req.trim(); // Remove leading/trailing spaces

    // P - get and reset the current pulse count
    if (req == "P") {
      unsigned long pulsesNow = m_powerMeter.readAndResetPulseCount();
      // Send response
      client.print(pulsesNow);
      client.print("\n");
      bHandledRequest = true;
    }
    //D -> toggle or press the garage Door button
    else if (req.indexOf("D") >= 0) {
      m_garageDoorController.pressButton();

      // Send response
      client.println("OK");
      bHandledRequest = true;
    }
    else {
      // Uknown command, send -1
      client.println(-1);
      bHandledRequest = true;
    }

    // Close connection
    client.stop();
  }
  if (bHandledRequest)
  {
    unsigned long endTime = millis();
    m_serialPrinter.print("Request handled in ");
    m_serialPrinter.print(endTime - currentMillis);
    m_serialPrinter.println(" ms");
 }
}