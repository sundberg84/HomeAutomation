// Enable debug prints
#define MY_DEBUG

// Enable and select radio type attached
#define MY_RADIO_NRF24
//#define MY_RADIO_RFM69

#define MY_NODE_ID 20

#include <SPI.h>
#include <MySensors.h>

#define CHILD_ID_Motion 2   // Id of the sensor child
#define DIGITAL_INPUT_SENSOR 4   // The digital input you attached your motion sensor.  (Only 2 and 3 generates interrupt!)
int oldTripped = 0;
int tripped = 0;

#define LED_PIN 3      // Arduino pin attached to MOSFET Gate pin
#define FADE_DELAY 10  // Delay in ms for each percentage fade up/down (10ms = 1s full-range dim)
static int16_t currentLevel = 0;  // Current dim level...

// Initialize motion and dimmer message
MyMessage msg(CHILD_ID_Motion, V_TRIPPED);
MyMessage dimmerMsg(0, V_DIMMER);
MyMessage lightMsg(1, V_LIGHT);


void setup()  
{  
  pinMode(DIGITAL_INPUT_SENSOR, INPUT);      // sets the motion sensor digital pin as input
}

void presentation()  {
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo("Dimmer Kitchen", "1.0");

  // Register all sensors to gw (they will be created as child devices)
  present(CHILD_ID_Motion, S_MOTION);
  wait(10);
  present(0, S_DIMMER );

          // Inform the gateway of the current DimmableLED's SwitchPower1 and LoadLevelStatus value...
        send(lightMsg.set(currentLevel > 0));

        // hek comment: Is this really nessesary?
        send( dimmerMsg.set(currentLevel) );
  
}

void loop()     
{
     
//Read digital motion value
tripped = digitalRead(DIGITAL_INPUT_SENSOR);

if (tripped != oldTripped){
#ifdef MY_DEBUG
  Serial.println("Motion");
#endif
  send(msg.set(tripped?"1":"0"));  // Send tripped value to gw
  oldTripped = tripped;
}}

void receive(const MyMessage &message)
{
    if (message.type == V_LIGHT || message.type == V_DIMMER) {

        //  Retrieve the power or dim level from the incoming request message
        int requestedLevel = atoi( message.data );

        // Adjust incoming level if this is a V_LIGHT variable update [0 == off, 1 == on]
        requestedLevel *= ( message.type == V_LIGHT ? 100 : 1 );

        // Clip incoming level to valid range of 0 to 100
        requestedLevel = requestedLevel > 100 ? 100 : requestedLevel;
        requestedLevel = requestedLevel < 0   ? 0   : requestedLevel;
#ifdef MY_DEBUG
        Serial.print( "Changing level to " );
        Serial.print( requestedLevel );
        Serial.print( ", from " );
        Serial.println( currentLevel );
#endif
        fadeToLevel( requestedLevel );

        // Inform the gateway of the current DimmableLED's SwitchPower1 and LoadLevelStatus value...
        send(lightMsg.set(currentLevel > 0));

        // hek comment: Is this really nessesary?
        send( dimmerMsg.set(currentLevel) );
    }
}

/***
 *  This method provides a graceful fade up/down effect
 */
void fadeToLevel( int toLevel )
{

    int delta = ( toLevel - currentLevel ) < 0 ? -1 : 1;

    while ( currentLevel != toLevel ) {
        currentLevel += delta;
        analogWrite( LED_PIN, (int)(currentLevel / 100. * 255) );
        delay( FADE_DELAY );
    }
}
