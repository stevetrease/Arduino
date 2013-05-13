/*
  DHCP-based 
  Two 2 wire temperature sensor webserver
 */

#include <SPI.h>
#include <Ethernet.h>
#include <Wire.h>
#include <BMP085.h>

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = {  
  0x90, 0xA2, 0xDA, 0x00, 0x74, 0x79 };

int tmp102Address_1 = 0x49;
int tmp102Address_2 = 0x48;
byte res;
int val;
BMP085 bmp;

// Initialize the Ethernet server library
// with the IP address and port you want to use 
// (port 80 is default for HTTP):
EthernetServer server(80);

// Initialize the Ethernet client library
// with the IP address and port of the server 
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;

void setup() {
  // start the serial library:
  Serial.begin(9600);
  // start the 2wire library:
  Wire.begin();
  
  bmp.begin(); 
  
  // setup output 9 as an LED
  pinMode(9, OUTPUT);
    
  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, flash LED 9 forevermore:
    //for(;;) {
//   delay(250);
//       digitalWrite(9, HIGH);   // set the LED on
//       delay(125);             
//       digitalWrite(9, LOW);    // set the LED off
//       Serial.print("X");
//    }
  }
  // print your local IP address:
  Serial.print("My IP address: "); 
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    Serial.print(Ethernet.localIP()[thisByte], DEC);
    Serial.print("."); 
  }
  Serial.println();
  
  // start the server:
  server.begin(); 
}

void loop() {    
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // blink LED 9
          digitalWrite(9, HIGH);   // set the LED on
          delay(125);             
          digitalWrite(9, LOW);    // set the LED off
          
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();
          
          
          Wire.requestFrom(tmp102Address_1,2); 
          byte MSB = Wire.read();
          byte LSB = Wire.read();
          int sensor_value_1 = ((MSB << 8) | LSB) >> 4; //it's a 12bit int, using two's compliment for negative
          float temp_c_1 = sensor_value_1*0.0625;  

          Wire.requestFrom(tmp102Address_2,2); 
          MSB = Wire.read();
          LSB = Wire.read();
          int sensor_value_2 = ((MSB << 8) | LSB) >> 4; //it's a 12bit int, using two's compliment for negative
          float temp_c_2 = sensor_value_2*0.0625; 
                
          client.print("Temp 1 is ");
          client.print(temp_c_1);
          client.print(" Celsius (");
          client.print(sensor_value_1);
          client.print(")<br>");
          Serial.print("Temp 1 is ");
          Serial.print(temp_c_1);
          Serial.print(" (");
          Serial.print(sensor_value_1);
          Serial.print(")");
          Serial.println();

          client.print("Temp 2 is ");
          client.print(temp_c_2);
          client.print(" Celsius (");
          client.print(sensor_value_2);
          client.print(")<br>");
          Serial.print("Temp 2 is ");
          Serial.print(temp_c_2);
          Serial.print(" (");
          Serial.print(sensor_value_2);
          Serial.println(")");
          
          client.print("Pressure is ");
          client.print(bmp.readPressure());
          client.println(" Pa");
          Serial.print("Pressure is ");
          Serial.print(bmp.readPressure());
          Serial.println(" Pa");
    
          Serial.println();
          Serial.println();
          
          // blink LED 9
          delay(64);
          digitalWrite(9, HIGH);   // set the LED on
          delay(125);             
          digitalWrite(9, LOW);    // set the LED off  
         
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
  }
}




