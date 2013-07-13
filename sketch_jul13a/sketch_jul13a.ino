
 float value = -34.03;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  randomSeed(analogRead(0));


}

void loop() {
  
  char  topic[128];
  char  message[128];

   
  float value =+ random(-7500,15000) / 1000.0;
 

  sprintf (topic, "sensor/%s", "01");
  dtostrf(value, 4, 2, message);
   
  Serial.print(topic);
  Serial.print(", ");
  Serial.println(message);
  
  delay(random(2500));
}
