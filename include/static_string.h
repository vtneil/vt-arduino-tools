#ifndef VT_ARDUINO_TOOLS_STATIC_STRING_H
#define VT_ARDUINO_TOOLS_STATIC_STRING_H

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <float.h>

namespace vt {
    namespace impl {
        template<size_t MAX_SIZE = 64, typename char_type = char>
        class static_string_t {
        protected:
            char_type str_[MAX_SIZE];

        public:
            constexpr static_string_t() = delete;

            constexpr static_string_t(const static_string_t &other) = default;

            constexpr static_string_t(static_string_t &&other) noexcept = default;

            explicit static_string_t(uint8_t number) {

            }

            static_string_t &operator=(const static_string_t &other) = default;

            static_string_t &operator=(static_string_t &&other) noexcept = default;

            const char_type *c_str() const { return str_; }
        };
    }
}

#endif //VT_ARDUINO_TOOLS_STATIC_STRING_H
