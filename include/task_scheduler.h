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
            func_ptr func_;
            smart_delay sdl_;

        public:
            constexpr task_var_t() : func_{nullptr}, sdl_{smart_delay(0, nullptr)} {}

            task_var_t(func_ptr func_to_run, uint32_t interval, time_func time_unit_func)
                    : func_{func_to_run}, sdl_{smart_delay(interval, time_unit_func)} {}

            task_var_t(func_ptr func_to_run, const smart_delay &sd)
                    : func_{func_to_run}, sdl_{sd} {}

            task_var_t(const task_var_t &) = default;

            task_var_t(task_var_t &&) noexcept = default;

            task_var_t &operator=(const task_var_t &other) {
                func_ = other.func_;
                sdl_.set_interval(other.sdl_.target_interval);
                sdl_.set_time_func(other.sdl_.get_time_func());
                return *this;
            };

            void exec(Args ...args) { if ((func_ != nullptr) && sdl_) func_(args...); }

            func_ptr func() { return func_; }
        };
    }

    using task_t = impl::task_var_t<void>;

    template<size_t MaxTasks>
    class task_scheduler {
        static_assert(MaxTasks > 0, "Scheduler size cannot be zero.");

    private:
        size_t size_ = 0;
        task_t tasks_[MaxTasks] = {};
        bool enable_[MaxTasks] = {};

    public:
        /**
         * Add a task to the scheduler.
         *
         * @param task
         * @return
         */
        task_scheduler &add_task(const task_t &task) {
            if (size_ < MaxTasks) {
                enable_[size_] = true;
                tasks_[size_++] = task;
            }
            return *this;
        }

        /**
         * Construct and add a task to the scheduler.
         *
         * @param func
         * @param interval
         * @param time_unit
         * @return
         */
        constexpr task_scheduler &add_task(task_t::func_ptr func, uint32_t interval, smart_delay::time_func time_unit) {
            return add_task(task_t(func, interval, time_unit));
        }

        /**
         * Construct and add a task to the scheduler (from smart_delay).
         *
         * @param func
         * @param sd
         * @return
         */
        constexpr task_scheduler &add_task(task_t::func_ptr func, const smart_delay &sd) {
            return add_task(task_t(func, sd));
        }

        /**
         * Execute each task when each smart delay interval is met.
         */
        void exec() {
            for (size_t i = 0; i < size_; ++i) {
                if (enable_[i]) tasks_[i].exec();
            }
        }

        void enable_unsafe(task_t::func_ptr func) { enable_unsafe(get_index_from_func_ptr(func)); }

        void disable_unsafe(task_t::func_ptr func) { disable_unsafe(get_index_from_func_ptr(func)); }

        void enable_unsafe(size_t i) { enable_[i] = true; }

        void disable_unsafe(size_t i) { enable_[i] = false; }

        /**
         * Enable task based on comparing function pointer if the function is valid.
         *
         * @param func
         */
        void enable(task_t::func_ptr func) { disable(get_index_from_func_ptr(func)); }

        /**
         * Disable task based on comparing function pointer if the function is valid.
         *
         * @param func
         */
        void disable(task_t::func_ptr func) { disable(get_index_from_func_ptr(func)); }

        void enable(size_t i) { if (i < MaxTasks) enable_[i] = true; }

        void disable(size_t i) { if (i < MaxTasks) enable_[i] = true; }

        task_t &get_task(task_t::func_ptr func) { return get_task(get_index_from_func_ptr(func)); }

        const task_t &get_task(task_t::func_ptr func) const { return get_task(get_index_from_func_ptr(func)); }

        task_t &get_task(size_t i) { return tasks_[(i < MaxTasks ? i : 0)]; }

        const task_t &get_task(size_t i) const { return tasks_[(i < MaxTasks ? i : 0)]; }

    private:
        size_t get_index_from_func_ptr(task_t::func_ptr func) {
            for (size_t i = 0; i < size_; ++i) {
                if (func == tasks_[i].func()) return i;
            }
            return MaxTasks;
        }
    };

}

#endif //VT_ARDUINO_TOOLS_TASK_SCHEDULER_H
