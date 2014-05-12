#include <Encoder.h>
Encoder knob1(3, 2);

long long positionKnob;

void setup() {
  Serial.begin(57600);
  Serial.println("Encoder Test:");
}

void loop() {
  long newKnob1;
  newKnob1 = knob1.read();
  if (newKnob1 != positionKnob) {
    Serial.print(newKnob1);
    Serial.println();
    positionKnob = newKnob1;
  }
}
