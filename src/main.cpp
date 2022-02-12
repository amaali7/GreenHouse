#include <Arduino.h>
#include "objects.h"
#include "DHT.h"

#define DHTPIN 2 // Digital pin connected to the DHT sensor
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22 // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);

/*************************************************************

  This is a simple demo of sending and receiving some data.
  Be sure to check out other examples!
 *************************************************************/

// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID "TMPLG5fkysNL"
#define BLYNK_DEVICE_NAME "Quickstart Device"
#define BLYNK_AUTH_TOKEN "MPd8MmCsrjQJpnGGE9exKrnotvWD8DBP"

// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial
#define SensorPin A0
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;

RELAY_INIT(SolidValve, 11, False);
RELAY_INIT(Pump, 12, False);

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "";
char pass[] = "";

BlynkTimer timer;

float *dhtRead()
{
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t))
  {
    Serial.println(F("Failed to read from DHT sensor!"));
    return NULL;
  }
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);
  static float result[3] = {h, t, hic};
  return result;
}

void dhtControll()
{
  float *result = dhtRead();
  // Blynk values
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
  if (!SolidValve.inProgres && ((int) output < 50 || (int) output > 30))
  {
    SolidValve.Control(&Pump, 5 * 60);
  }
}


void setup()
{
  // Debug console
  Serial.begin(115200);
  dht.begin();
  SolidValve.begin(&SolidValve);
  Pump.begin(&Pump);
  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  // Setup a function to be called every second
  timer.setInterval(1000, dhtControll);
  timer.setInterval(1000, SoilMoisture);
}

void loop()
{
  Blynk.run();
  timer.run();
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!
}
