#include <Arduino.h>

#include <WiFi.h>
#include "ThingsBoard.h"  // ThingsBoard SDK

#define WIFI_AP "admin"
#define WIFI_PASSWORD "admin123"

// See https://thingsboard.io/docs/getting-started-guides/helloworld/
// to understand how to obtain an access token
#define TOKEN "ThingsBoard_waletQu_Populasi"
#define THINGSBOARD_SERVER "demo.thingsboard.io"

// Baud rate for debug serial
#define SERIAL_DEBUG_BAUD   115200
#define SERIAL_UART_LoRa 9600

const int LED_EXT = 25;

unsigned long previousMillis;
int state_led;

// Initialize ThingsBoard client
WiFiClient espClient;
// Initialize ThingsBoard instance
ThingsBoard tb(espClient);
// the Wifi radio's status
int status = WL_IDLE_STATUS;

//String data;
bool parsing = false;
String sData, data[10];

void InitWiFi()
{
  Serial.println("Connecting to AP ...");
  // attempt to connect to WiFi network

  WiFi.begin(WIFI_AP, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to AP");
}

void reconnect() {
  // Loop until we're reconnected
  status = WiFi.status();
  if ( status != WL_CONNECTED) {
    WiFi.begin(WIFI_AP, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
      if (state_led == LOW) {
        state_led = HIGH;
      }
      else {
        state_led = LOW;
      }
      digitalWrite(LED_EXT, state_led);
    }
    Serial.println("Connected to AP");
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(SERIAL_DEBUG_BAUD);
  Serial2.begin(SERIAL_UART_LoRa, SERIAL_8N1);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_EXT, OUTPUT);

  WiFi.begin(WIFI_AP, WIFI_PASSWORD);
  InitWiFi();
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long currentMillis = millis();

  //delay(1000);

  if (WiFi.status() != WL_CONNECTED) {
    reconnect();
  }

  if (!tb.connected()) {
    // Connect to the ThingsBoard
    Serial.print("Connecting to: ");
    Serial.print(THINGSBOARD_SERVER);
    Serial.print(" with token ");
    Serial.println(TOKEN);
    if (!tb.connect(THINGSBOARD_SERVER, TOKEN)) {
      Serial.println("Failed to connect");
      return;
    }
  }

  while (Serial2.available()) {

      //Serial.write(Serial2.read());
      char inChar = Serial2.read();
      sData += inChar;
      if(inChar == '$') {
        parsing = true;
      }
      if(parsing) {
        int q = 0;
        for (int i = 0; i < sData.length(); i++) {
          if(sData[i] == '#') {
            q++;
            data[q] = "";
          } else {
            data[q] += sData[i];
          }
        }

        Serial.println("DATA MASUK: " + sData);
        Serial.println("Masuk :" + data[1]);
        Serial.println("Keluar :" + data[2]);
        Serial.println("Total :" + data[3]);
        Serial.println("\n");

        parsing = false;
        sData = "";
      }
      // delay(500);
      // if (state_led == LOW) {
      //   state_led = HIGH;
      // } else{
      //   state_led = LOW;
      // }

      digitalWrite(LED_BUILTIN, HIGH);
      //digitalWrite(LED_EXT, HIGH);
  }
  // else {
  //   //Serial.println("None");
  //   digitalWrite(LED_BUILTIN, LOW);
  //   //digitalWrite(LED_EXT, LOW);
  // }
  

  if ((currentMillis - previousMillis) >= 2000) {

     previousMillis = currentMillis;
     Serial.println("Sending data...");

     tb.sendTelemetryString("Masuk", data[1].c_str());
     tb.sendTelemetryString("Keluar", data[2].c_str());
     tb.sendTelemetryString("Total", data[3].c_str());

    if (state_led == LOW) {
      state_led = HIGH;
    } else{
    state_led = LOW;
    digitalWrite(LED_EXT, state_led);
   }

   tb.loop();
  }

  // Uploads new telemetry to ThingsBoard using MQTT.
  // See https://thingsboard.io/docs/reference/mqtt-api/#telemetry-upload-api
  // for more details
  
}

// String getValue(String data1, char separator, int index)
// {
//   int found = 0;
//   int strIndex[] = {0, -1};
//   int maxIndex = data1.length()-1;

//   for (int i=0; i<maxIndex && found<=index; i++) {
//     if (data1.charAt(i) == separator || i == maxIndex) {
//       found++;
//       strIndex[0] = strIndex[1]+1;
//       strIndex[1] = (i == maxIndex) ? i+1 : i;
//     }
//   }
//   return found>index ? data1.substring(strIndex[0], strIndex[1]) : "";
// }