#ifndef TG0_CLIENT_WIFI_H
#define TG0_CLIENT_WIFI_H
#if defined (ESP8266)
    #include <ESP8266WiFi.h>
#endif
#if defined (ESP32)
    #include <ESP32.h>
#endif
#include "wifi_config.h"


extern WiFiClient tg0_client;
extern void setup_client_wifi(char* new_ssid, char* new_password);
extern void client_to_server(byte* data, int data_size);
extern void check_server();
#endif