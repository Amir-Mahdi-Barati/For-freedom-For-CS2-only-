# ESP32 Password-Protected Timer

## Components

| Component            | Quantity |
|---------------------|----------|
| ESP32               | 1        |
| LCD 16x2            | 1        |
| Keypad 3x4          | 1        |
| Relay Module        | 1        |
| Buzzer              | 1        |
| Wiring / Breadboard | As needed|

## Connections

| Device       | ESP32 Pin  |
|--------------|------------|
| LCD RS       | GPIO23     |
| LCD EN       | GPIO22     |
| LCD D4       | GPIO21     |
| LCD D5       | GPIO19     |
| LCD D6       | GPIO18     |
| LCD D7       | GPIO17     |
| LCD VCC      | 3.3V       |
| LCD GND      | GND        |
| Keypad R1    | GPIO13     |
| Keypad R2    | GPIO12     |
| Keypad R3    | GPIO14     |
| Keypad R4    | GPIO27     |
| Keypad C1    | GPIO26     |
| Keypad C2    | GPIO25     |
| Keypad C3    | GPIO33     |
| Relay IN     | GPIO32     |
| Relay VCC    | 3.3V       |
| Relay GND    | GND        |
| Buzzer       | GPIO4      |
| Buzzer GND   | GND        |

> ⚠️ If LCD or Buzzer is 5V, use a **level shifter** for ESP32 GPIO.

---

### Persian Guide
[راهنمای فارسی نوشتن README برای پروژه‌های GitHub](https://anophel.com/fa/articles/how-to-write-a-good-readme-file)
