
// Enable debug prints to serial monitor
#define MY_DEBUG

#define MY_RADIO_RFM69
#define MY_RFM69_FREQUENCY RFM69_433MHZ
#define MY_IS_RFM69HW  // Mandatory if you radio module is the high power version (RFM69HW and RFM69HCW), Comment it if it's not the case

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
int numSensors = 0;
bool receivedConfig = false;
bool metric = true;
// Initialize temperature message
MyMessage Tmsg(1, V_TEMP);

//=========================
// BATTERY MEASURER
// VOLTAGE DIVIDER SETUP
// 1M, 470K divider across battery and using internal ADC ref of 1.1V
// Sense point is bypassed with 0.1 uF cap to reduce noise at that point
// ((1e6+470e3)/470e3)*1.1 = Vmax = 3.44 Volts
// 3.44/1023 = Volts per bit = 0.003363075
#define VBAT_PER_BITS 0.003363075
#define VMIN 2.2                                  //  Vmin (radio Min Volt)=1.9V (564v)
#define VMAX 3.1                                  //  Vmax = (2xAA bat)=3.0V (892v)
int batteryPcnt = 0;                              // Calc value for battery %
int batLoop = 0;                                  // Loop to help calc average
int batArray[3];                                  // Array to store value for average calc.
int BATTERY_SENSE_PIN = A0;                       // select the input pin for the battery sense point
//=========================

void before()
{
  // Startup up the OneWire library
  sensors.begin();
}


void setup() {

  // requestTemperatures() will not block current thread
  sensors.setWaitForConversion(false);


//=========================
// BATTERY MEASURER
  //Set internal ref to internal to be able to measure bat 0-1v
  //Make sure this fits other sensors using analogread! If you have a sensor measuring 0-5v you need to change analogRef. before reading that sensor.
  analogReference(INTERNAL);
  
  //Battery inital calc
  Serial.print("With Battery VMax (100%) = ");Serial.print(VMAX);Serial.print("volts and Vmin (0%) = ");Serial.print(VMIN);Serial.println(" volts");
  Serial.print("Battert Procent 25%/50%/75% should be: ");Serial.print(((VMAX-VMIN)/4)+VMIN);Serial.print("/");Serial.print(((VMAX-VMIN)/2)+VMIN);Serial.print("/");Serial.println(VMAX-((VMAX-VMIN)/4));
  delay(1000);
  int sensorValue = analogRead(BATTERY_SENSE_PIN);
  delay(50);
  float Vbat  = sensorValue * VBAT_PER_BITS;
  int batteryPcnt = static_cast<int>(((Vbat - VMIN) / (VMAX - VMIN)) * 100.);
  Serial.print("Current battery are measured to (please confirm!): "); Serial.print(batteryPcnt); Serial.print(" % - Or "); Serial.print(Vbat); Serial.println(" Volts");
//=========================

}

void presentation()  {
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo(SKETCH_NAME, SKETCH_VERSION);

  // Fetch the number of attached temperature sensors
  numSensors = sensors.getDeviceCount();

  // Present all sensors to controller
  for (int i = 0; i < numSensors && i < MAX_ATTACHED_DS18B20; i++) {
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
  for (int i = 0; i < numSensors && i < MAX_ATTACHED_DS18B20; i++) {


    // Fetch and round temperature to one decimal
    float temperature = static_cast<float>(static_cast<int>((getControllerConfig().isMetric ? sensors.getTempCByIndex(i) : sensors.getTempFByIndex(i)) * 10.)) / 10.;

    // Only send data if temperature has changed and no error
#if COMPARE_TEMP == 1
    if (lastTemperature[i] != temperature && temperature != -127.00 && temperature != 85.00) {
#else
    if (temperature != -127.00 && temperature != 85.00) {
#endif

      // Send in the new temperature
      send(Tmsg.setSensor(i).set(temperature, 1));
      // Save new temperatures for next compare
      lastTemperature[i] = temperature;
    }
  }
//=========================
// BATTERY MEASURER
  MeasureBattery();
//=========================

  sleep(900000);  // Make sure everything is stable before start to sleep with interrupts. (don't use "wait()" here). Tests shows false trip ~2s after battery report otherwise.

}

//=========================
// BATTERY MEASURER
void MeasureBattery() //The battery calculations
{
  delay(500);
  // Battery monitoring reading
  int sensorValue = analogRead(BATTERY_SENSE_PIN);
  delay(500);

  // Calculate the battery in %
  float Vbat  = sensorValue * VBAT_PER_BITS;
  int batteryPcnt = static_cast<int>(((Vbat - VMIN) / (VMAX - VMIN)) * 100.);
  Serial.print("Battery percent: "); Serial.print(batteryPcnt); Serial.print(" %");Serial.print("Battery Voltage: "); Serial.print(Vbat); Serial.println(" Volts");

  // Add it to array so we get an average of 3 (3x20min)
  batArray[batLoop] = batteryPcnt;

  if (batLoop > 2) {
    batteryPcnt = (batArray[0] + batArray[1] + batArray[2] + batArray[3]);
    batteryPcnt = batteryPcnt / 3;

    if (batteryPcnt > 100) {
      batteryPcnt = 100;
    }

    Serial.print("Battery Average (Send): "); Serial.print(batteryPcnt); Serial.println(" %");
    sendBatteryLevel(batteryPcnt);
    batLoop = 0;
  }
  else
  {
    batLoop++;
  }
}
//=========================
