#include <Servo.h>
#include <Arduino.h>

// Define ESC/Servo pins
const int BAUG = D9;
const int AS = D10;
const int AB = D11;

Servo t1, t2, t3;

void setup() {
  Serial.begin(115200);

  // Attach ESC pins with standard PWM ranges (1200us to 1900us)
  t1.attach(AB, 1300, 1900);
  t2.attach(AS, 1300, 1900);
  t3.attach(BAUG, 1300, 1900);

  // Send neutral signal on boot
  t1.writeMicroseconds(1500);
  t2.writeMicroseconds(1500);
  t3.writeMicroseconds(1500);

  delay(7000);
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();

    // Parse four comma-separated values: T1, T2, T3
    int v1, v2, v3;
    int parsed = sscanf(input.c_str(), "%d,%d,%d", &v1, &v2, &v3);

    if (parsed == 3) {
      // Valid packet received -> write to ESCs
      t1.writeMicroseconds(v1);
      t2.writeMicroseconds(v2);
      t3.writeMicroseconds(v3);
    }
  }
}