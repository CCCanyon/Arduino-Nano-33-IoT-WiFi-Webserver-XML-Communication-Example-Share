/*
2022/09/26 auto reconnect, basic d13 LED buttons
2022/09/27 figure out what is xml. basically xml is just a message that doesn't refresh
there may be better methods that don't send other info, the message only
 
  typedef enum {
  WL_NO_SHIELD = 255,
        WL_NO_MODULE = WL_NO_SHIELD,
        WL_IDLE_STATUS = 0,
        WL_NO_SSID_AVAIL, // 1
        WL_SCAN_COMPLETED, // 2
        WL_CONNECTED, // 3
        WL_CONNECT_FAILED, // 4
        WL_CONNECTION_LOST, // 5
        WL_DISCONNECTED, // 6
        WL_AP_LISTENING, // 7
        WL_AP_CONNECTED, // 8
        WL_AP_FAILED // 9
  } wl_status_t;


*/
#include <SPI.h>
#include <WiFiNINA.h>

#include "arduino_secrets.h"
#include "web_page.h"
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;  // your network SSID (name)
char pass[] = SECRET_PASS;  // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;           // your network key index number (needed only for WEP)

int status = WL_IDLE_STATUS;
WiFiServer server(80);

uint32_t then = 0;
uint32_t dt = 0;

uint32_t display_timer = 0;
const uint32_t display_to = 2000000;  // 2 sec

int server_status = 0;  // disconnected, connected, began
uint32_t server_dc_timer = 0;
uint32_t server_began_timer = 0;
const uint32_t server_dc_to = 5000000;
const uint32_t server_began_delay = 1000000;

uint32_t blink_timer = 0;
const uint32_t blink_to = 1000000;  // 1 sec

const char *web_page = PAGE_MAIN;
int update_page = 0;
char buffer[512];


void setup() {
  Serial.begin(9600);   // initialize serial communication
  pinMode(13, OUTPUT);  // set the LED pin mode
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true)
      ;
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  then = micros();
}


void loop() {
  // time
  dt = micros() - then;
  then += dt;

  WIFIconnect();

  // display
  display_timer += dt;
  if (display_timer > display_to) {
    display_timer %= display_to;  // subtract
    printWifiStatus();
  }

  if (blink_timer < blink_to) {
    blink_timer += dt;
    digitalWrite(13, LOW);  // GET /L turns the LED off
  } else if (server_status == 2) {
    digitalWrite(13, HIGH);
  }

  // begin server
  if (server_status == 2) {
    WiFiClient client = server.available();  // listen for incoming clients

    if (client) {
      // if you get a client,
      Serial.println("new client");
      String currentLine = "";
      while (client.connected()) {
        if (client.available())  // if there's bytes to read from the client,
        {
          char c = client.read();  // read a byte, then
          Serial.write(c);         // print it out the serial monitor
          if (c == '\n')           // if the byte is a newline character
          {
            // end of the client HTTP request, so send a response:
            if (currentLine.length() == 0) {
              switch (update_page) {
                case 1:
                  client.print(HTML_OK);
                  client.println(web_page);
                  update_page = 0;
                  break;
                case 2:
                  client.println(web_page);
                  update_page = 0;
                  break;
                default:
                  break;
              }
              // break out of the while loop:
              break;
            } else {
              currentLine = "";  // if you got a newline, then clear currentLine:
            }
          } else if (c != '\r') {
            currentLine += c;  // add it to the end of the currentLine
          }

          // Check to see if the client request was "GET /H" or "GET /L":
          if (currentLine.endsWith("PUT /led")) {
            blink_timer = 0;
          }
          if (currentLine.endsWith("GET /home")) {
            web_page = PAGE_MAIN;
            update_page = 1;
          } else if (currentLine.endsWith("GET /cal")) {
            web_page = PAGE_CAL;
            update_page = 1;
          } else if (currentLine.endsWith("GET /test")) {
            sprintf(buffer, "%.2f", double(then) / 1000000);
            String mes = XMLparam0;
            mes += buffer;
            mes += XMLparam1;
            mes.toCharArray(buffer, 512, 0);
            web_page = buffer;
            update_page = 2;
            //Serial.print("\nbuffer: "); Serial.println(web_page);
          } else if (currentLine.endsWith("GET /update")) {
            sprintf(buffer, "%u", then/1000000);
            String mes = XMLparam0;
            mes += "<time>";
            mes += buffer;
            mes += "</time>";
            mes += XMLparam1;
            mes.toCharArray(buffer, 512, 0);
            web_page = buffer;
            update_page = 2;
            Serial.print("\nXML:\n"); Serial.println(web_page);
          } else {
            update_page = 1;
          }
        }
      }
      // close the connection:
      client.stop();
      Serial.println("client disconnected");
    }
  }
}

void WIFIconnect() {
  switch (server_status) {
    case 0:
      digitalWrite(13, LOW);
      WiFi.end();
      status = WL_IDLE_STATUS;
      while (status != WL_CONNECTED) {
        Serial.print("Attempting to connect to Network named: ");
        Serial.println(ssid);  // print the network name (SSID);

        // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
        status = WiFi.begin(ssid, pass);
        // wait 10 seconds for connection:
        server_status = 1;
        server_began_timer = 0;
      }
      break;
    case 1:
      if (server_began_timer >= server_began_delay) {
        server.begin();
        server_status = 2;
      } else {
        server_began_timer += dt;
      }
      break;
    case 2:
      if (WL_DISCONNECTED == WiFi.status()) {
        server_status = 0;
      }
      break;
  }
}
void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("WiFi status: ");
  Serial.println(WiFi.status());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}