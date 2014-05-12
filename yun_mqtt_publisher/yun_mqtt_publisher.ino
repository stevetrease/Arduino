
#include <PubSubClient.h>
#include <bridge.h>
#include <YunClient.h>


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
  pinMode(13,OUTPUT);
  digitalWrite(13, LOW);
  Bridge.begin();
  digitalWrite(13, HIGH);  
}

void loop()
{
  // client.loop();
    
  if (client.connect("arduinoClient")) {
    digitalWrite(13, LOW);
    dtostrf( (float) millis() / 1000.0, 4, 2, message_buff);
    client.publish("outTopic",message_buff);
    digitalWrite(13, HIGH);
  }
  // delay(1000);
}

