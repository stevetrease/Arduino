#include <Adafruit_NeoPixel.h>

#define PIN 6
#define BRIGHT 4



// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip)

#define X  8
#define Y  5
int XSIZE = X;
int YSIZE = Y;

int modx(int a) { return (a+XSIZE) % XSIZE; }
int mody(int a) { return (a+YSIZE) % YSIZE; }

Adafruit_NeoPixel strip = Adafruit_NeoPixel(XSIZE * YSIZE, PIN, NEO_GRB + NEO_KHZ800);

bool array [X] [Y];
bool newarray [X] [Y];
int r, g, b;

void setup() {
  randomSeed(analogRead(0));
  Serial.begin(57600);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
 
  r = random(0,48);
  g = random(0,48);
  b = random(0,48);

  for (int y = 0; y < YSIZE; y++) {
    for (int x = 0; x < XSIZE; x++) {
      if (random(1,10) < 6) {
        array[x][y] = false;
      } else {
        array[x][y] = true;
      }
    }
  }
}



void loop() {
  
  for (int y=0 ; y<YSIZE ; y++) {
    for (int x=0 ; x<XSIZE; x++) {    
      // count alive neighbors
      int alive = 0;
      alive += array[modx(x+1)][mody(y)  ] != 0;
      alive += array[modx(x)  ][mody(y+1)] != 0;
      alive += array[modx(x-1)][mody(y)  ] != 0;
      alive += array[modx(x)  ][mody(y-1)] != 0;
      alive += array[modx(x+1)][mody(y+1)] != 0;
      alive += array[modx(x-1)][mody(y-1)] != 0;
      alive += array[modx(x+1)][mody(y-1)] != 0;
      alive += array[modx(x-1)][mody(y+1)] != 0;
      if (array[x][y])
        if (alive < 2 || alive > 3)
          newarray[x][y] = 0;
        else
          newarray[x][y] = array[x][y] + 1;
      else
        if (alive == 3)
          newarray[x][y] = 1;
    }
  }    
  
  for (int y=0 ; y<YSIZE; y++) {
    for (int x=0 ; x<XSIZE; x++) {    
      array[x][y] = newarray[x][y];
    }
  }  
  
  
  
   for (int y = 0; y < YSIZE; y++) {
     for (int x = 0; x < XSIZE; x++) {
       int i = (x) + (y * XSIZE);
       if (array[x][y] == true) {
         strip.setPixelColor(i, r, g, b);
       } else {
         strip.setPixelColor(i, 0, 0, 0);
       }         
     } 
   }
   strip.show();
   delay(1000);
}

