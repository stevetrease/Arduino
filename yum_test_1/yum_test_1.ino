#include <Adafruit_NeoPixel.h>

#define PIN 6 
#define BRIGHT 12

// #include <Bridge.h>
// #include <YunServer.h>
// #include <YunClient.h>


// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip)

int XSIZE = 8;
int YSIZE = 5;

int target_r = 123;
int target_g = 200;
int target_b = 45;
int r = 0;
int g = 0;
int b = 0;

// Listen on default port 5555, the webserver on the Yun
// will forward there all the HTTP requests for us.
// YunServer server;


Adafruit_NeoPixel strip = Adafruit_NeoPixel(XSIZE * YSIZE, PIN, NEO_GRB + NEO_KHZ800);

// bool array [XSIZE] [YSIZE];


void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  Serial.begin(57600);
  
  pinMode(13,OUTPUT);
  digitalWrite(13, LOW);
  // Bridge.begin();
  digitalWrite(13, HIGH);
  
  // server.listenOnLocalhost();
  // server.begin();
  
  delay(100);
}



void loop() {   

  // Bridge.put("r", String(r));
  // Bridge.put("g", String(g));
  // Bridge.put("b", String(b));
  
  colorWipe(strip.Color(r, g, b), 00);

  int i = target_r - r;
  if (i != 0) {
    if (i > 0) {
      r++;  
    } else {
      r--;
    }
  }
  
  i = target_g - g;
  if (i != 0) {
    if (i > 0) {
      g++;  
    } else {
      g--;
    }
  }
    
  i = target_b - b;
  if (i != 0) {
    if (i > 0) {
      b++;  
    } else {
      b--;
    }
  }

  
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<BRIGHT; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & BRIGHT));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
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

