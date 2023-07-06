#ifndef VT_ARDUINO_TOOLS_SMART_DELAY_H
#define VT_ARDUINO_TOOLS_SMART_DELAY_H

#include <Arduino.h>

namespace vt {
    /**
     * Smart Delay wrapper class
     */
    class smart_delay {
    private:
        typedef uint32_t (*time_func)();

        time_func func;
        uint32_t interval;
        uint32_t previous_time;

    public:
        explicit smart_delay(uint32_t interval, time_func func)
                : func{func}, interval{interval} { previous_time = func(); }

        explicit operator bool() { return has_passed(); }

        explicit operator uint32_t() const { return interval; }

        bool has_passed() {
            uint32_t curr_time = func();
            if (curr_time - previous_time >= interval) {
                previous_time = curr_time;
                return true;
            }
            return false;
        }

        void set_interval(uint32_t new_interval) { interval = new_interval; }

        void set_time_func(time_func new_func) { func = new_func; }
    };
}

#endif //VT_ARDUINO_TOOLS_SMART_DELAY_H
