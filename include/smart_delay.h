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
        /**
         * The constructor
         *
         * @param interval
         * @param func
         */
        smart_delay(uint32_t interval, time_func func)
                : func{func}, interval{interval} { previous_time = func(); }

        /**
         * Default copy constructor
         */
        smart_delay(const smart_delay &) = default;

        /**
         * Disabled move constructor
         */
        smart_delay(smart_delay &&) noexcept = delete;

        /**
         * Conversion to bool returns has_passed().
         *
         * @return
         */
        explicit operator bool() { return has_passed(); }

        /**
         * Conversion to uint32_t returns interval.
         *
         * @return
         */
        explicit operator uint32_t() const { return interval; }

        /**
         * Reset timer's interval.
         */
        void reset() { previous_time = func(); }

        /**
         * Check if the interval has passed.
         *
         * @return
         */
        bool has_passed() {
            uint32_t curr_time = func();
            if (curr_time - previous_time >= interval) {
                previous_time = curr_time;
                return true;
            }
            return false;
        }

        /**
         * Set a new interval.
         *
         * @param new_interval
         */
        void set_interval(uint32_t new_interval) { interval = new_interval; }

        /**
         * Set time function, i.e., millis or micros.
         *
         * @param new_func
         */
        void set_time_func(time_func new_func) { func = new_func; }
    };
}

#endif //VT_ARDUINO_TOOLS_SMART_DELAY_H
