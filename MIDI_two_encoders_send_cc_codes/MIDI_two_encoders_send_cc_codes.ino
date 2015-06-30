#include <Encoder.h>

int led = 13;
int channel = 1;
int note = 0; // C4
int octave = 5;

long positionOne = 0;
long positionTwo = 0;

Encoder knobOne(0, 1);
Encoder knobTwo(2, 3);


void setup () {
  pinMode(led, OUTPUT);   
  digitalWrite(led, LOW);

}


void loop () {
  long newOne, newTwo;
  int changed = 0;
  
  newOne = knobOne.read();
  newTwo = knobTwo.read();
  
  
  if (newOne != positionOne) {
    if (newOne < positionOne) {
      note--;
    } else {
      note++;
    }
    note = constrain(note,0, 11);
    positionOne = newOne;
    changed = 1;
  }
  if (newTwo != positionTwo) {
    if (newTwo < positionTwo) {
      octave--;
    } else {
      octave++;
    }
    octave = constrain (octave, 0, 10);
    positionTwo = newTwo;
    changed = 1;
  }

  if (changed == 1) {
    int midiNote = (octave * 12) + note;
    digitalWrite(led, HIGH);
    usbMIDI.sendNoteOn(midiNote, 99, channel);
    delay(10);   
    usbMIDI.sendNoteOn(midiNote, 0, channel);
    digitalWrite(led, LOW); 
    changed = 0;
  }
}



