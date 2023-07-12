#ifndef VT_ARDUINO_TOOLS_TASK_SCHEDULER_H
#define VT_ARDUINO_TOOLS_TASK_SCHEDULER_H

#include <Arduino.h>
#include "smart_delay.h"

namespace vt {
    namespace impl {
        template<typename ReturnType, typename ...Args>
        class task_var_t {
        public:
            typedef ReturnType (*func_ptr)(Args...);

        protected:
            using time_func = smart_delay::time_func;
            func_ptr func;
            smart_delay sdl;

        public:
            constexpr task_var_t() : func{nullptr}, sdl{smart_delay(0, nullptr)} {}

            task_var_t(func_ptr func_to_run, uint32_t interval, time_func time_unit_func)
                    : func{func_to_run}, sdl{smart_delay(interval, time_unit_func)} {}

            task_var_t(const task_var_t &) = default;

            task_var_t(task_var_t &&) noexcept = default;

            task_var_t &operator=(const task_var_t &other) {
                func = other.func;
                sdl.set_interval(other.sdl.target_interval);
                sdl.set_time_func(other.sdl.get_time_func());
                return *this;
            };

            void exec(Args ...args) { if ((func != nullptr) && sdl) func(args...); }
        };
    }

    using task_t = impl::task_var_t<void>;

    template<size_t MaxTasks>
    class task_scheduler {
    private:
        size_t pv_size = 0;
        task_t pv_tasks[MaxTasks] = {};

    public:
        task_scheduler &add_task(const task_t &task) {
            if (pv_size < MaxTasks) pv_tasks[pv_size++] = task;
            return *this;
        }

        constexpr task_scheduler &add_task(task_t::func_ptr func, uint32_t interval, smart_delay::time_func time_unit) {
            return add_task(task_t(func, interval, time_unit));
        }

        void exec() { for (size_t i = 0; i < pv_size; ++i) pv_tasks[i].exec(); }
    };

}

#endif //VT_ARDUINO_TOOLS_TASK_SCHEDULER_H
