#include <JeeLib.h>
#include <Ports.h>
#include <PortsBMP085.h>
#include <PortsLCD.h>
#include <PortsSHT11.h>
#include <RF12.h>
#include <RF12sio.h>


int count;
Port port_1(1);
Port port_2(2);
Port port_3(3);
Port port_4(4);

int node = 2;
int group = 55;






void setup() {
    rf12_initialize(node, RF12_868MHZ, group);   
    pinMode(9, OUTPUT); 
    port_1.mode2(INPUT);
    port_2.mode2(INPUT);
    // port_3.mode2(INPUT);
    // port_4.mode2(INPUT);
    Serial.begin(57600);
    Serial.println("Init:");
    count = 0;
}    


void loop() {   
    
  //  while (!rf12_canSend()) {
  //    rf12_recvDone();
  //  } 
  //  Serial.println();
    count++;
    
    unsigned long _millis = millis();
    word p1 = port_1.anaRead();
    word p2 = port_2.anaRead();
    // word p3 = port_3.anaRead();
    // word p4 = port_4.anaRead();
    
    
    String message;
    message = "g:";
    message += group;
    message += ":n:";
    message += node;
    message += ":c:";
    message += count;
    message += ":m:";
    message += _millis;
    message += ":p1:";
    message += p1;
    message += ":p2:";
    message += p2;
    // message += ":p3:";
    // message += p3;
    // message += ":p4:";
    // message += p4;
    message += ":";
   
    Serial.print(count);
    Serial.print(":1:");
    Serial.print(p1);
    Serial.print(":2:");
    Serial.print(p2);
    // Serial.print("\t3:");
    // Serial.print(p3);
    // Serial.print("\t4:");
    // Serial.print(p4);
    Serial.print(":");
    Serial.println();
  
    Serial.print(message.length());
    Serial.print("-");
    Serial.println(message);
    Serial.println();
    
    rf12_sendStart(0, &count, sizeof count);
    rf12_sendWait(2);
  
    digitalWrite(9, LOW);
    delay(25);         
    digitalWrite(9, HIGH);    
    delay(25);
    digitalWrite(9, LOW);
    delay(25);         
    digitalWrite(9, HIGH);    

     
    delay(1000);  
 }
