#include "application.h"
#include "Trigger.h"

Trigger::Trigger(int pin, void (*onTriggerButtonPressed)(), void (*onTriggerButtonReleased)(), voidFuncPtr interruptDispatcher) {
    _pin = pin;
    _triggerButtonPressedProcessedFlag = true;
    _triggerButtonReleasedProcessedFlag = true;
    _triggerButtonPressed = onTriggerButtonPressed;
    _triggerButtonReleased = onTriggerButtonReleased;

    pinMode(_pin, INPUT);
    _triggerButtonState = digitalRead(_pin);
    attachInterrupt(_pin, interruptDispatcher, CHANGE);
}

void Trigger::checkTriggerButton() {
    if (!_triggerButtonPressedProcessedFlag) {
        (*_triggerButtonPressed)();
        _triggerButtonPressedProcessedFlag = true;
    }
    if (!_triggerButtonReleasedProcessedFlag) {
        (*_triggerButtonReleased)();
        _triggerButtonReleasedProcessedFlag = true;
    }
}

void Trigger::triggerButtonInterrupt() {
    if (_triggerButtonState == LOW) {
        // Button was just pressed
        _triggerButtonPressedProcessedFlag = false;
    } else if (_triggerButtonState == HIGH) {
        // Button was just released
        _triggerButtonReleasedProcessedFlag = false;
    }
    _triggerButtonState = digitalRead(_pin);
}
