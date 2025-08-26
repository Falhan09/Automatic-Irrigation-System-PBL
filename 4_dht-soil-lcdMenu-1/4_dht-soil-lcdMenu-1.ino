#include <DHT.h>
#include "Wire.h"
#include "LiquidCrystal_I2C.h"

const int BtUp = 16;
const int BtDown = 17;
const int BtOk = 4;
const int Btback = 5;
const int soilPin = 15;
const int dhtPin = 23;

float t = 0;
float h = 0;
int menu = 0;
int totalMenu = 3;
int currentSoil = 0;
int soilPercen = 0;
bool inSubMenu = false;
bool firstEnter = true;

DHT dht(dhtPin, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);
  dht.begin();
  lcd.init();
  lcd.backlight();
  pinMode(BtUp, INPUT_PULLUP);
  pinMode(BtDown, INPUT_PULLUP);
  pinMode(BtOk, INPUT_PULLUP);
  pinMode(Btback, INPUT_PULLUP);

  tampilMenu();
}

void loop() {
  int btsBtUp = digitalRead(BtUp);
  int btsBtDown = digitalRead(BtDown);
  int btsBtOk = digitalRead(BtOk);
  int btsBtback = digitalRead(Btback);

  if (!inSubMenu) {
    if (btsBtUp == LOW) {
      menu++;
      if (menu > totalMenu) menu = 0;
      tampilMenu();
      delay(200);
    } else if (btsBtDown == LOW) {
      menu--;
      if (menu < 0) menu = totalMenu;
      tampilMenu();
      delay(200);
    } else if (btsBtOk == LOW) {
      delay(200);
      inSubMenu = true;
      firstEnter = true;
    }
  } else {
    if (menu == 0) {
      lcd.clear();
      soilValue();
      delay(200);
    } else if (menu == 1) {
      if (firstEnter) {
        tampilSet("Setting", 300);
        firstEnter = false;
      }
    } else if (menu == 2) {
      if (firstEnter) {
        lcd.clear();
        lcd.setCursor(0, 1);
        lcd.print("Run Program");
        firstEnter = false;
      }
    }
    if (btsBtback == LOW) {
      delay(200);
      tampilMenu();
      inSubMenu = false;
    }
  }
}

void tampilMenu() {
  lcd.clear();
  if (menu == 0) lcd.print("> Suhu");
  if (menu == 1) lcd.print("> Kelembaban");
  if (menu == 2) lcd.print("> Soil");
  if (menu == 3) lcd.print("> Close");
}

void tampilSet(String label, int nilai) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(label);
  lcd.setCursor(0, 1);
  lcd.print("Nilai: ");
  lcd.print(nilai);
}

void dhtValue() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(t) || isnan(h)) {
    Serial.println("Gagal membaca dht11!");
    return;
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("T : ");
  lcd.print(t, 1);
  lcd.print(char(223));
  lcd.print("C");
}

void soilValue() {
  int raw = analogRead(soilPin);
  raw = constrain(raw, 982, 2416);
  int soilPercen = map(raw, 2416, 982, 0, 100);
  lcd.setCursor(0, 1);
  lcd.print("Soil = ");
  lcd.print(soilPercen);
  lcd.print(" %");
}
