#include "DHTesp.h" // Click here to get the library: http://librarymanager/All#DHTesp
#include "relay.h"
#include <Arduino.h>

#ifdef ESP32
#pragma message(THIS EXAMPLE IS FOR ESP8266 ONLY !)
#error Select ESP8266 board.
#endif

DHTesp dht;
#define SensorPin A0

RELAY_INIT(SolenoidValve, 16, false);
RELAY_INIT(Pump, 5, false);

#define BLYNK_TEMPLATE_ID "TMPLRTBGZAnH"
#define BLYNK_DEVICE_NAME "Green House"
#define BLYNK_AUTH_TOKEN "hhWG0nhfODlwwAL23pHrBxcXCX5t2CPx"

// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial
// #define BLYNK_DEBUG

#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFi.h>

char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "HostNow";
char pass[] = "kde8vf1qws1";

BlynkTimer timer;

void dhtControll() {
  // delay(dht.getMinimumSamplingPeriod());
  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();
  Blynk.virtualWrite(V0, temperature);
  Blynk.virtualWrite(V1, humidity);
  if (!Pump.inProgres && ((int) temperature  > 20 || (int) humidity < 20)) {
    Pump.Control(&Pump, 1 * 60);
  }
}

void SoilMoisture() {
  float sensorValue = analogRead(SensorPin), output;
  output = map(sensorValue, 0, 1023, 0, 255);
  Blynk.virtualWrite(V2, output);
  if (!SolenoidValve.inProgres && ((int)output < 50 || (int)output > 30)) {
    SolenoidValve.Control(&SolenoidValve, 5 * 60);
  }
}

void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  dht.setup(2, DHTesp::DHT11); // Connect DHT sensor to GPIO 17

  SolenoidValve.begin(&SolenoidValve);
  Pump.begin(&Pump);
  timer.setInterval(1000L, dhtControll);
  // timer.setInterval(1000L, SoilMoisture);
}

void loop() {
  Blynk.run();
  timer.run();
}
