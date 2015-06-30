int led = 13;

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);     
}

// the loop routine runs over and over again forever:
void loop() {
    int  channel = 1;
   
    for (int note = 1; note < 255; note++) {
        usbMIDI.sendNoteOn(note, 99, channel);
        digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
        delay (100);
        
        digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
        usbMIDI.sendNoteOff(note, 0, channel); 
        delay (100);
    }
    for (int note = 255; note > 0; note--) {
        usbMIDI.sendNoteOn(note, 99, channel);
        digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
        delay (100);
        
        digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
        usbMIDI.sendNoteOff(note, 0, channel); 
        delay (100);
    }
}



