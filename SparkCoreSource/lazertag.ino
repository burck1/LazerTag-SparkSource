// This #include statement was automatically added by the Spark IDE.
#include "Ammo.h"

// This #include statement was automatically added by the Spark IDE.
#include "Trigger.h"

// This #include statement was automatically added by the Spark IDE.
#include "LedStrip.h"
#include "LedColors.h"

// This #include statement was automatically added by the Spark IDE.
#include "IRremote.h"

int led = D7;

/* --- LED Strip Variables --- */
#define AMMO_LED_STRIP_PIN D4
#define AMMO_LED_STRIP_COUNT 10
#define MAX_AMMO 10 /* same as AMMO_LED_STRIP_COUNT for now  */
LedStrip ammoLedStrip(AMMO_LED_STRIP_PIN, AMMO_LED_STRIP_COUNT);
int current_ammo;
unsigned long ammoStripColor = RED;

#define LIVES_LED_STRIP_PIN D5
#define LIVES_LED_STRIP_COUNT 10
#define MAX_LIVES 10
LedStrip livesLedStrip(LIVES_LED_STRIP_PIN, LIVES_LED_STRIP_COUNT);
int current_lives;
unsigned long livesStripColor = GREEN;

/* --- IR Led Variables --- */
#define IRLed D2
IRsend irsend(IRLed);
int MY_IR_ID = 500;
int OTHER_IR_ID = 1500;

/* --- Trigger Button Variables --- */
#define triggerButtonPin D1
void triggerButtonPressed(void);
void triggerButtonReleased(void);
void triggerDispatcher(void);
Trigger trigger(triggerButtonPin, triggerButtonPressed, triggerButtonReleased, &triggerDispatcher);

/* --- IR Receiver Variables --- */
#define irRecieverPin D3
volatile unsigned long irRecieverStartTime;
volatile unsigned long irRecieverTimeDiff;
volatile boolean irRecieverDataReceived;


/* --- Runs on boot --- */
void setup() {
    pinMode(led, OUTPUT);
    digitalWrite(led, LOW);

    current_ammo = MAX_AMMO;
    ammoLedStrip.xOn(current_ammo, ammoStripColor);

    current_lives = MAX_LIVES;
    livesLedStrip.xOn(current_lives, livesStripColor);

    setupIRReceiver();
}

/* --- Runs continuously --- */
void loop() {
    trigger.checkTriggerButton();

    if (irRecieverDataReceived) {
        if (irRecieverTimeDiff > 0 && irRecieverTimeDiff < 2000) {
            // I'm hit!
            if (irRecieverTimeDiff < 1000) {
                if (MY_IR_ID >= 1000) {
                    iveBeenHit(OTHER_IR_ID);
                } else {
                    // I hit myself
                }
            } else if (irRecieverTimeDiff >= 1000) {
                if (MY_IR_ID < 1000) {
                    iveBeenHit(OTHER_IR_ID);
                } else {
                    // I hit myself
                }
            }
        } else {
            // just noise
        }
        irRecieverDataReceived = false;
    }
}

void iveBeenHit(int irId) {
    current_lives -= 1;
    if (current_lives == 0) {
        Spark.publish("im-hit", String(irId), 60, PRIVATE);
        iLoose();
    } else {
        livesLedStrip.clear();
        delay(200);
        livesLedStrip.xOn(current_lives, livesStripColor);
        delay(200);
        livesLedStrip.clear();
        delay(200);
        livesLedStrip.xOn(current_lives, livesStripColor);
        delay(200);
        livesLedStrip.clear();
        delay(200);
        livesLedStrip.xOn(current_lives, livesStripColor);
        delay(200);
        livesLedStrip.clear();
        delay(200);
        livesLedStrip.xOn(current_lives, livesStripColor);

        Spark.publish("im-hit", String(irId), 60, PRIVATE);
    }
}

void iLoose() {
    noInterrupts();
    while (true) {
        ammoLedStrip.xOn(AMMO_LED_STRIP_COUNT, BLUE);
        delay(50);
        livesLedStrip.xOn(LIVES_LED_STRIP_COUNT, BLUE);
        delay(50);
        ammoLedStrip.clear();
        delay(50);
        livesLedStrip.clear();
        delay(50);
    }
}

void IRremoteSendID() {
    unsigned int buf0[] { MY_IR_ID };
    irsend.sendRaw(buf0, 1, 38);
    // delay(2);
    // unsigned int buf1[] { 500 };
    // irsend.sendRaw(buf1, 1, 38);
    // delay(2);
    // unsigned int buf2[] { 1500 };
    // irsend.sendRaw(buf2, 1, 38);
}

/* --- Trigger buttton functions --- */
void triggerButtonPressed() {
    digitalWrite(led, HIGH);
    IRremoteSendID();
    current_ammo -= 1;
    if (current_ammo <= 0) {
        reloadAmmo();
    } else {
        ammoLedStrip.xOn(current_ammo, ammoStripColor);
    }
}
void triggerButtonReleased() {
    digitalWrite(led, LOW);
}
void triggerDispatcher() {
    trigger.triggerButtonInterrupt();
}
void reloadAmmo() {
    ammoLedStrip.clear();
    delay(100);
    ammoLedStrip.xOn(1, ammoStripColor);
    delay(100);
    ammoLedStrip.clear();
    delay(100);
    ammoLedStrip.xOn(1, ammoStripColor);
    delay(100);
    ammoLedStrip.clear();
    delay(100);
    current_ammo = MAX_AMMO;
    ammoLedStrip.reload(ammoStripColor, 50);
}


/* --- IR Receiver Functions ---  */
void setupIRReceiver() {
    irRecieverDataReceived = false;
    pinMode(irRecieverPin, INPUT);
    attachInterrupt(irRecieverPin, irRecieverChanged, CHANGE);
}

void irRecieverChanged() {
    unsigned long now = micros();

    if (irRecieverDataReceived) { return; } // previous signal not read

    if (digitalRead(irRecieverPin) == LOW) { // HIGH to LOW
        irRecieverStartTime = now;
    } else { // LOW to HIGH
        irRecieverTimeDiff = now - irRecieverStartTime;
        irRecieverDataReceived = true;
    }
}
