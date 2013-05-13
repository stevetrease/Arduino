/*
  Web  Server
 
 A simple web server that shows the value of the analog input pins.
 using an Arduino Wiznet Ethernet shield. 
 
 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Analog inputs attached to pins A0 through A5 (optional)
 
 created 18 Dec 2009
 by David A. Mellis
 modified 4 Sep 2010
 by Tom Igoe
 
 */

#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 192,168,1, 177 };

// Initialize the Ethernet server library
// with the IP address and port you want to use 
// (port 80 is default for HTTP):
Server server(80);

void setup()
{
  // setup Arduino ethernet LED
  pinMode(9, OUTPUT);
  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
}

void loop()
{   
  int sensor_value = 0;
  int temp_c = 0;
  int temp_f = 0;
  
  // listen for incoming clients
  Client client = server.available();
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
          temp_f = (temp_c * 9)/ 5 + 32;
          
          client.print("analog 0 input is ");
          client.print(sensor_value);
          client.println("<br />");
          
          client.print("Celcius ");
          client.print(temp_c);
          client.println("<br />");
          
          client.print("Fahrenheit ");
          client.print(temp_f);
          client.println("<br />");
          
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
