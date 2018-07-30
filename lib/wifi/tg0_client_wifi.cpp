#include "tg0_client_wifi.h"

WiFiClient tg0_client;
bool data_to_server = true;
// Replace these with your WiFi network settings
char *server_ssid = "ESP8266"; //replace this with your WiFi network name
char *server_password = "ESP8266Test"; //replace this with your WiFi network password

void setup_client_wifi(char* new_ssid=server_ssid, char* new_password=server_password) {
    WiFi.mode(WIFI_OFF);
    WiFi.mode(WIFI_STA);
    server_ssid = new_ssid;
    server_password = new_password;
    WiFi.begin(server_ssid, server_password);
    while (WiFi.status() != WL_CONNECTED) {
        /* wait */
        delay(3); //5 milliseconds
    }
    while (!tg0_client.connect(REMOTE_IP_ADDRESS, DATA_PORT)) {
        delay(3); //5 milliseconds, I found that reducing from 10 to 3 milliseconds improved the reconnect time;
    }
    tg0_client.setTimeout(WIFI_TIMEOUT);
    tg0_client.setNoDelay(true);
    data_to_server = true;
}

void check_server()
{
    if(!tg0_client.connected())
    {
        int tries = 0;
        while(WiFi.status() == WL_NO_SSID_AVAIL)
        {
            if(tries ++ == 100) break;
            delay(5);
        }
        if (WiFi.status() == WL_CONNECTED) {
             tg0_client.stop();
             while (!tg0_client.connect(REMOTE_IP_ADDRESS, DATA_PORT)) {
                delay(3);
            }
	    tg0_client.setTimeout(WIFI_TIMEOUT);
	    tg0_client.setNoDelay(true);
        }
        else if (WiFi.status() == WL_DISCONNECTED) {
            tg0_client.stop(); //close existing connection with remote server
            WiFi.begin(server_ssid, server_password);
            while (!tg0_client.connect(REMOTE_IP_ADDRESS, DATA_PORT)) {
                delay(3);
                if(WiFi.status() == WL_NO_SSID_AVAIL) break;
            }
            tg0_client.setTimeout(WIFI_TIMEOUT);
	        tg0_client.setNoDelay(true);
        }
    }
}

bool server_status()
{
    return (tg0_client.connected());
}

void client_to_server(byte* data, int data_size) {
    /* check to see if tg0_client is connected, if client
    is not connected, check if WIFI is connected, if WIFI
    is conencted then just disconnect the client and
    connect to server again, else we want to connect to
    WIFI aagain */
    if (tg0_client.connected() && data_to_server) {
        tg0_client.write(data, data_size);
    }
}

int read_server(byte* data) {
    int data_size = 0;
    if (tg0_client.connected()) {
        data_size = tg0_client.available();
        if(data_size > 0)
            data_size = tg0_client.readBytes(data, data_size);
    }
    return data_size;
}

void set_data_server(bool enable)
{
    data_to_server = enable;
}