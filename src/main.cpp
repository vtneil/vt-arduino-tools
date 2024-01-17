#include <Arduino.h>
#include "vt_tools"

using namespace vt;

arduino_iostream cout(Serial);
String s;

task_scheduler<5> scheduler;

in_place_buffer<double, 4> buf;

bool test_val(double x, double, double) {
    return x > 1;
}

void test(void *) {
    asm("");
}

void setup() {
    pinMode(PC13, OUTPUT);
    Serial.begin(115200);
    s = "Hello";
    s.reserve(100);
    build_string_to(s, "b1", "b2", "b3", "b4", 5, 7, 999.99);
    scheduler.add_task(test, nullptr, 1000, millis);

    String x = build_string<512>(1, 2, 3);
}

void loop() {
//    scheduler.exec();
    digitalWrite(PC13, !digitalRead(PC13));
    cout << s.length() << " " << s << endl;
    delay(1000);

    while (!buf.push(4));

    if (buf.any(test_val, 1, 2)) {

    }
}
