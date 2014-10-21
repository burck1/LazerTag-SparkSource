#ifndef Trigger_h
#define Trigger_h

#include "application.h"

void triggerButtonPressed(void);
void triggerButtonReleased(void);

class Trigger {
    public:
        Trigger(int pin, void (*onTriggerButtonPressed)(), void (*onTriggerButtonReleased)(), voidFuncPtr interruptDispatcher);
        void checkTriggerButton();
        void triggerButtonInterrupt();
    private:
        int _pin;
        volatile int _triggerButtonState;
        volatile boolean _triggerButtonPressedProcessedFlag;
        volatile boolean _triggerButtonReleasedProcessedFlag;
        void (*_triggerButtonPressed)();
        void (*_triggerButtonReleased)();
};

#endif
