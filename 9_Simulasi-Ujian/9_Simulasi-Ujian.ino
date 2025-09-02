#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL65IC8WRWg"
#define BLYNK_TEMPLATE_NAME "Automatic Irrigation System PBL"
#define BLYNK_AUTH_TOKEN "sWkMk_ucwQsOWvhg1vNd-9kTSEhEdw3a"

#include "DHT.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#define DHTPIN 25
#define SOILPIN 36
#define RELAYPIN 5
#define DHTTYPE DHT11

char ssid[] = "p";
char pass[] = "pppppppppp";

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  pinMode(RELAYPIN, OUTPUT);
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int soilADC = analogRead(SOILPIN);
  int soilPercent = map(soilADC, 4095, 0, 0, 100);
  soilPercent = constrain(soilPercent, 0, 100);

  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  Serial.print("Suhu : ");
  Serial.print(t);
  Serial.print("°C ");
  Serial.print(" | Kelembapan : ");
  Serial.print(h);
  Serial.println("%");
  Serial.print("Soil ADC : ");
  Serial.print(soilADC);
  Serial.print(" |  Soil Percent : ");
  Serial.println(soilPercent);

  Blynk.virtualWrite(V0, t);
  Blynk.virtualWrite(V1, h);
  Blynk.virtualWrite(V2, soilPercent);

  if (soilPercent < 50) {
    Blynk.virtualWrite(V3, "ON");
    digitalWrite(RELAYPIN, LOW);
  } else {
    Blynk.virtualWrite(V3, "OFF");
    digitalWrite(RELAYPIN, HIGH);
  }

  delay(1000);
}
