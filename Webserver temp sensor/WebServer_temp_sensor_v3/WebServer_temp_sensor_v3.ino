/*
  DHCP-based IP printer
 
 This sketch uses the DHCP extensions to the Ethernet library
 to get an IP address via DHCP and print the address obtained.
 using an Arduino Wiznet Ethernet shield. 
 
 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 
 created 12 April 2011
 by Tom Igoe
 
 */

#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = {  
  0x00, 0x22, 0x33, 0x44, 0x55, 0x66 };


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
  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    for(;;)
      ;
  }
  // print your local IP address:
  Serial.print("My IP address: "); 
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    Serial.print(Ethernet.localIP()[thisByte], DEC);
    Serial.print("."); 
  }
  Serial.println();
  
  // setup Arduino ethernet LED
  pinMode(9, OUTPUT);
  // start the server:
  server.begin();
  
  
}

void loop() {   
  int sensor_value = 0;
  float temp_c = 0;
  // float temp_f = 0;
  
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

          // output the value of each analog input pin
          
          sensor_value = analogRead(0);
          temp_c = (5.0 * sensor_value * 100.0) / 1024.0;
          // temp_f = (temp_c * 9)/ 5 + 32;
          
          client.print("analog 0 input is ");
          client.print(sensor_value);
          client.println("<br>");
          
          client.print("Celsius ");
          client.print(temp_c);
          client.println("<br>");
          
          // client.print("Fahrenheit ");
          // client.print(temp_f);
          // client.println("<br>");
          
          // blink LED 9
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



