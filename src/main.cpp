#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <ThingsBoard.h>
#include <Config.h>

Adafruit_BME280 bme; 

float Temperature;
float Humidity;
float Pressure;

// Initialize ThingsBoard client
WiFiClient espClient;
// Initialize ThingsBoard instance
ThingsBoard tb(espClient);


void setup() {
  Wire.begin(26, 25);
  Serial.begin(115200);
  delay(100);

  bme.begin(0x76);

  Serial.println("Connecting to ");
  Serial.println(WIFI_SSID);

  //connect to your local wi-fi network
  WiFi.begin(WIFI_SSID, WIFI_PW);

  //check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());




}
void loop() {
  delay(10);
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

  Pressure = bme.readPressure() / 100.0F;
  Temperature = bme.readTemperature();
  Humidity = bme.readHumidity(); 
  Serial.print(" Temperature: ");
  Serial.println(Temperature);
  Serial.print(" Humidity: ");
  Serial.println(Humidity);
  Serial.print(" Pressure: ");
  Serial.println(Pressure);

  if (!isnan(Temperature) ) {
    tb.sendTelemetryFloat("temperature", Temperature);
  }
  if (!isnan(Pressure) ) {
    tb.sendTelemetryFloat("pressure", Pressure);
  }
  if (!isnan(Humidity) ) {
    tb.sendTelemetryFloat("humidity", Humidity);
  }
  tb.loop();
  delay(1000);
}
