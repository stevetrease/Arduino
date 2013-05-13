/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */

void setup() {                
  // initialize the digital pin as an output.
  // Pin 13 has an LED connected on most Arduino boards:
  pinMode(5, OUTPUT);     
  pinMode(6, OUTPUT); 
}

void loop() {
  digitalWrite(5, HIGH);   // set the LED on
  digitalWrite(6, LOW);
  delay(1000);              // wait for a second
  digitalWrite(5, LOW);    // set the LED off
  digitalWrite(6, HIGH); 
  delay(250);              // wait for a second
}
