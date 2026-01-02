#include <LiquidCrystal.h>
#include <Keypad.h>

/* ---------------- LCD ---------------- */
LiquidCrystal lcd(23, 22, 21, 19, 18, 17);

/* ---------------- Keypad ---------------- */
const byte ROWS = 4;
const byte COLS = 3;

char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte rowPins[ROWS] = {13, 12, 14, 27};
byte colPins[COLS] = {26, 25, 33};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

/* ---------------- Pins ---------------- */
#define RELAY_PIN   32
#define BUZZER_PIN  4

/* ---------------- Settings ---------------- */
String password = "7355608";

/* ---------------- Variables ---------------- */
bool counting  = false;
bool stopped   = false;
bool alertMode = false;

long totalSeconds = 0;
unsigned long lastTick = 0;
unsigned long tickInterval = 1000;

/* ---------------- Functions ---------------- */

// دریافت عدد با محدودیت
int getLimitedNumber(const char* title, int minV, int maxV) {
  while (true) {
    lcd.clear();
    lcd.print(title);
    lcd.setCursor(0,1);

    String num = "";
    while (true) {
      char k = keypad.getKey();
      if (k >= '0' && k <= '9') {
        num += k;
        lcd.print(k);
      }
      if (k == '#') break;
    }

    int value = num.toInt();
    if (value >= minV && value <= maxV) return value;

    lcd.clear();
    lcd.print("INVALID VALUE");
    delay(1200);
  }
}

// بررسی رمز در حین شمارش (بدون توقف تایمر)
bool checkPasswordWhileRunning() {
  lcd.clear();
  lcd.print("ENTER PASSWORD");
  lcd.setCursor(0,1);

  String input = "";

  while (input.length() < password.length()) {
    char k = keypad.getKey();

    // شمارش ادامه دارد
    if (!stopped && millis() - lastTick >= tickInterval) {
      lastTick = millis();
      totalSeconds--;
    }

    if (k >= '0' && k <= '9') {
      input += k;
      lcd.print('*');
    }

    if (totalSeconds <= 0) return false;
  }

  return (input == password);
}

/* ---------------- Setup ---------------- */
void setup() {
  lcd.begin(16,2);

  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(RELAY_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  lcd.print("TIMER");
  delay(2000);
}

/* ---------------- Loop ---------------- */
void loop() {

  /* ----------- Start ----------- */
  if (!counting && !stopped) {

    alertMode = false;
    tickInterval = 1000;

    int h = getLimitedNumber("Hour (0-23)#", 0, 23);
    int m = getLimitedNumber("Minute(0-59)#", 0, 59);
    int s = getLimitedNumber("Second(0-59)#", 0, 59);

    totalSeconds = h * 3600L + m * 60L + s;
    counting = true;
    lastTick = millis();
  }

  /* ----------- Countdown ----------- */
  if (counting && !stopped) {

    char key = keypad.getKey();

    // اگر * زده شد
    if (key == '*') {
      bool ok = checkPasswordWhileRunning();

      if (ok) {
        // توقف امن
        stopped = true;
        counting = false;

        lcd.clear();
        lcd.print("STOPPED SAFE");
        digitalWrite(BUZZER_PIN, LOW);
        return;
      } else {
        // رمز اشتباه
        alertMode = true;
        tickInterval = 300;
      }
    }

    // تیک تایمر
    if (millis() - lastTick >= tickInterval) {
      lastTick = millis();
      totalSeconds--;

      long h = totalSeconds / 3600;
      long m = (totalSeconds % 3600) / 60;
      long s = totalSeconds % 60;

      lcd.clear();
      lcd.print(alertMode ? "!!! ALERT !!!" : "COUNTDOWN");
      lcd.setCursor(0,1);
      lcd.printf("%02ld:%02ld:%02ld", h, m, s);

      digitalWrite(BUZZER_PIN, HIGH);
      delay(alertMode ? 80 : 30);
      digitalWrite(BUZZER_PIN, LOW);
    }

    // پایان
    if (totalSeconds <= 0) {
      counting = false;
      digitalWrite(RELAY_PIN, HIGH);

      lcd.clear();
      lcd.print("TIME OVER");

      while (true) {
        digitalWrite(BUZZER_PIN, HIGH);
        delay(alertMode ? 150 : 400);
        digitalWrite(BUZZER_PIN, LOW);
        delay(alertMode ? 150 : 400);
      }
    }
  }
}
