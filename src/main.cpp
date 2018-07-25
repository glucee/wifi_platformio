#include "tg0_server_wifi.h"
#include "tg0_client_wifi.h"
#define BAUD_RATE 115200
const double samplingFreq = 100.0;
double timePerLoop = (1000./samplingFreq);
byte random_data[14] = {0};

void setup() {
  Serial.begin(BAUD_RATE);
  while(!Serial);
  /* make sure the password is at least 8 characters, 
  and the username has no weird characters */
  setup_client_wifi("GWIFI", "WIFIPASS");
}

int data = 0;

void run_client() {
  random_data[0] = 'R';
  for (int i = 1; i < 14; i++) {
    random_data[i] = data;
  }
  data ++;
  random_data[13] = 0xff;
  random_data[12] = 0xff;
  check_server();
  client_to_server(random_data, 14);
}

void run_server() {
  check_client();
  client_to_serial();
}

void loop() {
  long before = millis();
  run_client();
  long elapsed = millis() - before;
  int sleep = timePerLoop - elapsed;
  if (sleep < 0) {
  } else {
    delay(sleep);
  }
}