/*
 ===================================================
 Use a web page to set a RGB LED to the selected color
 ===================================================

 ===================================================
 Converted to analog write commands and removed BlinkM
 Also removed simple text box page and used code from 
 http://www.colorpicker.com/ to pick the LED color.

 Mike Audleman
 Jun 3, 2012
 ===================================================

 ===================================================
 Prior Header
 ===================================================
*/

// Small Arduino Webserver --- for BlinkM
// by Andreas Cahen ---** updated for Arduino 0021 - by PsycleSam and Co. 20101219 tested on arduino uno with updated ethernet shield
//
// Used libraries and examples:
//
// Arduino Ethershield, server.pde
// Blinkm Library, BlinkMfuncs.h +++**SPI Library, SPI.h
// ---** no longer used - TextString Library, WString.h




#include <SPI.h>
#include <Ethernet.h>
#include <Wire.h>
#define maxLength 25


#include <Adafruit_NeoPixel.h>

#define PIN 6

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(8, PIN, NEO_GRB + NEO_KHZ800);





byte mac[] = {
  0x12, 0x12, 0x12, 0x12, 0x12, 0x12 }; //Set your own MAC
// Using DHCP
//byte ip[] = {
//  192, 168, 0, 200 };

String inString = String(maxLength);
int val;
int r = 0;
int g = 0;
int b = 0;
char colorBuff[4];

EthernetServer server(80);

/*
----------------------------------------------------------
Setup configuration.
---------------------------------------------------------- 
*/
void setup()
{
  uint16_t i;
     
  Serial.begin(57600);
  Serial.println("Booting");
  Ethernet.begin(mac);
  server.begin();


  // Init Testing of the LED
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  rainbow(1);
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, (r,g,b));
  }
  strip.show(); 
}

/*
----------------------------------------------------------
Main Logic Loop
---------------------------------------------------------- 
*/
void loop()
{
  uint16_t i;
  int bufLength;
  EthernetClient client = server.available();
  if (client) {
    boolean current_line_is_blank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (inString.length() < maxLength) {
          //inString.append(c);
          inString += c;
        }        
        if (c == '\n' && current_line_is_blank) {
          if (inString.indexOf("?") > -1) {

            int Pos_CC = inString.indexOf("k=")+2;
            int End_CC = inString.indexOf("&", Pos_CC);

            if(End_CC < 0){
              End_CC =  inString.length() + 1;
            }
            String ColorHex = inString.substring(Pos_CC,End_CC);
            Serial.println( "Received Hex String " + ColorHex );
            r = CheckRange( HexToDec(ColorHex.substring(0,2)) );
            g = CheckRange( HexToDec(ColorHex.substring(2,4)) );
            b = CheckRange( HexToDec(ColorHex.substring(4,6)) );

            // SetLED( LEDr, r );
            // SetLED( LEDg, g );
            // SetLED( LEDb, b );
            for(i=0; i<strip.numPixels(); i++) {
              strip.setPixelColor(i, r,g,b);
            }
            strip.show();
          }

          ShowWebPage (client);

          break;
        }
        if (c == '\n') {
          current_line_is_blank = true;
        }
        else if (c != '\r') {
          current_line_is_blank = false;
        }
      }
    }
    delay(1);
    inString = "";
    client.stop();
  }
} 



/*
----------------------------------------------------------
Validates the passed value 0 > val > 255
---------------------------------------------------------- 
*/
int CheckRange( int val ) {
  int tmpVal = val;
  if ( tmpVal > 255 ){
    tmpVal=255;
  };
  if ( tmpVal < 0 ){
    tmpVal=0;
  };
  return tmpVal;
}

/*
----------------------------------------------------------
Displays the web page
---------------------------------------------------------- 
*/
void ShowWebPage (EthernetClient theClient) {
  String t = "";
  theClient.println("HTTP/1.1 200 OK");
  theClient.println("Content-Type: text/html");
  theClient.println();
  theClient.println("<html>");

  // t = "<script type='text/javascript'";
  theClient.println("<head><base href='http://www.colorpicker.com/'><link rel='stylesheet' href='css/colorpicker.com.css' type='text/css'>");
  // theClient.print(t);
  theClient.println("<script type='text/javascript' src='js/mootools-1.2.4-core.js'></script>");
  // theClient.print(t);
  theClient.println("<script type='text/javascript' src='js/colorpicker.com.js'></script></head>");
  theClient.println("<body>");

  theClient.println("<div><div style='position:absolute;top:0px;left:0px;width:430px'><div id='colorDiv' style=display:none;>.</div><div id='colorBoxes'></div></div>");
  theClient.println("<center><div id='colorWidgetDiv' style='position:relative;padding-left:100px;width:450px;height:340px;margin-top:70px;'></div></center></div>");
  theClient.println("<form method=get onSubmit='SE();'>");
  theClient.println("<input type='hidden' id='k' name='k'value='0'>");
  theClient.println("<input type='submit' id='H' name='H' value='Submit'>");
  theClient.println("<input type='hidden' id='cc' name='colorcode' value='0'>");
  theClient.println("</form>");

  theClient.println("<script language='javascript'>");
  theClient.println("function SE(){var s = $$('input[class^=ColorPicker_com_colorSlider_rgbCode]');var c = s.get('value');c = c.toString(c); ");
  t = "').set('value',c); ";
  theClient.print("$('k");
  theClient.println(t);
  theClient.print("$('cc");
  theClient.println(t);
  theClient.println("}");
  theClient.println("</script>");
  theClient.println("</body></html>");

}


/*
----------------------------------------------------------
Converts a single hex character to a byte
---------------------------------------------------------- 
*/
int hex2dec(byte c) {
  if (c >= '0' && c <= '9') {
    return c - '0';
  } else if (c >= 'A' && c <= 'F') {
    return c - 'A' + 10;
  }
}

/*
----------------------------------------------------------
Converts a 2 byte hex string to an int
---------------------------------------------------------- 
*/
int HexToDec(String c) {
  int Digit1 = hex2dec( c.charAt(0) ) * 16;
  int Digit2 = hex2dec( c.charAt(1) );
  return Digit1 + Digit2;
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

