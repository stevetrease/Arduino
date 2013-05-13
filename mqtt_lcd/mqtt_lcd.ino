/*
 * Ethernet RGB LEDS with MQTT 
 * (c) Mark McKillen
 *
 */

#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
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



#define MQTT_SERVER "192.168.1.103"

byte MAC_ADDRESS[] = { 0x91, 0xA3, 0xBB, 0x00, 0x01, 0x03 };
char message_buff[30];

int LEDpin   = 9;   //

EthernetClient ethClient;  // Ethernet object
PubSubClient client( MQTT_SERVER, 1883, callback, ethClient); // MQTT object

void setup() {
  // set random MaAC 
  for (byte i = 1; i < 6; ++i) {
    MAC_ADDRESS[i] = random(0,255);     
  } 
  
  Serial.begin(57600);
  lcd.begin(16, 2);
  lcd.setBacklight(WHITE);
  randomSeed(analogRead(0));
  
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("DHCP...");
  lcd.setCursor(0, 1);
  Serial.println("DHCP...");
  
  pinMode(LEDpin, OUTPUT);

  if (Ethernet.begin(MAC_ADDRESS) == 0)       // Get connected!
  {
      Serial.println("Failed to configure Ethernet using DHCP");
      lcd.print("DHCP failed");
      return;
  } else {
      Serial.print("IP: ");                       // A little debug.. show IP address
      Serial.println(Ethernet.localIP());

      for (byte thisByte = 0; thisByte < 4; thisByte++) {
          // print the value of each byte of the IP address:
          Serial.print(Ethernet.localIP()[thisByte], DEC);
          lcd.print(Ethernet.localIP()[thisByte], DEC);
          if (thisByte < 3) {
              Serial.print("."); 
              lcd.print(".");
          }
      }   
      delay(2000);
  }
 
  if (client.connect("arduinoClient")) {      // connect to MQTT server
    client.subscribe("sensors/+/+");             // subscribe to topic "rgblight"
    Serial.println("Connected to MQTT");      // let us know this has happened
  }
}

void loop()
{
  client.loop();                              // loop for ever waiting for MQTT event
}

// handles message arrived on subscribed topic
void callback(char* topic, byte* payload, unsigned int length) {
  int i;
  
  digitalWrite(LEDpin, HIGH);
  for (int i = 0; i < length; i++) {
    Serial.print((char) payload[i]);
  }
  Serial.print('\t');
  Serial.println(topic);
 
  // lcd.setCursor(0,0);
  // for (i = 0; i < 16; i++) {
     // lcd.print(" ");
  // }
  lcd.setCursor(0, 0);
  lcd.print(topic+8);
  lcd.print("          ");
  // for (i = topic.length(); i < 16; i++) {
     // lcd.print(" ");
  // }
  
  lcd.setCursor(0,1);
  for (i = 0; i < length; i++) {
    lcd.print((char) payload[i]);
  }
  for (i; i < 16; i++) {
     lcd.print(" ");
  }
  
  digitalWrite(LEDpin, LOW);            
}
