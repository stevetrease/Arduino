// Send out a radio packet every minute, consuming as little power as possible
// 2010-08-29 <jc@wippler.nl> http://opensource.org/licenses/mit-license.php

#include <JeeLib.h>


// this must be added since we're using the watchdog for low-power waiting
ISR(WDT_vect) { Sleepy::watchdogEvent(); }

Port port_1(1);


void setup() {
    rf12_initialize(17, RF12_868MHZ, 5);
    port_1.mode2(INPUT);
    pinMode(9, OUTPUT); 
}

void loop() {
   word p1 = port_1.anaRead();
    
    while (!rf12_canSend())
        rf12_recvDone();
    
    rf12_sendStart(0, &p1, sizeof p1);
    // set the sync mode to 2 if the fuses are still the Arduino default
    // mode 3 (full powerdown) can only be used with 258 CK startup fuses
    rf12_sendWait(2);
    digitalWrite(9, LOW);
    delay(25);         
    digitalWrite(9, HIGH);    
    delay(25);
    digitalWrite(9, LOW);
    delay(25);         
    digitalWrite(9, HIGH);
    
    rf12_sleep(RF12_SLEEP);
    Sleepy::loseSomeTime(2500);
    rf12_sleep(RF12_WAKEUP);
}
