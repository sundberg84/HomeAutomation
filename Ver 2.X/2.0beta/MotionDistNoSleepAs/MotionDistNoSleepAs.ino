

// Enable debug prints
#define MY_DEBUG

// Enable and select radio type attached
#define MY_RADIO_NRF24
//#define MY_RADIO_RFM69

#define MY_NODE_ID 12

#include <SPI.h>
#include <MyConfig.h>
#include <MySensors.h>
#include <NewPing.h>
  
#define DIGITAL_INPUT_SENSOR 3   // The digital input you attached your motion sensor.  (Only 2 and 3 generates interrupt!)
#define CHILD_ID 1   // Id of the sensor child
#define CHILD_ID_DIST 2
#define TRIGGER_PIN  6  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     5  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 100 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

// Initialize motion message
MyMessage msg(CHILD_ID, V_TRIPPED);

// Initialize distance message
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
MyMessage msgdist(CHILD_ID_DIST, V_DISTANCE);
int lastDist;
boolean metric = true;
int oldTripped = 0;
int tripped = 0; 
unsigned long lastRequest = 0;

void setup()  
{  
  pinMode(DIGITAL_INPUT_SENSOR, INPUT);      // sets the motion sensor digital pin as input
  metric = getConfig().isMetric;
}

void presentation()  {
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo("Paddington", "1.3");

  // Register all sensors to gw (they will be created as child devices)
  present(CHILD_ID, S_MOTION);
  wait(100);
  present(CHILD_ID_DIST, S_DISTANCE);
}

void loop(){     

unsigned long now = millis();

//Read digital motion value
tripped = digitalRead(DIGITAL_INPUT_SENSOR);

if (tripped != oldTripped){
  lastRequest = now;

  wait(1000);
  
  // Distance?
  int dist = metric?sonar.ping_cm():sonar.ping_in();
  Serial.print("Ping: ");
  Serial.print(dist); // Convert ping time to distance in cm and print result (0 = outside set distance range)
  Serial.println(metric?" cm":" in");

  send(msgdist.set(dist));
  wait(500);
   // Motion?
  send(msg.set(tripped?"1":"0"));  // Send tripped value to gw 
  oldTripped = tripped;
}

if ((now - lastRequest) > (60UL * 1000UL * 60UL)){
lastRequest = now;
Serial.print("Heartbeat");
sendHeartbeat();}
}


