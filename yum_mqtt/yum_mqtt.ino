// #include <PubSubClient.h>

#include <Bridge.h>
#include <HttpClient.h>


// Update these with values suitable for your network.
byte server[] = { 192, 168, 1, 103 };


// PubSubClient client(server, 1883, callback, ethClient);

void setup()
{
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  Bridge.begin();
  digitalWrite(13, HIGH);
  
}

void loop()
{
  HttpClient client;
  
  // Make a HTTP request:
  client.connect(server, 1883);
  if (client.connected()) {
    client.write("Hello");    
  };
  delay (1000);
}

