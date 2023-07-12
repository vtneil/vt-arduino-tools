#ifndef VT_ARDUINO_TOOLS_ARDUINO_STREAM_H
#define VT_ARDUINO_TOOLS_ARDUINO_STREAM_H

#include <Arduino.h>
#include "arduino_tools.h"

namespace vt {
    constexpr const char *endl = "\r\n";

    class arduino_iostream {
    protected:
        Stream &stream;

    public:
        explicit arduino_iostream(Stream &stream)
                : stream(stream) {}

        arduino_iostream(const arduino_iostream &) = delete;

        arduino_iostream(arduino_iostream &&) noexcept = delete;

        arduino_iostream &operator=(const arduino_iostream &) = delete;

        arduino_iostream &operator=(arduino_iostream &&) = delete;

        template<typename T>
        arduino_iostream &operator<<(const T &value) {
            stream.print(value);
            return *this;
        }

        arduino_iostream &operator>>(String &str) {
            while (stream.available()) str += static_cast<char>(stream.read());
            return *this;
        }
    };
}

#endif //VT_ARDUINO_TOOLS_ARDUINO_STREAM_H
