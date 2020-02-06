/// Enable debug prints
#define MY_DEBUG

// Enable and select radio type attached
#define MY_RADIO_NRF24
//#define MY_RADIO_RFM69

#define MY_NODE_ID 21

#include <MySensors.h>

#define SKETCH_NAME "Kattlucka #21"
#define SKETCH_MAJOR_VER "1"
#define SKETCH_MINOR_VER "0"

#define PRIMARY_CHILD_ID 3

#define UT_PIN 2   // Arduino Digital I/O pin for button/reed switch
#define IN_PIN 3 // Arduino Digital I/O pin for button/reed switch

// Change to V_LIGHT if you use S_LIGHT in presentation below
MyMessage msg(PRIMARY_CHILD_ID, V_TRIPPED);

bool value = 0;

//=========================
// BATTERY MEASURER
// VOLTAGE DIVIDER SETUP
// 1M, 470K divider across battery and using internal ADC ref of 1.1V
// Sense point is bypassed with 0.1 uF cap to reduce noise at that point
// ((1e6+470e3)/470e3)*1.1 = Vmax = 3.44 Volts
// 3.44/1023 = Volts per bit = 0.003363075
#define VBAT_PER_BITS 0.003363075
#define VMIN 2.8                                  //  Vmin (radio Min Volt)=1.9V (564v)
#define VMAX 3.2                                //  Vmax = (2xAA bat)=3.0V (892v) (IKEA rechargables 2.5)  
int batteryPcnt = 0;                              // Calc value for battery %
int batLoop = 0;                                  // Loop to help calc average
int batArray[3];                                  // Array to store value for average calc.
int BATTERY_SENSE_PIN = A0;                       // select the input pin for the battery sense point
//=========================

void setup()
{

pinMode(UT_PIN, INPUT);      
pinMode(IN_PIN, INPUT);    

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

}

void presentation()
{
	// Send the sketch version information to the gateway and Controller
	sendSketchInfo(SKETCH_NAME, SKETCH_MAJOR_VER "." SKETCH_MINOR_VER);

	// Register binary input sensor to sensor_node (they will be created as child devices)
	// You can use S_DOOR, S_MOTION or S_LIGHT here depending on your usage.
	// If S_LIGHT is used, remember to update variable type you send in. See "msg" above.
	present(PRIMARY_CHILD_ID, S_DOOR);
}

// Loop will iterate on changes on the BUTTON_PINs
void loop()
{

Serial.print("Läser UTPIN: ");Serial.println(digitalRead(UT_PIN));
Serial.print("Läser INPIN: ");Serial.println(digitalRead(IN_PIN));

if (digitalRead(UT_PIN) == 1){
    send(msg.set(value==HIGH));
}


if (digitalRead(IN_PIN) == 1){
    send(msg.set(value==LOW));
}

 // BATTERY MEASURER
  MeasureBattery();

wait(4000);

	// Sleep until something happens with the sensor
  sleep(UT_PIN-2, RISING, IN_PIN-2, RISING, 0);
	
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
    batteryPcnt = (batArray[0] + batArray[1] + batArray[2] + batArray[3]);
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
