#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

int BTUP = 32;
int BTDOWN = 33;
int BTOK = 25;
int BTBACK = 26;

int currentMenu = 0;
int totalMenu = 3;  // 0-3 = 4 menu items
int debounce = 50;
bool inSubMenu = false;

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  pinMode(BTUP, INPUT_PULLUP);
  pinMode(BTDOWN, INPUT_PULLUP);
  pinMode(BTOK, INPUT_PULLUP);
  pinMode(BTBACK, INPUT_PULLUP);

  lcd.setCursor(0, 0);
  lcd.print("System Menu");
  delay(1500);
  tampilMenu();
}

void loop() {
  // Navigasi UP (kurangi menu)
  if (digitalRead(BTUP) == LOW) {
    delay(debounce);
    if (!inSubMenu) {
      currentMenu--;
      if (currentMenu < 0) currentMenu = totalMenu;
      tampilMenu();
    }
    while (digitalRead(BTUP) == LOW);  // Wait for button release
  }

  // Navigasi DOWN (tambah menu)
  if (digitalRead(BTDOWN) == LOW) {
    delay(debounce);
    if (!inSubMenu) {
      currentMenu++;
      if (currentMenu > totalMenu) currentMenu = 0;
      tampilMenu();
    }
    while (digitalRead(BTDOWN) == LOW);  // Wait for button release
  }

  // Konfirmasi OK (masuk ke menu)
  if (digitalRead(BTOK) == LOW) {
    delay(debounce);
    if (!inSubMenu) {
      prosesMenu();
    }
    while (digitalRead(BTOK) == LOW);  // Wait for button release
  }

  // Tombol BACK (kembali ke menu utama)
  if (digitalRead(BTBACK) == LOW) {
    delay(debounce);
    if (inSubMenu) {
      inSubMenu = false;
      tampilMenu();
    }
    while (digitalRead(BTBACK) == LOW);  // Wait for button release
  }

  // Debug serial monitor
  Serial.print("Current Menu: ");
  Serial.print(currentMenu);
  Serial.print(" | UP: ");
  Serial.print(digitalRead(BTUP));
  Serial.print(" | DOWN: ");
  Serial.print(digitalRead(BTDOWN));
  Serial.print(" | OK: ");
  Serial.print(digitalRead(BTOK));
  Serial.print(" | BACK: ");
  Serial.println(digitalRead(BTBACK));

  delay(100);  // Small delay to prevent excessive serial output
}

void tampilMenu() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("=== MAIN MENU ===");
  lcd.setCursor(0, 1);

  switch (currentMenu) {
    case 0:
      lcd.print("-> Status");
      break;
    case 1:
      lcd.print("-> Setting");
      break;
    case 2:
      lcd.print("-> Info");
      break;
    case 3:
      lcd.print("-> Exit");
      break;
  }

  // Tampilkan menu selanjutnya di baris ke-2
  lcd.setCursor(0, 2);
  int nextMenu = (currentMenu + 1) > totalMenu ? 0 : currentMenu + 1;
  switch (nextMenu) {
    case 0:
      lcd.print("   Status");
      break;
    case 1:
      lcd.print("   Setting");
      break;
    case 2:
      lcd.print("   Info");
      break;
    case 3:
      lcd.print("   Exit");
      break;
  }
}

void prosesMenu() {
  inSubMenu = true;
  lcd.clear();
  lcd.setCursor(0, 0);

  switch (currentMenu) {
    case 0:
      lcd.print("=== STATUS ===");
      lcd.setCursor(0, 1);
      lcd.print("System: OK");
      lcd.setCursor(0, 2);
      lcd.print("Temp: 25C");
      lcd.setCursor(0, 3);
      lcd.print("Press BACK to return");
      break;

    case 1:
      lcd.print("=== SETTING ===");
      lcd.setCursor(0, 1);
      lcd.print("Brightness: 80%");
      lcd.setCursor(0, 2);
      lcd.print("Volume: 50%");
      lcd.setCursor(0, 3);
      lcd.print("Press BACK to return");
      break;

    case 2:
      lcd.print("=== INFO ===");
      lcd.setCursor(0, 1);
      lcd.print("Version: 1.0");
      lcd.setCursor(0, 2);
      lcd.print("Author: User");
      lcd.setCursor(0, 3);
      lcd.print("Press BACK to return");
      break;

    case 3:
      lcd.print("=== EXIT ===");
      lcd.setCursor(0, 1);
      lcd.print("Goodbye!");
      delay(2000);
      inSubMenu = false;
      tampilMenu();
      break;
  }
}