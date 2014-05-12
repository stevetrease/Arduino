/* 
 Based on https://github.com/mat-lo/yun-examples/tree/master/color_picker 	   
*/
#include <Bridge.h>
#include <YunServer.h>
#include <YunClient.h>
#include <HttpClient.h>
#include <Adafruit_NeoPixel.h>

#define NEO_PIN    6
#define NEO_LENGTH 40

HttpClient httpClient;
YunServer server;
String startString;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NEO_LENGTH, NEO_PIN, NEO_GRB + NEO_KHZ800);

int led = 13;


int r_target = 255;
int g_target = 255;
int b_target = 255;
int r_now = 0;
int g_now = 0;
int b_now = 0;

void setup() {
  pinMode(led, OUTPUT); 
  digitalWrite(led, LOW); 
  
  strip.begin();
  strip.show();
  
  Bridge.begin();
  server.listenOnLocalhost();
  server.begin();
  httpClient.getAsynchronously("http://www.trease.eu/neopixel/booting");
  // Serial.begin(57600);
}

void loop() {
  if ((r_now != r_target) || (g_now != g_target) || (b_now != b_target)) {
    if (r_now != r_target) {
      if (r_now > r_target) r_now--; 
      if (r_now < r_target) r_now++; 
    }
    if (g_now != g_target) {
      if (g_now > g_target) g_now--; 
      if (g_now < g_target) g_now++; 
    }
    if (b_now != b_target) {
      if (b_now > b_target) b_now--; 
      if (b_now < b_target) b_now++; 
    }
    
    for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, r_now, g_now, b_now);
    }
    strip.show(); 
  }
 

  
  YunClient client = server.accept();

  if(client) {  
    digitalWrite(led, HIGH);
    String command = client.readStringUntil('/');    
    if (command == "rgb") {
      httpClient.getAsynchronously("http://www.trease.eu/neopixel/rgb");
      //received string is "r.g.b"
      //parseInt "consumes" the string
      r_target = client.parseInt();
      g_target = client.parseInt();
      b_target = client.parseInt();
      r_target = constrain(r_target,0,255);
      g_target = constrain(g_target,0,255);
      b_target = constrain(b_target,0,255);
    } else if (command == "on") {
      httpClient.getAsynchronously("http://www.trease.eu/neopixel/on");
      r_target = 255;
      g_target = 255;
      b_target = 255;
    } else if (command == "off") {
      httpClient.getAsynchronously("http://www.trease.eu/neopixel/off");
      r_target = 0;
      g_target = 0;
      b_target = 0;
    }
    client.stop();
    // Serial.print("New target  ");
    // Serial.print(r_target);
    // Serial.print(",");
    // Serial.print(g_target);
    // Serial.print(",");
    // Serial.print(b_target);
    // Serial.print(",");
    // Serial.println();
    // digitalWrite(led, LOW);
  }  
}











