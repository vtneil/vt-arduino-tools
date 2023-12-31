#ifndef VT_ARDUINO_TOOLS_SMART_DELAY_H
#define VT_ARDUINO_TOOLS_SMART_DELAY_H

#include <Arduino.h>

namespace vt {
    /**
     * Smart Delay with Adaptive interval adjustment wrapper class
     */
    class smart_delay {
    public:
        typedef uint32_t (*time_func_t)();

    private:
        time_func_t time_func = nullptr;
        uint32_t target_interval_val = 0;
        uint32_t previous_time = 0;
        uint32_t true_interval_val = target_interval_val;

    public:
        /**
         * The constructor
         *
         * @param interval
         * @param time_func
         */
        smart_delay(uint32_t interval, time_func_t time_func)
                : time_func{time_func}, target_interval_val{interval} {
            if (time_func != nullptr)
                previous_time = time_func();
        }

        /**
         * Default copy constructor
         */
        smart_delay(const smart_delay &) = default;

        /**
         * Disabled move constructor
         */
        smart_delay(smart_delay &&) noexcept = default;

        /**
         * Conversion to bool returns has_passed().
         *
         * @return
         */
        explicit operator bool() { return has_passed(); }

        /**
         * Conversion to uint32_t returns target_interval_val.
         *
         * @return
         */
        explicit operator uint32_t() const { return target_interval_val; }

        /**
         * Reset timer's target_interval_val.
         */
        void reset() { if (time_func != nullptr) previous_time = time_func(); }

        /**
         * Check if the target_interval_val has passed.
         *
         * @return
         */
        bool has_passed() {
            uint32_t curr_time = 0;
            if (time_func != nullptr) curr_time = time_func();
            // Adaptive interval adjustment
            if (curr_time - previous_time >= true_interval_val) {
                // absolute delta_e
                uint32_t delta_e = (target_interval_val > true_interval_val) ?
                                   target_interval_val - true_interval_val :
                                   true_interval_val - target_interval_val;
                if (delta_e < true_interval_val) true_interval_val -= delta_e;
                else true_interval_val += delta_e;
                previous_time = curr_time;
                return true;
            }
            return false;
        }

        /**
         * Set a new target_interval_val.
         *
         * @param new_interval
         */
        void set_interval(uint32_t new_interval) { target_interval_val = new_interval; }

        /**
         * Set time function, i.e., millis or micros.
         *
         * @param new_func
         */
        void set_time_func(time_func_t new_func) { time_func = new_func; }

        time_func_t get_time_func() const { return time_func; }

        const uint32_t &target_interval = target_interval_val;

        const uint32_t &true_interval = true_interval_val;
    };
}

#endif //VT_ARDUINO_TOOLS_SMART_DELAY_H
