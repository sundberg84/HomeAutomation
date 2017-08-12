
// Enable debug prints to serial monitor
//#define MY_DEBUG 

#define MY_RADIO_RFM69
#define MY_RFM69_FREQUENCY RFM69_433MHZ
#define MY_NODE_ID 19
#define SKETCH_NAME "Garden Beercooler #19"
#define SKETCH_VERSION "1.0"

#include <SPI.h>
#include <MySensors.h>  
#include <Vcc.h>
#include <DallasTemperature.h>
#include <OneWire.h>

#define COMPARE_TEMP 0 // Send temperature only if changed? 1 = Yes 0 = No
#define ONE_WIRE_BUS 3 // Pin where dallase sensor is connected 
#define MAX_ATTACHED_DS18B20 1
OneWire oneWire(ONE_WIRE_BUS); // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
DallasTemperature sensors(&oneWire); // Pass the oneWire reference to Dallas Temperature. 
float lastTemperature[MAX_ATTACHED_DS18B20];
int numSensors=0;
bool receivedConfig = false;
bool metric = true;
// Initialize temperature message
MyMessage Tmsg(1,V_TEMP);

#define CHILD_ID 1   // Id of the sensor child
#define VCC_MIN 1.9
#define VCC_MAX 3.3

MyMessage msg(CHILD_ID, V_VOLTAGE);

Vcc vcc;

void before()
{
  // Startup up the OneWire library
  sensors.begin();
}


void setup() {
  // put your setup code here, to run once:

  // requestTemperatures() will not block current thread
  sensors.setWaitForConversion(false);

}

void presentation()  {
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo(SKETCH_NAME, SKETCH_VERSION);

  // Register all sensors to gw (they will be created as child devices)
  present(CHILD_ID, S_MULTIMETER);

 // Fetch the number of attached temperature sensors  
  numSensors = sensors.getDeviceCount();

  // Present all sensors to controller
  for (int i=0; i<numSensors && i<MAX_ATTACHED_DS18B20; i++) {   
     present(i, S_TEMP);
  }
}


void loop() {
  
    // Fetch temperatures from Dallas sensors
  sensors.requestTemperatures();

  // query conversion time and sleep until conversion completed
  int16_t conversionTime = sensors.millisToWaitForConversion(sensors.getResolution());
  // sleep() call can be replaced by wait() call if node need to process incoming messages (or if node is repeater)
  sleep(conversionTime);

  // Read temperatures and send them to controller 
  for (int i=0; i<numSensors && i<MAX_ATTACHED_DS18B20; i++) {

    // Fetch and round temperature to one decimal
    float temperature = static_cast<float>(static_cast<int>((getControllerConfig().isMetric?sensors.getTempCByIndex(i):sensors.getTempFByIndex(i)) * 10.)) / 10.;

    // Only send data if temperature has changed and no error
    #if COMPARE_TEMP == 1
    if (lastTemperature[i] != temperature && temperature != -127.00 && temperature != 85.00) {
    #else
    if (temperature != -127.00 && temperature != 85.00) {
    #endif

      // Send in the new temperature
      send(Tmsg.setSensor(i).set(temperature,1));
      // Save new temperatures for next compare
      lastTemperature[i]=temperature;
    }
  }
  
  
  // put your main code here, to run repeatedly:

     sendBatteryReport();
     float volts = vcc.Read_Volts();
    send(msg.set(volts,1));
 
  sleep(900000);  // Make sure everything is stable before start to sleep with interrupts. (don't use "wait()" here). Tests shows false trip ~2s after battery report otherwise.

}

void sendBatteryReport() {
 
  float p = vcc.Read_Perc(VCC_MIN, VCC_MAX, true);
  int batteryPcnt = static_cast<int>(p);
#ifdef MY_DEBUG
  Serial.print("Battery is: "); Serial.println(batteryPcnt);
#endif
  sendBatteryLevel(batteryPcnt);
}
