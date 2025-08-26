#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL6RPLfT94Q"
#define BLYNK_TEMPLATE_NAME "LATIHAN1"
#define BLYNK_AUTH_TOKEN "wfJx6JOq3pHQi1Vj6hQuSO6pI5vJBM7Q"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "Wire.h"
#include "LiquidCrystal_I2C.h"

char ssid[] = "Ws-Eletronika";
char pass[] = "@bpvppdg25";
const int Soil = 36;
const int LED = 4;
int currentvalue = 0;
unsigned long previousMillis = 0;
const int interval = 200;

LiquidCrystal_I2C lcd(0x27, 16, 2);

byte full[] = { 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F };
byte half[] = { 0x1F, 0x11, 0x11, 0x11, 0x1F, 0x1F, 0x1F, 0x1F };
byte low[] = { 0x1F, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x1F };

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, pass);
  // Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  lcd.init();
  lcd.backlight();
  pinMode(LED, OUTPUT);
}

void loop() {
  unsigned long currentMillis = millis();

  int soilValue = analogRead(Soil);
  soilValue = constrain(soilValue, 982, 2416);
  int soilPercen = map(soilValue, 2416, 982, 0, 100);

  if (soilPercen >= 50) {
    digitalWrite(LED, HIGH);
    Blynk.virtualWrite(V1, 1);
  } else {
    digitalWrite(LED, LOW);
    Blynk.virtualWrite(V1, 0);
  }

  if (currentMillis - previousMillis >= interval) {
    Blynk.virtualWrite(V0, soilPercen);
    previousMillis = currentMillis;

    if (currentvalue != soilPercen) {
      lcd.clear();
      Serial.print("Soil ADC = ");
      Serial.print(soilValue);
      Serial.print(" | Soil = ");
      Serial.print(soilPercen);
      Serial.println(" %");

      lcd.setCursor(0, 0);
      lcd.print("Soil ADC = ");
      lcd.print(soilValue);

      lcd.setCursor(2, 1);
      lcd.print("Soil = ");
      lcd.print(soilPercen);
      lcd.print(" %");

      if (soilPercen <= 35) {
        lcd.createChar(0, low);
        lcd.home();
        lcd.setCursor(0, 1);
        lcd.write(0);
      } else if (soilPercen > 35 && soilPercen <= 65) {
        lcd.createChar(0, half);
        lcd.home();
        lcd.setCursor(0, 1);
        lcd.write(0);
      } else {
        lcd.createChar(0, full);
        lcd.home();
        lcd.setCursor(0, 1);
        lcd.write(0);
      }

      currentvalue = soilPercen;
    }
  }
}
