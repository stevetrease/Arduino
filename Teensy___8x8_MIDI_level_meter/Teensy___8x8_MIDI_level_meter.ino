/* Print Incoming Message USB MIDI Example
   Use the Arduino Serial Monitor to view the messages
   as Teensy receives them by USB MIDI

   You must select MIDI from the "Tools > USB Type" menu

   This example code is in the public domain.
*/

#include <Wire.h>
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"

int led = 13;

static uint8_t PROGMEM
  smile[] =
  { B00111100,
    B01000010,
    B10100101,
    B10000001,
    B10100101,
    B10011001,
    B01000010,
    B00111100 };
    
static uint8_t levels[] =
  { 0,
    0,
    0,
    0,
    0,
    0,
    0,
    0 };



// Adafruit_7segment matrix = Adafruit_7segment();
Adafruit_8x8matrix matrix = Adafruit_8x8matrix();

void setup() { 
  pinMode(led, OUTPUT); 

  matrix.begin(0x70);
  matrix.clear();
  matrix.drawBitmap(0, 0, smile, 8, 8, LED_ON);
  matrix.writeDisplay();
  delay(1500);
  matrix.drawBitmap(0, 0, levels, 8, 8, LED_ON);
  matrix.writeDisplay();
  
  usbMIDI.setHandleControlChange(OnControlChange);
}

void loop() { 
  usbMIDI.read(); // USB MIDI receive
}






void OnControlChange(byte channel, byte control, byte value) {
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  if (control == 7) {
    int bits = map(value, 0, 127, 0, 7);
    
    digitalWrite(led, HIGH); 
    if (channel > 0 && channel <= 8) {
      levels[channel - 1] = pow (2, bits);
    }
    digitalWrite(led, LOW); 
  }
  
  matrix.clear();
  matrix.drawBitmap(0, 0, levels, 8, 8, LED_ON);
  matrix.writeDisplay();
 
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
}


