#include <Adafruit_NeoPixel.h>

#define PIN 7 
#define BRIGHT 12



// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip)

int XSIZE = 8;
int YSIZE = 5;


Adafruit_NeoPixel strip = Adafruit_NeoPixel(XSIZE * YSIZE, PIN, NEO_GRB + NEO_KHZ800);

// bool array [XSIZE] [YSIZE];


void setup() {
  Serial.begin(57600);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  delay(10000);
}



void loop() {
   int x, y;
   
   for (y = 0; y < YSIZE; y++) {
     for (x = 0; x < XSIZE; x++) {
       int i = (x) + (y * XSIZE);
       Serial.print(x);
       Serial.print("\t");
       Serial.print(y);
       Serial.print("\t");
       Serial.print(i);
       Serial.print("\t");
       Serial.println();

       strip.setPixelColor(i, 255, 0, 0);
       strip.show();
       delay(50);
       strip.setPixelColor(i, 0, 0, 0);
     } 
   }
  
  
  // Some example procedures showing how to display to the pixels:
  // colorWipe(strip.Color(BRIGHT - 1 , 0, 0), 50); // Red
   // colorWipe(strip.Color(0, BRIGHT - 1, 0), 50); // Green
  // colorWipe(strip.Color(0, 0, BRIGHT - 1), 50); // Blue
  // rainbow(20);
  // rainbowCycle(20);
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

