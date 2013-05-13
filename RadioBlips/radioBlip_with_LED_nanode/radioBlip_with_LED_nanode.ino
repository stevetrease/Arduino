// Send out a radio packet every minute, consuming as little power as possible
// 2010-08-29 <jc@wippler.nl> http://opensource.org/licenses/mit-license.php

#include <JeeLib.h>

static long payload;


void setup() {
    rf12_initialize(0, RF12_868MHZ, 212);
    pinMode(5, OUTPUT); 
    pinMode(6, OUTPUT);
    Serial.begin(9600);
}

void loop() {
    ++payload;
    
    digitalWrite(6,LOW);
    digitalWrite(5,LOW);
   
    while (!rf12_canSend()) {
        // flicker the red LED whilst can't send
        digitalWrite(6,LOW);
        rf12_recvDone();
        Serial.println("rf12_recvDone()");        
        delay(100);
        digitalWrite(6,HIGH);
        delay(50);
    }
    digitalWrite(6, LOW);
    
    rf12_sendStart(0, &payload, sizeof payload);
    // set the sync mode to 2 if the fuses are still the Arduino default
    // mode 3 (full powerdown) can only be used with 258 CK startup fuses
    rf12_sendWait(2);
    
    Serial.println("rf12_sendWait()");
    // digitalWrite(5, HIGH);   
    // delay(125);              
    // digitalWrite(5, LOW);
    // delay(125);
    // digitalWrite(5, HIGH);   
    // delay(125);              
    // digitalWrite(5, LOW);
    
    delay(2000);
}
