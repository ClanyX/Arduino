## 🚀 Introduction to Arduino

![Arduino](https://img.shields.io/badge/Arduino-Platform-blue?style=for-the-badge&logo=arduino)

### 📌 What is Arduino?
Arduino is an **open-source** platform for developing electronic projects. It consists of **microcontroller boards** and a development environment (Arduino IDE) that allows easy programming.

---

### 🛠️ Basic Arduino Components
| Component     | Description |
|--------------|--------------------------------|
| Arduino Uno  | The most popular board with ATmega328 |
| LED          | Indicates output signal |
| Resistor     | Limits current |
| Sensors      | Measures temperature, humidity, motion, etc. |
| Motors       | Servo, DC, or stepper motors |

---

### 🔧 Basic Circuit Connection
Example of a simple LED circuit:

```
   +5V  -----> (Arduino)
   GND  -----> (GND on the board)
   PIN 13 -----> (LED + Resistor)
```

---

### 🚀 First Program (Blink)
```cpp
void setup() {
  pinMode(13, OUTPUT);
}

void loop() {
  digitalWrite(13, HIGH);
  delay(1000);
  digitalWrite(13, LOW);
  delay(1000);
}
```

This program makes the LED on **pin 13** blink.

---

### 📜 License
This document is freely available under the **MIT license**.

---

### 👤 Author
**ClanyX**  
🔗 [GitHub](https://github.com/ClanyX)
