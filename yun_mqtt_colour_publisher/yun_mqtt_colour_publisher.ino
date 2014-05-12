#include <PubSubClient.h>
#include <bridge.h>
#include <YunClient.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"


Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);

// Update these with values suitable for your network.
byte server[] = { 192, 168, 1, 103 };
char message_buff[64];

void callback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
}
YunClient yunClient;
PubSubClient client(server, 1883, callback, yunClient);



void setup()
{
  // Serial.begin(57600);
  pinMode(13,OUTPUT);
  digitalWrite(13, LOW);
  Bridge.begin();
  tcs.begin();
  client.connect("arduinoClient");
  digitalWrite(13, HIGH);  
}

void loop()
{
  uint16_t r, g, b, c, colorTemp, lux;
  
  tcs.getRawData(&r, &g, &b, &c);
  colorTemp = tcs.calculateColorTemperature(r, g, b);
  lux = tcs.calculateLux(r, g, b);
  
  // Serial.print("Color Temp: "); Serial.print(colorTemp, DEC); Serial.print(" K - ");
  // Serial.print("Lux: "); Serial.print(lux, DEC); Serial.print(" - ");
  // Serial.print("R: "); Serial.print(r, DEC); Serial.print(" ");
  // Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
  // Serial.print("B: "); Serial.print(b, DEC); Serial.print(" ");
  // Serial.print("C: "); Serial.print(c, DEC); Serial.print(" ");
  // Serial.println(" ");
    
  mqtt_pub("sensors/light/temp", colorTemp);
  mqtt_pub("sensors/light/lux", lux);
  mqtt_pub("sensors/light/r", r);
  mqtt_pub("sensors/light/g", g);
  mqtt_pub("sensors/light/b", b);
  mqtt_pub("sensors/light/c", c);
  
  // Serial.println("++++++++++++++++++++++");

  delay(10000);
}

void mqtt_pub (char *topic, uint16_t value) {
  digitalWrite(13, LOW);
  // Serial.print(topic);
  // Serial.print("\t");
  // Serial.println(value, DEC);
  dtostrf(value, 4, 0, message_buff);
  client.publish(topic,message_buff);
  digitalWrite(13, HIGH);
}
