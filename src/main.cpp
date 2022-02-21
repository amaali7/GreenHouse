#include <Arduino.h>
#include "DHTesp.h" // Click here to get the library: http://librarymanager/All#DHTesp

#ifdef ESP32
#pragma message(THIS EXAMPLE IS FOR ESP8266 ONLY !)
#error Select ESP8266 board.
#endif

DHTesp dht;
#define SensorPin A0
#define PUMP 5
#define SV 16

// RELAY_INIT(SolenoidValve, 16, false);
// RELAY_INIT(Pump, 5, false);
bool Pump = false, SolenoidValve = false;
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
  if (!Pump && ((int)temperature > 20 || (int)humidity < 20)) {
    digitalWrite(PUMP, LOW);
    Pump = true;
  } else if (Pump && !((int)temperature > 20 || (int)humidity < 20))
  {
    digitalWrite(PUMP,LOW);
    Pump = false;
  }

  int output_value = analogRead(SensorPin);
  output_value = map(output_value,1024,460,0,100);
  Blynk.virtualWrite(V2, output_value);
  if (!SolenoidValve && ((int)output_value > 20 || (int)output_value < 40)) {
    digitalWrite(SV, HIGH);
    SolenoidValve = true;
  } else if (SolenoidValve && !((int)output_value > 20 || (int)output_value < 20))
  {
    digitalWrite(SV,HIGH);
    SolenoidValve = false;
  }
}

void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  dht.setup(2, DHTesp::DHT11); // Connect DHT sensor to GPIO 17
  pinMode(PUMP, OUTPUT);
  pinMode(SV, OUTPUT);
  timer.setInterval(1000L, dhtControll);
}

void loop() {
  Blynk.run();
  timer.run();
}
