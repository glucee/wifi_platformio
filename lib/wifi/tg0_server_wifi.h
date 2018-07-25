#ifndef TG0_WIFI_H
#define TG0_WIFI_H

#if defined (ESP8266)
    #include <ESP8266WiFi.h>
#endif
#if defined (ESP32)
    #include <WiFi.h>
    #include <WiFiClient.h>
    #include <WiFiServer.h>
#endif

#include <Arduino.h>
#include "wifi_config.h"
extern void setup_server(char* ssid, char* password);
extern void check_client();
extern void client_to_serial();
extern void server_to_client(byte* data, int data_size);
extern void run_server();
#endif
