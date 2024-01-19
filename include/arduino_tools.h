#ifndef VT_ARDUINO_TOOLS_ARDUINO_TOOLS_H
#define VT_ARDUINO_TOOLS_ARDUINO_TOOLS_H

#include <stdint.h>
#include <string.h>
#include <Arduino.h>

namespace vt {
    template<bool First = true, typename T>
    String &build_csv_to(String &s0, const T &val) {
        if (First) s0 = "";
        s0 += val;
        return s0;
    }

    template<bool First = true, typename T, typename ...Ts>
    String &build_csv_to(String &s0, const T &val, const Ts (&...vals)) {
        if (First) s0 = "";
        s0 += val;
        s0 += ",";
        build_csv_to<false, Ts...>(s0, vals...);
        return s0;
    }

    /**
     * Build Arduino String from multiple fields, separated by commas.
     * @param val Value
     * @return built Arduino String
     */
    template<size_t = 0, typename T>
    String build_csv(const T &val) { return String(val); }

    /**
     * Build Arduino String from multiple fields, separated by commas.
     * @param val Value
     * @return built Arduino String
     */
    template<size_t = 0, typename = String>
    String build_csv(const String &val) { return val; }

    /**
     * Build Arduino String from multiple fields, separated by commas.
     * @return empty Arduino String
     */
    template<size_t StringReserveSize = 0, typename = String>
    String build_csv() {
        String s0 = "";
        if (StringReserveSize > 0) s0.reserve(StringReserveSize);
        return s0;
    }

    /**
     * Build Arduino String from multiple fields, separated by commas.
     * @param val Value
     * @param vals Values
     * @return built Arduino String
     */
    template<size_t StringReserveSize = 128, typename T, typename ...Ts>
    String build_csv(const T &val, const Ts (&...vals)) {
        String s0 = "";
        if (StringReserveSize > 0) s0.reserve(StringReserveSize);
        s0 += val;
        s0 += ",";
        s0 += build_csv<StringReserveSize, Ts...>(vals...);
        return s0;
    }

    template<bool First = true, typename T>
    String &build_string_to(String &s0, const T &val) {
        if (First) s0 = "";
        s0 += val;
        return s0;
    }

    template<bool First = true, typename T, typename ...Ts>
    String &build_string_to(String &s0, const T &val, const Ts (&...vals)) {
        if (First) s0 = "";
        s0 += val;
        build_string_to<false, Ts...>(s0, vals...);
        return s0;
    }

    /**
     * Build Arduino String from multiple fields.
     * @param val Value
     * @return built Arduino String
     */
    template<size_t = 0, typename T>
    String build_string(const T &val) { return String(val); }

    /**
     * Build Arduino String from multiple fields.
     * @param val Value
     * @return built Arduino String
     */
    template<size_t = 0, typename = String>
    String build_string(const String &val) { return val; }

    /**
     * Build Arduino String from multiple fields
     * @return empty Arduino String
     */
    template<size_t StringReserveSize = 0, typename = String>
    String build_string() {
        String s0 = "";
        if (StringReserveSize > 0) s0.reserve(StringReserveSize);
        return s0;
    }

    /**
     * Build Arduino String from multiple fields.
     * @param val Value
     * @param vals Values
     * @return built Arduino String
     */
    template<size_t StringReserveSize = 128, typename T, typename ...Ts>
    String build_string(const T &val, const Ts (&...vals)) {
        String s0 = "";
        if (StringReserveSize > 0) s0.reserve(StringReserveSize);
        s0 += val;
        s0 += build_string<StringReserveSize, Ts...>(vals...);
        return s0;
    }

    template<typename SerialT = HardwareSerial, SerialT *stream = &Serial, typename T>
    void write_stream(T val) { stream->write(val); }

    template<typename SerialT = HardwareSerial, SerialT *stream = &Serial, typename T, typename... Ts>
    void write_stream(T val, Ts... vals) {
        stream->write(val);
        write_stream<SerialT, stream, Ts...>(vals...);
    }

#ifdef __arm__
    extern "C" char *sbrk(int incr);
#else
    extern char *__brkval;
#endif

    /**
     * Get Free RAM Memory
     * @return free memory remaining
     */
    size_t free_memory() {
        char top;
#ifdef __arm__
        return &top - reinterpret_cast<char *>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
        return &top - __brkval;
#else
        return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif
    }
}

#endif //VT_ARDUINO_TOOLS_ARDUINO_TOOLS_H
