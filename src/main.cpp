#include <Arduino.h>
#include "vt_tools"

using namespace vt;

arduino_iostream cout(Serial);
String s;

//task_scheduler<5> scheduler;
//task_t task1(setup, 1000, millis);

void setup() {
    pinMode(PC13, OUTPUT);
    Serial.begin(115200);
    s = "Hello";
    s.reserve(100);
    build_string_to(s, "b1", "b2", "b3", "b4", 5, 7, 999.99);
//    scheduler.add_task(setup, 1000, millis);
//    scheduler.add_task(task1);
}

void loop() {
//    scheduler.exec();
    digitalWrite(PC13, !digitalRead(PC13));
    cout << s.length() << " " << s << endl;
    delay(1000);
}
