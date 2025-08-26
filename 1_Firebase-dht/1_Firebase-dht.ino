#include "DHT.h"
#include <WiFi.h>
#include <Firebase_ESP_Client.h>

#define DHTTYPE DHT11 
#define WIFI_SSID "p"
#define WIFI_PASSWORD "pppppppppp"
#define API_KEY "AIzaSyBFOP544VcMrLKnDQHH8DpZZ-DrO9bMsAw"
#define DATABASE_URL "https://smart-farming-4ba44-default-rtdb.asia-southeast1.firebasedatabase.app"

const int DHTPIN = 4;
const int MQPIN = 34;

unsigned long previousDHTMillis = 0;
unsigned long previousMQMillis = 0;
int DHTInterval = 1000;
int MQInterval = 1000;

float t, h = 0;
int MQADC = 0;
const int SMOKE_THRESHOLD = 2000;

DHT dht(DHTPIN, DHT11);
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

void setup() {
  Serial.begin(9600);
  dht.begin();

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Menghubungkan WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nWiFi Terhubung!");

  // --- Konfigurasi Firebase ---
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  dhtSensor();
  mqSensor();
  sendDataToFirebase();
}

void dhtSensor() {
  if (millis() - previousDHTMillis >= DHTInterval) {
    previousDHTMillis = millis();
    t = dht.readTemperature();
    h = dht.readHumidity();

    if (isnan(h) || isnan(t)) {
      Serial.println("Gagal baca DHT!");
    }

    Serial.print("Suhu: ");
    Serial.print(t);
    Serial.print(" °C | Kelembapan: ");
    Serial.print(h);
    Serial.println(" %");
  }
}

void mqSensor() {
  if (millis() - previousMQMillis >= MQInterval) {
    previousMQMillis = millis();
    MQADC = analogRead(MQPIN);
    Serial.print("Nilai MQ135 : ");
    Serial.println(MQADC);

    if (MQADC > SMOKE_THRESHOLD) {
      Serial.println("Asap Terdeteksi!!");
    }
  }
}

void sendDataToFirebase(t, h, MQADC) {
  if (Firebase.ready()) {
    String path = "/sensor";

    if (Firebase.RTDB.setFloat(&fbdo, path + "/suhu", t)) {
      Serial.println("Suhu terkirim.");
    } else {
      Serial.print("Gagal kirim suhu: ");
      Serial.println(fbdo.errorReason());
    }

    if (Firebase.RTDB.setFloat(&fbdo, path + "/kelembapan", h)) {
      Serial.println("Kelembapan terkirim.");
    } else {
      Serial.print("Gagal kirim kelembapan: ");
      Serial.println(fbdo.errorReason());
    }

    if (Firebase.RTDB.setInt(&fbdo, path + "/mq135", MQADC)) {
      Serial.println("MQ135 terkirim.");
    } else {
      Serial.print("Gagal kirim MQ135: ");
      Serial.println(fbdo.errorReason());
    }

    Serial.println("===== Data Terkirim ke Firebase =====");
    Serial.print("Suhu: ");
    Serial.println(t);
    Serial.print("Kelembapan: ");
    Serial.println(h);
    Serial.print("MQ135: ");
    Serial.println(MQADC);
  }
}
