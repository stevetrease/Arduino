#include <JeeLib.h>
#include <Ports.h>


//    TMP102 on Jeenode port 1
//      SDA to DIO
//      SCL to AIO
int tmp102Address = 0x48;  // ADD0 to ground on TMP102
PortI2C   myport(1, PortI2C::KHZ100);
DeviceI2C temperature(myport, tmp102Address); 

// RF stuff
#define RF_NODEID 5
#define RF_CHANNEL 111

// this must be added since we're using the watchdog for low-power waiting
// ISR(WDT_vect) { Sleepy::watchdogEvent(); }

// number of millisecs to powerdown and sleep for between reading and sending reading
#define DELAY 5000


typedef struct {		//data Structure to be sent, called payload
    	  int   node;		// sending node ID
	  float data;		// data
} Payload;
Payload data;

 
void setup() {
  rf12_initialize(RF_NODEID, RF12_868MHZ, RF_CHANNEL);
  Serial.begin(57600);
  Serial.println("Initalising...");
  data.node = RF_NODEID;
}  
  

void loop() {
  while (!rf12_canSend())
    rf12_recvDone();
  
  data.data = getTemperature();
  Serial.print(millis()/1000.0);
  Serial.print("\t");
  Serial.println(data.data);
    
  rf12_sendStart(0, &data, sizeof data);
  // rf12_sendWait(2);
  
  delay(DELAY);
    
  // rf12_sleep(RF12_SLEEP);
  // Sleepy::loseSomeTime(DELAY);
  // rf12_sleep(RF12_WAKEUP);
}


// Function to get temperature from TMP102 and return it as a float in Celsius
float getTemperature()
{   
  temperature.send();             // go into send mode
  temperature.write(0x00);        //
  temperature.receive();          // grab it
  byte MSB = temperature.read(0); // Whole degrees
  byte LSB = temperature.read(1); // fractional degrees
  temperature.stop();             // done

  int TemperatureSum = ((MSB << 8) | LSB) >> 4; //it's a 12bit int, using two's compliment for negative
  float celsius = TemperatureSum*0.0625;  
  
  // Serial.print(MSB,BIN);
  // Serial.print(" ");
  // Serial.print(LSB,BIN);
  // Serial.print("\t");
  // Serial.print(TemperatureSum, BIN);
  // Serial.print("\t");
  // Serial.print(celsius);
  // Serial.println();
  
  return celsius;
}
