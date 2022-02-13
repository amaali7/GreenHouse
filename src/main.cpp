#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include "relay.h"
#include "DHT.h"

#define DHTPIN 2 // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22 // DHT 22  (AM2302), AM2321
#define BLYNK_TEMPLATE_ID "TMPLG5fkysNL"
#define BLYNK_DEVICE_NAME "Quickstart Device"
#define BLYNK_AUTH_TOKEN "MPd8MmCsrjQJpnGGE9exKrnotvWD8DBP"
#define BLYNK_PRINT Serial
#define SensorPin A0

DHT dht(DHTPIN, DHTTYPE);
RELAY_INIT(SolenoidValve, 11, False);
RELAY_INIT(Pump, 12, False);
BlynkTimer timer;

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "";
char pass[] = "";

float *dhtRead()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t))
  {
    Serial.println(F("Failed to read from DHT sensor!"));
    return NULL;
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
  Blynk.virtualWrite(V2, result[3]);
  if (!Pump.inProgres && (result[0] < 50 || result[1] > 30))
  {
    Pump.Control(&Pump, 30 * 60);
  }
}

void SoilMoisture(){
  float sensorValue = analogRead(SensorPin), output;
  output = map(sensorValue, 0, 1023, 0, 255);
  if (!SolenoidValve.inProgres && ((int) output < 50 || (int) output > 30))
  {
    SolenoidValve.Control(&SolenoidValve, 5 * 60);
  }
}


void setup()
{
  Serial.begin(115200);
  dht.begin();
  SolenoidValve.begin(&SolenoidValve);
  Pump.begin(&Pump);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000, dhtControll);
  timer.setInterval(1000, SoilMoisture);
}

void loop()
{
  Blynk.run();
  timer.run();
}
