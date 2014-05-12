/*
 Basic MQTT example 
 
  - connects to an MQTT server
  - publishes "hello world" to the topic "outTopic"
  - subscribes to the topic "inTopic"
*/ 

int rgb_r, rgb_g, rgb_b;

#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.
byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
byte server[] = { 192, 168, 1, 103 };
byte ip[]     = { 192, 168, 1, 200 };

void callback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
  int k, i;
  char buffer[256];
  
  if (length >= 256) return;
  
  Serial.print(topic);
  Serial.print(" - ");
  
  for (k = 0; k < length; k++){
      Serial.print (char(payload[k]));
      buffer[k] = payload[k];
  }
  buffer[k] = 0;
      
  Serial.print(" - ");
  
  // Serial.print(buffer);
  
  // Serial.print(" - ");
  
  i = constrain(atoi(buffer), 0, 255);
  Serial.print(i);
  
  Serial.println();
  
  //Serial.println(char(topic[strlen(topic)-1]));
  switch (char(topic[strlen(topic)-1])) {
    case 'r':
      // Serial.print("Red");
      rgb_r = i;
      break;
    case 'g':
      // Serial.println("Green");
      rgb_g = i;
      break;
    case 'b':
      // Serial.println("Blue");
      rgb_b = i;
      break;
  }
}



EthernetClient ethClient;
PubSubClient client(server, 1883, callback, ethClient);

void setup()
{
  Serial.begin(57000);
  Serial.println("Booting");

  Ethernet.begin(mac, ip);
  if (client.connect("arduinoClient")) {
    // client.publish("outTopic","hello world");
    client.subscribe("neopixel/r");
    client.subscribe("neopixel/g");
    client.subscribe("neopixel/b");
  }
  Serial.println("Booted");
}

void loop()
{
  client.loop();
}

