#ifndef VT_ARDUINO_TOOLS_SMART_DELAY_H
#define VT_ARDUINO_TOOLS_SMART_DELAY_H

#include <Arduino.h>

namespace vt {
    /**
     * Smart Delay with Adaptive interval adjustment wrapper class
     */
    class smart_delay {
    public:
        typedef uint32_t (*time_func)();

    private:
        time_func func = nullptr;
        uint32_t target_interval_val = 0;
        uint32_t previous_time = 0;
        uint32_t true_interval_val = target_interval_val;

    public:
        /**
         * The constructor
         *
         * @param interval
         * @param func
         */
        smart_delay(uint32_t interval, time_func func)
                : func{func}, target_interval_val{interval} { if (func != nullptr) previous_time = func(); }

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
        void reset() { if (func != nullptr) previous_time = func(); }

        /**
         * Check if the target_interval_val has passed.
         *
         * @return
         */
        bool has_passed() {
            uint32_t curr_time = 0;
            if (func != nullptr) curr_time = func();
            // Adaptive interval adjustment
            if (curr_time - previous_time >= true_interval_val) {
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
        void set_time_func(time_func new_func) { func = new_func; }

        time_func get_time_func() const { return func; }

        const uint32_t &target_interval = target_interval_val;

        const uint32_t &true_interval = true_interval_val;
    };

    /**
     * Smart Delay without Adaptive interval adjustment wrapper class.
     *
     * For testing or when the smart_delay (adaptive) is not working properly.
     *
     * Works like plain-old smart delay.
     */
    class smart_delay_non_adaptive {
    public:
        typedef uint32_t (*time_func)();

    private:
        time_func func = nullptr;
        uint32_t target_interval_val = 0;
        uint32_t previous_time = 0;
        uint32_t true_interval_val = target_interval_val;

    public:
        smart_delay_non_adaptive(uint32_t interval, time_func func)
                : func{func}, target_interval_val{interval} { if (func != nullptr) previous_time = func(); }

        smart_delay_non_adaptive(const smart_delay_non_adaptive &) = default;

        smart_delay_non_adaptive(smart_delay_non_adaptive &&) noexcept = default;

        explicit operator bool() { return has_passed(); }

        explicit operator uint32_t() const { return target_interval_val; }

        void reset() { previous_time = func(); }

        bool has_passed() {
            uint32_t curr_time = 0;
            if (func != nullptr) curr_time = func();
            uint32_t delta_t = curr_time - previous_time;
            if (delta_t >= target_interval_val) {
                true_interval_val = delta_t;
                previous_time = curr_time;
                return true;
            }
            return false;
        }

        void set_interval(uint32_t new_interval) { target_interval_val = new_interval; }

        void set_time_func(time_func new_func) { func = new_func; }

        time_func get_time_func() { return func; }

        const uint32_t &target_interval = target_interval_val;

        const uint32_t &true_interval = true_interval_val;
    };
}

#endif //VT_ARDUINO_TOOLS_SMART_DELAY_H
