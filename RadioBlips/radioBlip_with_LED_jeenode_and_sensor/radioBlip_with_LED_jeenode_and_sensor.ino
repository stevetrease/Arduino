// Send out a radio packet every minute, consuming as little power as possible
// 2010-08-29 <jc@wippler.nl> http://opensource.org/licenses/mit-license.php

#include <JeeLib.h>

static word payload;
Port port_1(1);

// this must be added since we're using the watchdog for low-power waiting
ISR(WDT_vect) { Sleepy::watchdogEvent(); }

void setup() {
#if defined(__AVR_ATtiny84__)
    cli();
    CLKPR = bit(CLKPCE);
    CLKPR = 0; // div 1, i.e. speed up to 8 MHz
    sei();
#endif
    rf12_initialize(4, RF12_868MHZ, 100);
    pinMode(9, OUTPUT); 
    port_1.mode2(INPUT);
    Serial.begin(9600);
}    


void loop() {
    while (!rf12_canSend())
        rf12_recvDone();
   
    payload = port_1.anaRead();
    Serial.println(payload,HEX);
    Serial.println("Hello");
    rf12_sendStart(0, &payload, sizeof payload);
    // set the sync mode to 2 if the fuses are still the Arduino default
    // mode 3 (full powerdown) can only be used with 258 CK startup fuses
    rf12_sendWait(2);
    
    digitalWrite(9, LOW);
    delay(250);              // wait for a second
    digitalWrite(9, HIGH);
    
    rf12_sleep(RF12_SLEEP);
    Sleepy::loseSomeTime(5000);
    rf12_sleep(RF12_WAKEUP);
}
