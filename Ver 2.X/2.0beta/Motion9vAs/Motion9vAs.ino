
// Enable debug prints
#define MY_DEBUG

// Enable and select radio type attached
#define MY_RADIO_NRF24
//#define MY_RADIO_RFM69
#define MY_NODE_ID 16

#include <SPI.h>
#include <MySensor.h>

unsigned long SLEEP_TIME = 900000; // Sleep time between reports (in milliseconds)
#define DIGITAL_INPUT_SENSOR 3   // The digital input you attached your motion sensor.  (Only 2 and 3 generates interrupt!)
#define INTERRUPT DIGITAL_INPUT_SENSOR-2 // Usually the interrupt = pin -2 (on uno/nano anyway)
#define CHILD_ID 1   // Id of the sensor child
#define SKETCH_NAME "MotionTaketHall #16"                // Change to a fancy name you like
#define SKETCH_VERSION "1.0"                    // Your version

// Initialize motion message
MyMessage msg(CHILD_ID, V_TRIPPED);
boolean sl = false;

void setup()  
{  
  pinMode(DIGITAL_INPUT_SENSOR, INPUT);      // sets the motion sensor digital pin as input
}

void presentation()  {
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo(SKETCH_NAME, SKETCH_VERSION);

  // Register all sensors to gw (they will be created as child devices)
  present(CHILD_ID, S_MOTION);
}

void loop()     
{     
  // Read digital motion value
  boolean tripped = digitalRead(DIGITAL_INPUT_SENSOR) == HIGH; 
        
  Serial.println(tripped);
  send(msg.set(tripped?"1":"0"));  // Send tripped value to gw 

if (sl==false){
  sendHeartbeat();
}
 
  // Sleep until interrupt comes in on motion sensor. Send update every two minute. 
  sl = sleep(INTERRUPT,CHANGE, SLEEP_TIME);
}
