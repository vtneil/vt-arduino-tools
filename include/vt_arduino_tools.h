#ifndef VT_ARDUINO_TOOLS_VT_ARDUINO_TOOLS_H
#define VT_ARDUINO_TOOLS_VT_ARDUINO_TOOLS_H

#include <Arduino.h>
#include <stdint.h>
#include <string.h>

namespace vt {
    /**
     * Build Arduino String from multiple fields, separated by commas.
     * @param last Value
     * @return built Arduino String
     */
    template<typename T>
    String build_string(T last) { return String(last); }

    /**
     * Build Arduino String from multiple fields, separated by commas.
     * @param first Value
     * @param args Values
     * @return built Arduino String
     */
    template<size_t StringReserveSize = 128, typename T, typename... Ts>
    String build_string(T first, Ts... args) {
        String s0 = "";
        s0.reserve(StringReserveSize);
        s0 += String(first);
        s0 += ",";
        s0 += build_string<Ts...>(args...);
        return s0;
    }

    template<typename SerialT = HardwareSerial, SerialT *stream = &Serial, typename T>
    void write_stream(T last) { stream->write(last); }

    template<typename SerialT = HardwareSerial, SerialT *stream = &Serial, typename T, typename... Ts>
    void write_stream(T first, Ts... args) {
        stream->write(first);
        write_stream<SerialT, stream, Ts...>(args...);
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

#endif //VT_ARDUINO_TOOLS_VT_ARDUINO_TOOLS_H
