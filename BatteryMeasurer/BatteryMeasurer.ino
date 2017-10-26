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



void setup() {
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
  Serial.print("Battert Percent 25%/50%/75% should be: "); Serial.print(((VMAX - VMIN) / 4) + VMIN); Serial.print("/"); Serial.print(((VMAX - VMIN) / 2) + VMIN); Serial.print("/"); Serial.println(VMAX - ((VMAX - VMIN) / 4));
  delay(1000);
  int sensorValue = analogRead(BATTERY_SENSE_PIN);
  delay(50);
  float Vbat  = sensorValue * VBAT_PER_BITS;
  int batteryPcnt = static_cast<int>(((Vbat - VMIN) / (VMAX - VMIN)) * 100.);
  Serial.print("Current battery are measured to (please confirm!): "); Serial.print(batteryPcnt); Serial.print(" % - Or "); Serial.print(Vbat); Serial.println(" Volts");
  //=========================
}



void loop() {

  //=========================
  // BATTERY MEASURER
  MeasureBattery();
  //=========================
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
  Serial.print("Battery percent: "); Serial.print(batteryPcnt); Serial.print(" %"); Serial.print("Battery Voltage: "); Serial.print(Vbat); Serial.println(" Volts");

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
