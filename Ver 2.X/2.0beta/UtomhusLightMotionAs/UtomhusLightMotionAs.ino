// Enable debug prints to serial monitor
#define MY_DEBUG

// Enable and select radio type attached
#define MY_RADIO_NRF24
//#define MY_RADIO_RFM69

//Sketch/Node
#define MY_NODE_ID 13
#define MY_REPEATER_FEATURE

#include <SPI.h>
#include <MySensor.h>
#include <Wire.h>
#include <DallasTemperature.h>
#include <OneWire.h>

#define SketchName "Utomhus #13"
#define SketchVer "2.0"
unsigned long currentTime;
long previousTime = -300000 ;
long interval = 300000;

// ----------------------------------------------------------------------------
// Child sensor ids
#define CHILD_ID_MOTION 0
#define CHILD_ID_LIGHT 1

//----------------------------------------------------------------------------
// Light/Lux Sensor
#define LIGHT_SENSOR_ANALOG_PIN 0
MyMessage LightMsg(CHILD_ID_LIGHT, V_LIGHT_LEVEL);
int lastLightLevel = -1;

// ----------------------------------------------------------------------------
// Motion
#define DIGITAL_INPUT_SENSOR 3   // The digital input you attached your motion sensor.  (Only 2 and 3 generates interrupt!)
#define INTERRUPT DIGITAL_INPUT_SENSOR-2 // Usually the interrupt = pin -2 (on uno/nano anyway)
int oldTripped = 0;
int tripped = 0;
MyMessage msgMotion(CHILD_ID_MOTION, V_TRIPPED);

//Dallas Temp (Internal AC temp measure)
#define COMPARE_TEMP 0                      // Send temperature only if changed? 1 = Yes 0 = No
#define ONE_WIRE_BUS 8 // Pin where dallase sensor is connected 
#define MAX_ATTACHED_DS18B20 1
unsigned long SLEEP_TIME = 30000; // Sleep time between reads (in milliseconds)
OneWire oneWire(ONE_WIRE_BUS); // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
DallasTemperature sensors(&oneWire); // Pass the oneWire reference to Dallas Temperature.
float lastTemperature[MAX_ATTACHED_DS18B20];
int numSensors = 0;
boolean receivedConfig = false;
boolean metric = true;
// Initialize temperature message
MyMessage MsgDallas(2, V_TEMP);

void setup()
{
  pinMode(DIGITAL_INPUT_SENSOR, INPUT);      // sets the motion sensor digital pin as input

  //Temp

  // Startup up the OneWire library
  sensors.begin();
  // requestTemperatures() will not block current thread
  sensors.setWaitForConversion(false);
  getTemp();
  
}

void presentation()
{
  sendSketchInfo(SketchName, SketchVer);

  present(CHILD_ID_MOTION, S_MOTION);
  present(CHILD_ID_LIGHT, V_LIGHT_LEVEL);
   present(2, S_TEMP);
}

void loop()
{
  currentTime = millis();
  // Read digital motion value
  tripped = digitalRead(DIGITAL_INPUT_SENSOR);

  if (tripped != oldTripped)
  {
    motion();
  }

  if (currentTime - previousTime > interval) {
    previousTime = currentTime;
    updateLightSensor();
    getTemp();
  }
}

void updateLightSensor() {

  int lightLevel = (1023 - analogRead(LIGHT_SENSOR_ANALOG_PIN)) / 10.23;
  Serial.print("lightLevel: ");Serial.println(lightLevel);

    send(LightMsg.set(lightLevel));
    lastLightLevel = lightLevel;

}

void motion() {
  Serial.println("Motion");
  send(msgMotion.set(tripped ? "1" : "0")); // Send tripped value to gw
  oldTripped = tripped;
}

void getTemp() {
  Serial.println("Getting AC Temp");

  // Fetch temperatures from Dallas sensors
  sensors.requestTemperatures();

  // query conversion time and sleep until conversion completed
  int16_t conversionTime = sensors.millisToWaitForConversion(sensors.getResolution());
  // sleep() call can be replaced by wait() call if node need to process incoming messages (or if node is repeater)
  sleep(conversionTime);

  // Fetch and round temperature to one decimal
  float temperature = static_cast<float>(static_cast<int>((getConfig().isMetric ? sensors.getTempCByIndex(0) : sensors.getTempFByIndex(0)) * 10.)) / 10.;

  if (lastTemperature[0] != temperature && temperature != -127.00 && temperature != 85.00) {
    // Send in the new temperature
    send(MsgDallas.setSensor(0).set(temperature, 1));
    // Save new temperatures for next compare
    lastTemperature[0] = temperature;

  }
}

