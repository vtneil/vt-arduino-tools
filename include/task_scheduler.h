#ifndef VT_ARDUINO_TOOLS_TASK_SCHEDULER_H
#define VT_ARDUINO_TOOLS_TASK_SCHEDULER_H

#include <Arduino.h>
#include "smart_delay.h"

namespace vt {
    class task_t {
    public:
        typedef void (*func_ptr_t)(void *);

    protected:
        using time_func = smart_delay::time_func_t;

        func_ptr_t func_;
        void *arg_;
        smart_delay sdl_;
        uint8_t priority_;

    public:
        task_t() : func_{nullptr}, arg_{nullptr}, sdl_{smart_delay(0, nullptr)}, priority_(0) {}

        task_t(func_ptr_t func_to_run, void *arg, uint32_t interval, time_func time_unit_func, uint8_t priority = 0)
                : func_{func_to_run}, arg_{arg}, sdl_{smart_delay(interval, time_unit_func)}, priority_(priority) {}

        task_t(func_ptr_t func_to_run, void *arg, const smart_delay &sd, uint8_t priority = 0)
                : func_{func_to_run}, arg_{arg}, sdl_{sd}, priority_(priority) {}

        task_t(const task_t &) = default;

        task_t(task_t &&) noexcept = default;

        task_t &operator=(const task_t &other) {
            func_ = other.func_;
            arg_ = other.arg_;
            sdl_.set_interval(other.sdl_.target_interval);
            sdl_.set_time_func(other.sdl_.get_time_func());
            priority_ = other.priority_;
            return *this;
        };

        void exec() { if ((func_ != nullptr) && sdl_) func_(arg_); }

        void reset() { sdl_.reset(); }

        func_ptr_t func() { return func_; }

        constexpr const uint8_t priority() const { return priority_; }
    };

    template<size_t MaxTasks>
    class task_scheduler {
        static_assert(MaxTasks > 0, "Scheduler size cannot be zero.");

    private:
        size_t size_ = 0;
        task_t tasks_[MaxTasks] = {};

    public:
        task_scheduler &add_task(const task_t &task, const bool condition = true) {
            if (condition && size_ < MaxTasks) {
                size_t i;
                for (i = 0; i < size_ && tasks_[i].priority() <= task.priority(); ++i) static_cast<void>(0);
                for (size_t j = 0; j < size_ - i; ++j) tasks_[size_ - j] = tasks_[size_ - j - 1];
                tasks_[i] = task;
                ++size_;
                return *this;
            }
        }

        constexpr task_scheduler &add_task(task_t::func_ptr_t func,
                                           void *arg_ptr,
                                           uint32_t interval,
                                           smart_delay::time_func_t time_unit,
                                           uint8_t priority = 0,
                                           const bool condition = true) {
            return add_task(task_t(func, arg_ptr, interval, time_unit, priority), condition);
        }

        constexpr task_scheduler &add_task(task_t::func_ptr_t func,
                                           void *arg_ptr,
                                           const smart_delay &sd,
                                           uint8_t priority = 0,
                                           const bool condition = true) {
            return add_task(task_t(func, arg_ptr, sd, priority), condition);
        }

        void exec() {
            for (size_t i = 0; i < size_; ++i)
                tasks_[i].exec();
        }

        void clear() { size_ = 0; }

        void reset() {
            for (size_t i = 0; i < size_; ++i)
                tasks_[i].reset();
        }

        task_t &get_task(task_t::func_ptr_t func) { return get_task(get_index_from_func_ptr(func)); }

        const task_t &get_task(task_t::func_ptr_t func) const { return get_task(get_index_from_func_ptr(func)); }

        task_t &get_task(size_t i) { return tasks_[(i < MaxTasks ? i : 0)]; }

        const task_t &get_task(size_t i) const { return tasks_[(i < MaxTasks ? i : 0)]; }

    private:
        size_t get_index_from_func_ptr(task_t::func_ptr_t func) {
            for (size_t i = 0; i < size_; ++i) {
                if (func == tasks_[i].func()) return i;
            }
            return MaxTasks;
        }
    };

}

#endif //VT_ARDUINO_TOOLS_TASK_SCHEDULER_H
