#include "DHT.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define DHTTYPE DHT22

const int BTUP = 26;
const int BTDOWN = 27;
const int BTOK = 14;
const int BTBACK = 12;
const int DHTPIN = 33;
const int SOILPIN = 35;

int currentMenu = 0;
int totalMenu = 3;
int debounce = 50;
int lastSoil = 0;
float lastT = 0;
float lastH = 0;
bool inSubMenu = false;
bool lastBTUP = HIGH;
bool lastBTDOWN = HIGH;
bool lastBTOK = HIGH;
bool lastBTBACK = HIGH;

unsigned long lastUpdate = 0;
const int interval = 1000;

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  // put your setup code here, to run once:
  dht.begin();
  lcd.init();
  lcd.backlight();
  pinMode(BTUP, INPUT_PULLUP);
  pinMode(BTDOWN, INPUT_PULLUP);
  pinMode(BTOK, INPUT_PULLUP);
  pinMode(BTBACK, INPUT_PULLUP);
  tampilMenu();
}

void loop() {
  bool currentBTUP = digitalRead(BTUP);
  bool currentBTDOWN = digitalRead(BTDOWN);
  bool currentBTOK = digitalRead(BTOK);
  bool currentBTBACK = digitalRead(BTBACK);

  if (lastBTUP == HIGH && currentBTUP == LOW) {
    delay(debounce);
    if (inSubMenu == false) {
      currentMenu--;
      if (currentMenu < 0) currentMenu = totalMenu;
      tampilMenu();
    }
  }

  if (lastBTDOWN == HIGH && currentBTDOWN == LOW) {
    delay(debounce);
    if (inSubMenu == false) {
      currentMenu++;
      if (currentMenu > totalMenu) currentMenu = 0;
      tampilMenu();
    }
  }

  if (lastBTOK == HIGH && currentBTOK == LOW) {
    delay(debounce);
    if (inSubMenu == false) {
      subMenu();
    }
  }

  if (lastBTBACK == HIGH && currentBTBACK == LOW) {
    delay(debounce);
    if (inSubMenu == true) {
      inSubMenu = false;
      tampilMenu();
    }
  }

  lastBTUP = currentBTUP;
  lastBTDOWN = currentBTDOWN;
  lastBTOK = currentBTOK;
  lastBTBACK = currentBTBACK;

  if(inSubMenu) updateDisplay();
}

void tampilMenu() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Pilih Menu:");
  lcd.setCursor(0, 1);

  switch (currentMenu) {
    case 0:
      lcd.print("-> Suhu");
      break;
    case 1:
      lcd.print("-> Kelembapan");
      break;
    case 2:
      lcd.print("-> Soil");
      break;
    case 3:
      lcd.print("-> Setting");
      break;
  }
}

void subMenu() {
  inSubMenu = true;
  lcd.clear();

  switch (currentMenu) {
    case 0:
      lcd.setCursor(0, 0);
      lcd.print("Suhu : ");
      lcd.setCursor(0, 1);
      break;
    case 1:
      lcd.setCursor(0, 0);
      lcd.print("Kelembapan : ");
      lcd.setCursor(0, 1);
      break;
    case 2:
      lcd.setCursor(0, 0);
      lcd.print("Soil : ");
      lcd.setCursor(0, 1);
      break;
    case 3:
      lcd.setCursor(0, 0);
      lcd.print("Seting : ");
      lcd.setCursor(0, 1);
      break;
  }
}

void updateDisplay() {
  if (millis() - lastUpdate >= interval) {
    lastUpdate = millis();

    float t = dht.readTemperature();
    float h = dht.readHumidity();
    int soilADC = analogRead(SOILPIN);
    int soilPercent = map(soilADC, 4095, 0, 0, 100); 

    if (isnan(h) || isnan(t)) {
      lcd.setCursor(0, 1);
      lcd.print("Sensor Error!");
      return;
    }

    switch (currentMenu) {
      case 0:
        if (t != lastT) {
          lcd.setCursor(0, 1);
          lcd.print("        ");
          lcd.setCursor(0, 1);
          lcd.print(t);
          lcd.print(char(223));
          lcd.print("C");
          lastT = t;
        }
        break;
      case 1:
        if (h != lastH) {
          lcd.setCursor(0, 1);
          lcd.print("        ");
          lcd.setCursor(0, 1);
          lcd.print(h);
          lcd.print("%");
          lastH = h;
        }
        break;
      case 2:
        if (soilPercent != lastSoil) {
          lcd.setCursor(0, 1);
          lcd.print("        ");
          lcd.setCursor(0, 1);
          lcd.print(soilPercent);
          lcd.print("%");
          lastSoil = soilPercent;
        }
        break;
    }
  }
}