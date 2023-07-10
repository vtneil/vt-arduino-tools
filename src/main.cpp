#include <Arduino.h>
#include "vt_tools"

using namespace vt;

smart_delay sd1(1000, millis);  // smart delay 1,000 ms ( 1  s )
smart_delay sd2(2000, micros);  // smart delay 2,000 us ( 2 ms )
task_scheduler<3> scheduler;

void setup() {
    scheduler.add_task([]() -> void { Serial.println("Func 1"); }, 250, millis);
    scheduler.add_task([]() -> void { Serial.println("Func 2"); }, 500, millis);
    scheduler.add_task([]() -> void { Serial.println("Func 3"); }, 1000, millis);
    Serial.begin(115200);
}

void loop() {
    scheduler.exec();

    if (sd1) {  // or sd1.has_passed()
        Serial.println("Timer 1 OK!");
    }

    if (sd2) {
        Serial.println("Timer 2 OK!");
    }
}