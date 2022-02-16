#include <Arduino.h>
#include "relay.h"
#include "DHT.h"

#define DHTPIN 3 // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22 // DHT 22  (AM2302), AM2321
#define SensorPin A0


DHT dht(DHTPIN, DHTTYPE);
RELAY_INIT(SolenoidValve, 0, False);
RELAY_INIT(Pump, 1, False);

#define BLYNK_TEMPLATE_ID "TMPLRTBGZAnH"
#define BLYNK_DEVICE_NAME "Green House"
#define BLYNK_AUTH_TOKEN "hhWG0nhfODlwwAL23pHrBxcXCX5t2CPx"

// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial
// #define BLYNK_DEBUG        

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "HostNow";
char pass[] = "kde8vf1qws1";

BlynkTimer timer;

float *dhtRead()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t))
  {
    Serial.println(F("Failed to read from DHT sensor!"));
  }
  float hic = dht.computeHeatIndex(t, h, false);
  static float result[3] = {h, t, hic};
  return result;
}

void dhtControll()
{
  float *result = dhtRead();
  Blynk.virtualWrite(V0, result[0]);
  Blynk.virtualWrite(V1, result[1]);
  if (!Pump.inProgres && (result[0] < 50 || result[1] > 30))
  {
    Pump.Control(&Pump, 30 * 60);
  }
}

void SoilMoisture(){
  float sensorValue = analogRead(SensorPin), output;
  output = map(sensorValue, 0, 1023, 0, 255);
  Blynk.virtualWrite(V2, output);
  if (!SolenoidValve.inProgres && ((int) output < 50 || (int) output > 30))
  {
    SolenoidValve.Control(&SolenoidValve, 5 * 60);
  }
}



void setup()
{
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  dht.begin();
  SolenoidValve.begin(&SolenoidValve);
  Pump.begin(&Pump);
  timer.setInterval(1000L, dhtControll);
  timer.setInterval(1000L, SoilMoisture);
}

void loop()
{
  Blynk.run();
  timer.run();
}
