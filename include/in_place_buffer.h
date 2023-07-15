#ifndef VT_ARDUINO_TOOLS_IN_PLACE_BUFFER_H
#define VT_ARDUINO_TOOLS_IN_PLACE_BUFFER_H

namespace vt {
    template<typename T, size_t N>
    class in_place_buffer {
        static_assert(N > 0, "Buffer size cannot be zero.");

    private:
        T arr_[N] = {};
        size_t size_ = 0;
        size_t back_ = 0;

    public:
        T *ptr() { return arr_; }

        bool push(const T &data) {
            arr_[back_] = data;
            back_ = (back_ + 1) % N;
            size_ = (valid() ? size_ : size_ + 1);
            return valid();
        }

        T sum() const {
            T result;
            for (size_t i = 0; i < size_; ++i) result += arr_[i];
            return result;
        }

        T average() const { return sum() / size_; }

        template<typename CallablePredicate, typename ...Args>
        bool all(CallablePredicate predicate, const Args &...args) const {
            for (size_t i = 0; i < size_; ++i) if (!predicate(arr_[i], args...)) return false;
            return true;
        }

        template<typename CallablePredicate, typename ...Args>
        bool any(CallablePredicate predicate, const Args &...args) const {
            for (size_t i = 0; i < size_; ++i) if (predicate(arr_[i], args...)) return true;
            return false;
        }

        constexpr bool valid() const { return size_ == N - 1; }

        constexpr T size() const { return size_; }
    };
}

#endif //VT_ARDUINO_TOOLS_IN_PLACE_BUFFER_H
