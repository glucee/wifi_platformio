#include "tg0_server_wifi.h"

byte COMMANDTEXT[200] = {0};

// Replace these with your WiFi network settings
char* ssid = "ESP8266"; //replace this with your WiFi network name
char* password = "ESP8266Test"; //replace this with your WiFi network password
const int channel = 6;
IPAddress ip_addr, gateway;
IPAddress subnet(255, 255, 255, 0);
WiFiServer tg0_server(DATA_PORT);
WiFiClient* tg0_clients[MAX_CLIENTS] = { NULL };
byte data_to_read[MAX_DATA_SIZE] = {0};
const boolean is_ascii = true;
int* history_time = new int[MAX_CLIENTS];

void setup_server(char* ssid = ssid, char* password = password)
{/* You can remove the password parameter if you want the AP to be open. */
  //clean and remove all the server
  for (int i = 0; i < MAX_CLIENTS; i++) {
    if (tg0_clients[i] != NULL) {
      tg0_clients[i]->stop();
      delete tg0_clients[i];
    }
  }
  WiFi.mode(WIFI_OFF);
  //build a new wifi server
  WiFi.mode(WIFI_AP);
  boolean result = false;
  while(result!=true)
  {
    result = WiFi.softAP(ssid, password, channel);
    Serial.println("Wifi establishing");
    delay(10);
  }
  Serial.println("wifi established!");
  if (ip_addr.fromString(REMOTE_IP_ADDRESS) && gateway.fromString(REMOTE_GATEWAY_ADDRESS)) {
      WiFi.softAPConfig(ip_addr, gateway, subnet);
      Serial.println("host established!");
  }
  tg0_server.begin();
}

void check_client() {
  /* check all clients and accept new clients*/
  //for(int i = 0 ; i < MAX_CLIENTS; i ++)
  {
    WiFiClient client = tg0_server.available();
    if (client) { // add new client
      Serial.println("New Client");
      
      int empty_index = -1;
      int client_index = -1;

      for (int i = 0; i < MAX_CLIENTS; i++) {  
        if (tg0_clients[i] != NULL && tg0_clients[i]->remoteIP() == client.remoteIP()) { //if new client exists before
          client_index = i;
          break;
        }
        else if (tg0_clients[i] == NULL && empty_index == -1) { //also find the first empty index
          empty_index = i;
        }
      }
      if (client_index == -1 && empty_index != -1) // use the empty index
      {
        client_index = empty_index;

        Serial.print("Using Empty Index: ");
        Serial.println(empty_index);
      }
      else if (client_index == -1 && empty_index == -1 ) // use the oldest connected index
      {
        int client_longest_time = -1;
        int start_time = millis();
        for (int i = 0; i < MAX_CLIENTS; i++) {
          if ((start_time - history_time[i]) > client_longest_time ){
              client_longest_time = start_time - history_time[i];
              client_index = i;
          }
        }
      }

      /* replace new client*/
      Serial.println(client_index);
      if(tg0_clients[client_index] != NULL)
      {
        tg0_clients[client_index]->stop();
        delete tg0_clients[client_index];
      }
      tg0_clients[client_index] = new WiFiClient(client);
      tg0_clients[client_index]->setTimeout(WIFI_TIMEOUT);
      tg0_clients[client_index]->setNoDelay(true);
      tg0_clients[client_index]->flush();
    }
  }
}

void client_to_serial() {
  /* flush all the data from client to serial if there is data available for reading */
  
  for (int i = 0; i < MAX_CLIENTS; i++) {
    if (tg0_clients[i] != NULL){
      int bytes_to_read = tg0_clients[i]->available();
      if (bytes_to_read > 0 && bytes_to_read < MAX_DATA_SIZE) { //if there is data available
        history_time[i] = millis(); //save the last read time
        int returned_bytes = tg0_clients[i]->readBytes(data_to_read, bytes_to_read);
        if (returned_bytes > 0) {
          //Serial.println(bytes_to_read);
          //Serial.write(data_to_read, returned_bytes);
        } 
      }
      else if (bytes_to_read > MAX_DATA_SIZE)//if the size is too large
      {
        while(tg0_clients[i]->available()>0)
        {
          tg0_clients[i]->read();
        }
        tg0_clients[i]->flush();
        Serial.println("ERROR DATA LENGTH");
      }
    } 
  }
}

void serial_to_client() {
  int bytes_to_read = Serial.available();
  if (bytes_to_read) {
    Serial.readBytesUntil('\r', COMMANDTEXT, bytes_to_read);
    Serial.read();
    Serial.read();
    if (bytes_to_read >= COMMAND_MIN_LENGTH && bytes_to_read <= COMMAND_MAX_LENGTH) {
      /* do processing of data here */
    }
  }
}

void server_to_client(byte* data, int data_size) {
  for (int i = 0; i < MAX_CLIENTS; i++) {
      if (tg0_clients[i] != NULL) {
        tg0_clients[i]->write(data, data_size);
      }
  }
}