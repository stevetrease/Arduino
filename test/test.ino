#include <SPI.h>
#include <Ethernet.h>
#include <Wire.h>
#include <Adafruit_MCP23017.h>
#include <Adafruit_RGBLCDShield.h>

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7


// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = {  
  0x00, 0xAA, 0xBB, 0xCC, 0xCE, 0x02 };

// Initialize the Ethernet client library
// with the IP address and port of the server 
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);

  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Starting");
  

  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    lcd.setCursor(0, 0);
    lcd.print("DHCP failed");
    // no point in carrying on, so do nothing forevermore:
    for(;;)
      ;
  }
  // print your local IP address:
  Serial.print("My IP address: ");
  lcd.setCursor(0, 0);
  lcd.print("My IP address: ");
  lcd.setCursor(0, 1);
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    Serial.print(Ethernet.localIP()[thisByte], DEC);
    lcd.print(Ethernet.localIP()[thisByte], DEC);
    Serial.print("."); 
    lcd.print(".");
  }
  Serial.println();
}

void loop() {
}


