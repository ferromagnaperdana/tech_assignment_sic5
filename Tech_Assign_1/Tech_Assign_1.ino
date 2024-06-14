//#include <Arduino.h>  //Required if using PlatformIO IDE
#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"

#define DHTPIN 26
#define DHTTYPE DHT11

/*WiFi Credentials*/
const char* ssid = "Ferro";
const char* password = "8U4HRSP2KJ";

//Device IP Address Running Flask
const char* serverName = "http://192.168.45.15:5050/sensor/dht";

WiFiClient client;
HTTPClient http;

DHT dht(DHTPIN, DHTTYPE);

float temperature = 0;
float humidity = 0;

/*Timing Components*/
//HTTP POST Cycle
unsigned long httpLastTime = 0;
unsigned long httpTimer = 5000;


void setup() {
  Serial.begin(115200);

  /*Initialize WiFi Connection*/
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Initializing HTTP Client...");
  Serial.print("HTTP Server -> ");
  Serial.println(serverName);
 
  Serial.println("Siklus pengiriman data HTTP POST setiap 5 detik sekali.");
  Serial.println("");

  /*Initialize DHT Sensor Readings*/
  dht.begin();
}

void loop() {
  //Send an HTTP POST request every 5 seconds
  if ((millis() - httpLastTime) > httpTimer) {
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      // Your Domain name with URL path or IP address with path
      http.begin(client, serverName);

      //Read DHT11 Sensor Value
      float temperature = dht.readTemperature();
      float humidity = dht.readHumidity();
      // Check if any reads failed and exit early (to try again).
      if (isnan(temperature) || isnan(humidity)) {
        Serial.println("Failed to read from DHT sensor!");
        delay(1000);
        return;
      }

      Serial.print("Sensor Readings -> Temperature: ");
      Serial.print(temperature);
      Serial.print(" Humidity: ");
      Serial.println(humidity);

      String data = "temperature=" + String(temperature) + "&humidity=" + String(humidity);
      Serial.print("Data: ");
      Serial.println(data);

      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      int httpResponseCode = http.POST(data);
     
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);

      if (httpResponseCode > 0) {
        if (httpResponseCode == HTTP_CODE_OK) {
          String payload = http.getString();
          Serial.print("Response from server: ");
          Serial.println(payload);
        } else {
          Serial.print("Error sending data: ");
          Serial.println(http.errorToString(httpResponseCode));
        }
      } else {
        Serial.println("Error sending data: No response from server");
        Serial.println("");
      }  
      
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    httpLastTime = millis();
  }
}
