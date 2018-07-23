#include <SoftwareSerial.h>
#include <SerialESP8266wifi.h>

/* TCP server/client example, that manages client connections, checks for messages
 *  when client is connected and parses commands. Connect to the ESP8266 IP using
 * a TCP client such as telnet, eg: telnet 192.168.0.X 2121
 *  
 *  ESP8266 should be AT firmware based on 1.5 SDK or later
 *
 * 2016 - J.Whittington - engineer.john-whittington.co.uk
 */

#define sw_serial_rx_pin 4 //  Connect this pin to TX on the esp8266
#define sw_serial_tx_pin 6 //  Connect this pin to RX on the esp8266
#define esp8266_reset_pin 5 // Connect this pin to CH_PD on the esp8266, not reset. (let reset be unconnected)

#define SERVER_PORT "8000"
#define SSID "TG0_WIFI"
#define PASSWORD "20150921"

// the last parameter sets the local echo option for the ESP8266 module..
SerialESP8266wifi wifi(Serial, Serial, esp8266_reset_pin, swSerial);

uint8_t wifi_started = false;

void setup_server() {
  // start HW serial for ESP8266 (change baud depending on firmware)
  while (!Serial)
    ;
  Serial.println("Starting wifi");

  wifi.setTransportToTCP();// this is also default
  wifi.endSendWithNewline(false); // Will end all transmissions with a newline and carrage return ie println.. default is true

  wifi_started = wifi.begin();
  if (wifi_started) {
    wifi.connectToAP(SSID, PASSWORD);
    wifi.startLocalServer(SERVER_PORT);
  } else {
    // ESP8266 isn't working..
    Serial.println("Wifi conneced failed");
  }
}

void loop_server() {

  static WifiConnection *connections;

  // check connections if the ESP8266 is there
  if (wifi_started)
    wifi.checkConnections(&connections);

  // check for messages if there is a connection
  for (int i = 0; i < MAX_CONNECTIONS; i++) {
    if (connections[i].connected) {
      // See if there is a message
      WifiMessage msg = wifi.getIncomingMessage();
      // Check message is there
      if (msg.hasData) {
          Serial.println(msg.data);
        // process the command
        //processCommand(msg);
      }
    }
  }
}