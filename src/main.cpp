#include <Arduino.h>
#include "vt_tools"

using namespace vt;

String s;

void setup() {
    pinMode(PC13, OUTPUT);
    Serial.begin(115200);
    s = "Hello";
    s.reserve(100);
    build_string_to(s, "b1", "b2", "b3", "b4", 5, 7, 999.99);
}

void loop() {
    digitalWrite(PC13, !digitalRead(PC13));
    Serial.print(s.length());
    Serial.print(" ");
    Serial.println(s);
    delay(1000);
}
