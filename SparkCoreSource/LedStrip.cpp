#include "application.h"
#include "neopixel/neopixel.h"
#include "LedStrip.h"
#include "LedColors.h"

LedStrip::LedStrip(int pin, int count) {
    _pin = pin;
    _count = count;
    _ledStrip = new Adafruit_NeoPixel(count, pin, LED_TYPE);
    _ledStrip->begin();
    clear();
}

void LedStrip::clear() {
    _clearLEDs();
    _ledStrip->show();
}

void LedStrip::_clearLEDs() {
    for (int i=0; i<_count; i++) {
        _ledStrip->setPixelColor(i, 0);
    }
}

void LedStrip::xOn(int num, unsigned long color) {
    if (_count < num) { num = _count; }
    _clearLEDs();
    for (int i=0; i<num; i++) {
        _ledStrip->setPixelColor(i, color);
    }
    _ledStrip->setBrightness(255);
    _ledStrip->show();
}

void LedStrip::reload(unsigned long color, int delayMs) {
    clear();
    _ledStrip->setBrightness(255);
    for (int i=0; i<_count; i++) {
        _ledStrip->setPixelColor(i, color);
        _ledStrip->show();
        delay(delayMs);
    }
}

void LedStrip::rainbowPulse(int repeat, int delayMs) {
    for (int i=0; i<_count*repeat; i++) {
        _rainbow(i);
        delay(delayMs);
    }
}

void LedStrip::cylonPulse(int repeat, int delayMs) {
    for (int i=0; i<repeat; i++) {
        _cylon(INDIGO, delayMs);
    }
}

void LedStrip::cascadePulse(int repeat, int delayMs) {
    for (int i=0; i<repeat; i++) {
        _cascade(MEDIUMSPRINGGREEN, TOP_DOWN, delayMs);
    }
}

void LedStrip::_rainbow(byte startPosition) {
    int rainbowScale = 192 / _count;
    for (int i=0; i<_count; i++) {
        _ledStrip->setPixelColor(i, _rainbowOrder((rainbowScale * (i + startPosition)) % 192));
    }
    _ledStrip->show();
}
uint32_t LedStrip::_rainbowOrder(byte position) {
    if (position < 31) { // Red -> Yellow (Red = FF, blue = 0, green goes 00-FF)
        return _ledStrip->Color(0xFF, position * 8, 0);
    }
    else if (position < 63) { // Yellow -> Green (Green = FF, blue = 0, red goes FF->00)
        position -= 31;
        return _ledStrip->Color(0xFF - position * 8, 0xFF, 0);
    }
    else if (position < 95) { // Green->Aqua (Green = FF, red = 0, blue goes 00->FF)
        position -= 63;
        return _ledStrip->Color(0, 0xFF, position * 8);
    }
    else if (position < 127) { // Aqua->Blue (Blue = FF, red = 0, green goes FF->00)
        position -= 95;
        return _ledStrip->Color(0, 0xFF - position * 8, 0xFF);
    }
    else if (position < 159) { // Blue->Fuchsia (Blue = FF, green = 0, red goes 00->FF)
        position -= 127;
        return _ledStrip->Color(position * 8, 0, 0xFF);
    }
    else { //160 <position< 191   Fuchsia->Red (Red = FF, green = 0, blue goes FF->00)
        position -= 159;
        return _ledStrip->Color(0xFF, 0x00, 0xFF - position * 8);
    }
}

void LedStrip::_cylon(unsigned long color, byte wait) {
    const byte weight = 4;
    byte red = (color & 0xFF0000) >> 16;
    byte green = (color & 0x00FF00) >> 8;
    byte blue = (color & 0x0000FF);

    for (int i=0; i<=_count-1; i++) {
        _clearLEDs();
        _ledStrip->setPixelColor(i, red, green, blue);
        for (int j=1; j<3; j++) {
            if (i-j >= 0)
                _ledStrip->setPixelColor(i-j, red/(weight*j), green/(weight*j), blue/(weight*j));
            if (i-j <= _count)
                _ledStrip->setPixelColor(i+j, red/(weight*j), green/(weight*j), blue/(weight*j));
        }
        _ledStrip->show();
        delay(wait);
    }

    for (int i=_count-2; i>=1; i--) {
        _clearLEDs();
        _ledStrip->setPixelColor(i, red, green, blue);
        for (int j=1; j<3; j++) {
            if (i-j >= 0)
                _ledStrip->setPixelColor(i-j, red/(weight*j), green/(weight*j), blue/(weight*j));
            if (i-j <= _count)
                _ledStrip->setPixelColor(i+j, red/(weight*j), green/(weight*j), blue/(weight*j));
            }
        _ledStrip->show();
        delay(wait);
    }
}

void LedStrip::_cascade(unsigned long color, byte direction, byte wait) {
    if (direction == TOP_DOWN) {
        for (int i=0; i<_count; i++) {
            _clearLEDs();  // Turn off all LEDs
            _ledStrip->setPixelColor(i, color);  // Set just this one
            _ledStrip->show();
            delay(wait);
        }
    }
    else {
        for (int i=_count-1; i>=0; i--) {
            _clearLEDs();
            _ledStrip->setPixelColor(i, color);
            _ledStrip->show();
            delay(wait);
        }
    }
}
