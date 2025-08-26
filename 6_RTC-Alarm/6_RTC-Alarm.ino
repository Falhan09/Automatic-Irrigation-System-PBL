#include "RTClib.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int LED = 23;
int alarmHour = 11;
int alarmMinute = 8;
int blinkLED = 200;

unsigned long prevLCD = 0;
unsigned long lastBlink = 0;
const int intervalLCD = 1000;
const int intervalBlink = 100;
bool ledState = false;

RTC_DS3231 rtc;
LiquidCrystal_I2C lcd(0x27, 16, 2);

char daysOfTheWeek[7][12] = { "Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jumat", "Sabtu" };

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  pinMode(LED, OUTPUT);

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1)
      ;
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

void loop() {
  DateTime now = rtc.now();
  TampilLCD();
  if (now.hour() == alarmHour && now.minute() == alarmMinute) {
    if (millis() - lastBlink >= intervalBlink) {
      lastBlink = millis();
      ledState = !ledState;
      digitalWrite(LED, ledState);
    }
  } else {
    digitalWrite(LED, LOW);
  }
}

void TampilLCD() {
  if (millis() - prevLCD >= intervalLCD) {
    prevLCD = millis();
    Serial.print(now.year());
    Serial.print('/');
    Serial.print(now.month());
    Serial.print('/');
    Serial.print(now.day());
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour());
    Serial.print(':');
    Serial.print(now.minute());
    Serial.print(':');
    Serial.print(now.second());
    Serial.println();

    lcd.setCursor(0, 0);
    lcd.print(now.year());
    lcd.print('/');
    lcd.print(now.month());
    lcd.print('/');
    lcd.print(now.day());
    lcd.setCursor(10, 0);
    lcd.print(daysOfTheWeek[now.dayOfTheWeek()]);
    lcd.setCursor(0, 1);
    lcd.print("          ");
    lcd.setCursor(0, 1);
    lcd.print(now.hour());
    lcd.print(':');
    lcd.print(now.minute());
    lcd.print(':');
    lcd.print(now.second());
  }
}