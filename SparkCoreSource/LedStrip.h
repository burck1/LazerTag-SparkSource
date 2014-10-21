#ifndef LedStrip_h
#define LedStrip_h

#include "application.h"
#include "neopixel/neopixel.h"

#define LED_TYPE WS2812

class LedStrip {
    public:
        LedStrip(int pin, int count);
        void clear();
        void xOn(int num, unsigned long color);
        void reload(unsigned long color, int delayMs);
        void rainbowPulse(int repeat, int delayMs);
        void cylonPulse(int repeat, int delayMs);
        void cascadePulse(int repeat, int delayMs);
    private:
        uint16_t _count;
        uint8_t _pin;
        Adafruit_NeoPixel *_ledStrip;
        void _clearLEDs();
        void _rainbow(byte startPosition);
        uint32_t _rainbowOrder(byte position);
        void _cylon(unsigned long color, byte wait);
        void _cascade(unsigned long color, byte direction, byte wait);
};

#endif
