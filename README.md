# WiFi Platformio Implementation

WiFi library used in platformio, the library can build a client or a server on ESP8266 or ESP32.

you could easily choose the chip in platformio.ini, for example, the configuration for ESP8266 is:
```
[env:featheresp8266]
platform = espressif8266
framework = arduino
board=huzzah
```

## Getting Started

1. You need to install [platformio](https://platformio.org/) before using the code. However, it is easier to change the code to compatiable with Arduino IDE.

2. In the src/ folder, you need to choose to keep main_client.cpp or main_server.cpp for your device.

3. You can use:
```
make (all): to compile the firmware
make upload: to upload to your device
make clean: clean the project
make update: to update all the libraries
```
3. Enjoy it

## API Functions
Server
```
void setup_server(char* ssid, char* password);
void check_client();
void client_to_serial();
void server_to_client(byte* data, int data_size);
void run_server();
void set_data_serial(bool enable);
void server_status();

```
Client
```
void setup_client_wifi(char* new_ssid, char* new_password);
void client_to_server(byte* data, int data_size);
void check_server();
int read_server(byte* data);
bool server_status();
void set_data_server(bool enable);

```
## Licence

MIT
