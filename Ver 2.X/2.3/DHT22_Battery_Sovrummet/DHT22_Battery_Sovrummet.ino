// Enable debug prints
//#define MY_DEBUG

// Enable and select radio type attached
#define MY_RADIO_NRF24
//#define MY_RADIO_RFM69

//Fixed ID/Parent?
#define MY_NODE_ID 2                     //To set a fixed ID for your node
//#define MY_PARENT_NODE_ID 100             //To set a fixed parent for this node

#include <SPI.h>
#include <MySensors.h>  
#include <DHT.h>  

#define CHILD_ID_HUM 0
#define CHILD_ID_TEMP 1
#define HUMIDITY_SENSOR_DIGITAL_PIN 3
unsigned long SLEEP_TIME = 600000; // Sleep time between reads (in milliseconds)

#define SKETCH_NAME "Stora Sovrummet #2"        // Change to a fancy name you like
#define SKETCH_VERSION "1.1"                    // Your version

DHT dht;
float lastTemp;
float lastHum;
boolean metric = true; 
MyMessage msgHum(CHILD_ID_HUM, V_HUM);
MyMessage msgTemp(CHILD_ID_TEMP, V_TEMP);

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

void setup()  
{ 
  
   //=========================
  // BATTERY MEASURER
  //Set internal ref to internal to be able to measure bat 0-1v
  //Make sure this fits other sensors using analogRead()! 
  //If you have a sensor reporting 0-5v you need to change analogReference() before reading that sensor.
  
  analogReference(INTERNAL);
  //DEFAULT: the default analog reference of 5 volts (on 5V Arduino boards) or 3.3 volts (on 3.3V Arduino boards)
  //INTERNAL: an built-in reference, equal to 1.1 volts on the ATmega168 or ATmega328 and 2.56 volts on the ATmega8 (not available on the Arduino Mega)
  //EXTERNAL: the voltage applied to the AREF pin (0 to 5V only) is used as the reference.
  
  //Battery inital calc
  Serial.print("With Battery VMax (100%) = "); Serial.print(VMAX); Serial.print("volts and Vmin (0%) = "); Serial.print(VMIN); Serial.println(" volts");
  Serial.print("Battery Percent 25%/50%/75% calculates to: "); Serial.print(((VMAX - VMIN) / 4) + VMIN); Serial.print("/"); Serial.print(((VMAX - VMIN) / 2) + VMIN); Serial.print("/"); Serial.println(VMAX - ((VMAX - VMIN) / 4));
  delay(1000);
  int sensorValue = analogRead(BATTERY_SENSE_PIN);
  delay(50);
  float Vbat  = sensorValue * VBAT_PER_BITS;
  int batteryPcnt = static_cast<int>(((Vbat - VMIN) / (VMAX - VMIN)) * 100.);
  Serial.print("Current battery are measured to (please confirm!): "); Serial.print(batteryPcnt); Serial.print(" % - Or "); Serial.print(Vbat); Serial.println(" Volts");
  //=========================

  delay(500); // Allow time for radio if power used as reset
  
  dht.setup(HUMIDITY_SENSOR_DIGITAL_PIN); 

}

void presentation()  
{ 
  // Send the Sketch Version Information to the Gateway
 // Send the Sketch Version Information to the Gateway
  sendSketchInfo(SKETCH_NAME, SKETCH_VERSION);

  // Register all sensors to gw (they will be created as child devices)
  present(CHILD_ID_HUM, S_HUM);
  present(CHILD_ID_TEMP, S_TEMP);
}

void loop()      
{  
  delay(500); // Allow time for radio if power used as reset
  delay(dht.getMinimumSamplingPeriod());
 
  // Fetch temperatures from DHT sensor
  float temperature = dht.getTemperature();
  if (isnan(temperature)) {
      Serial.println("Failed reading temperature from DHT");
  } else if (temperature != lastTemp) {
    lastTemp = temperature;
    if (!metric) {
      temperature = dht.toFahrenheit(temperature);
    }
    send(msgTemp.set(temperature, 1));
    Serial.print("T: ");
    Serial.println(temperature);
  }
  
  // Fetch humidity from DHT sensor
  float humidity = dht.getHumidity();
  if (isnan(humidity)) {
      Serial.println("Failed reading humidity from DHT");
  } else if (humidity != lastHum) {
      lastHum = humidity;
      send(msgHum.set(humidity, 1));
      Serial.print("H: ");
      Serial.println(humidity);
  }
  
  //=========================
  // BATTERY MEASURER
  MeasureBattery();
  //=========================
  
  sleep(SLEEP_TIME); //sleep a bit
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
#ifdef MY_DEBUG
  Serial.print("Battery percent: "); Serial.print(batteryPcnt); Serial.print(" %"); Serial.print("Battery Voltage: "); Serial.print(Vbat); Serial.println(" Volts");
#endif
  
  // Add it to array so we get an average of 3 (3x20min)
  batArray[batLoop] = batteryPcnt;

  if (batLoop > 2) {
    batteryPcnt = (batArray[0] + batArray[1] + batArray[2]);
    batteryPcnt = batteryPcnt / 3;

    if (batteryPcnt > 100) {
      batteryPcnt = 100;
    }
#ifdef MY_DEBUG
    Serial.print("Battery Average (Send): "); Serial.print(batteryPcnt); Serial.println(" %");
#endif
    sendBatteryLevel(batteryPcnt);
    batLoop = 0;
  }
  else
  {
    batLoop++;
  }
}
//=========================
