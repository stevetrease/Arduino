/*
  DHCP-based IP printer

 This sketch uses the DHCP extensions to the Ethernet library
 to get an IP address via DHCP and print the address obtained.
 using an Arduino Wiznet Ethernet shield.

 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13

 created 12 April 2011
 modified 9 Apr 2012
 by Tom Igoe

 */

#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include <Adafruit_NeoPixel.h>

byte server[] = { 192, 168, 1, 103 };

#define BUFFER_LEN 64
#define redpin 3
#define greenpin 5
#define bluepin 7
#define PIN 6
#define NUMPIXELS      16

#define commonAnode true

// our RGB -> eye-recognized gamma color
byte gammatable[256];


Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);




EthernetClient ethClient;
PubSubClient client(server, 1883, callback, ethClient);

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = {
  0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02
};
String  ipAddressString = "";
char    buffer[BUFFER_LEN];




void setup() {
  
  // pinMode(redpin, OUTPUT);
  // pinMode(greenpin, OUTPUT);
  // pinMode(bluepin, OUTPUT);
  
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  
  // thanks PhilB for this gamma table!
  // it helps convert RGB colors to what humans see
  for (int i=0; i<256; i++) {
    float x = i;
    x /= 255;
    x = pow(x, 2.5);
    x *= 255;
      
    if (commonAnode) {
      gammatable[i] = 255 - x;
    } else {
      gammatable[i] = x;      
    }
  }
  
  // Open serial communications and wait for port to open:
  Serial.begin(57600);
  // this check is only needed on the Leonardo:
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  
  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1); // halt!
  }

  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    for (;;)
      ;
  }
  // print your local IP address:
  Serial.print("My IP address: ");
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    ipAddressString += Ethernet.localIP()[thisByte];
    ipAddressString += ".";
  }
  Serial.println(ipAddressString);
  ipAddressString.toCharArray(buffer, BUFFER_LEN);
  
  if (client.connect("arduinoClient")) {
    // client.publish("outTopic","hello world");
    client.publish("sensors/colour/ip",buffer);
  }
}


void loop() {
  uint16_t clear, red, green, blue;
  
  
  // client.loop();
  
  tcs.getRawData(&red, &green, &blue, &clear);
  
  Serial.print("C:\t"); Serial.print(clear);
  Serial.print("\tR:\t"); Serial.print(red);
  Serial.print("\tG:\t"); Serial.print(green);
  Serial.print("\tB:\t"); Serial.print(blue);

  // Figure out some basic hex code for visualization
  uint32_t sum = clear;
  float r, g, b;
  r = red; r /= sum;
  g = green; g /= sum;
  b = blue; b /= sum;
  r *= 256; g *= 256; b *= 256;
  Serial.print("\t");
  Serial.print((int)r, HEX); Serial.print((int)g, HEX); Serial.print((int)b, HEX);
  Serial.println();

  //Serial.print((int)r ); Serial.print(" "); Serial.print((int)g);Serial.print(" ");  Serial.println((int)b );

  // analogWrite(redpin, gammatable[(int)r]);
  // analogWrite(greenpin, gammatable[(int)g]);
  // analogWrite(bluepin, gammatable[(int)b]);
  
  ipAddressString = "";
  ipAddressString += String((int)r,HEX);
  ipAddressString += String((int)g, HEX);
  ipAddressString += String((int)b, HEX);
  ipAddressString.toCharArray(buffer, BUFFER_LEN);
  client.publish("sensors/colour/rgb",buffer);
  
  ipAddressString = "";
  ipAddressString += String(gammatable[(int)r], HEX);
  ipAddressString += String(gammatable[(int)g], HEX);
  ipAddressString += String(gammatable[(int)b], HEX);
  ipAddressString.toCharArray(buffer, BUFFER_LEN);
  client.publish("sensors/colour/gamma",buffer);
  
  
  for(int i=0;i<NUMPIXELS;i++){
    strip.setPixelColor(i, strip.Color(gammatable[(int)r], gammatable[(int)g], gammatable[(int)b])); // Moderately bright green color.
    strip.show(); // This sends the updated pixel color to the hardware.
  }
}



void callback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
}
