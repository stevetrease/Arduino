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

#include <Adafruit_NeoPixel.h>
#define PIN 6
#define LENGTH 8

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LENGTH, PIN, NEO_GRB + NEO_KHZ800);



// Update these with values suitable for your network.
byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
byte server[] = { 192, 168, 1, 103 };
byte ip[]     = { 192, 168, 1, 200 };

void callback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
  int k, i;
  char buffer[256];
  
  if (length >= 256) return;
  
  // Serial.print(topic);
  // Serial.print(" - ");
  
  for (k = 0; k < length; k++){
      Serial.print (char(payload[k]));
      buffer[k] = payload[k];
  }
  buffer[k] = 0;
      
  // Serial.print(" - ");
  
  // Serial.print(buffer);
  
  // Serial.print(" - ");
  
  i = constrain(atoi(buffer), 0, 255);
  // Serial.print(i);
  
  // Serial.println();
  
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
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, rgb_r, rgb_g, rgb_b);
  }
  strip.show();
}



EthernetClient ethClient;
PubSubClient client(server, 1883, callback, ethClient);

void setup()
{
  // Serial.begin(57000);
  // Serial.println("Booting");
  
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  rainbow(1);
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, 0, 0, 0);
  }
  strip.show(); 

  Ethernet.begin(mac, ip);
  if (client.connect("arduinoClient")) {
    // client.publish("outTopic","hello world");
    client.subscribe("neopixel/r");
    client.subscribe("neopixel/g");
    client.subscribe("neopixel/b");
  }
  // Serial.println("Booted");
}

void loop()
{
  client.loop(); 
}





void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}
// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

