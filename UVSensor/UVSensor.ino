// Enable debug prints to serial monitor
#define MY_DEBUG 

// Enable and select radio type attached
#define MY_RADIO_NRF24
//#define MY_RADIO_RFM69
#define MY_NODE_ID 19

#include <SPI.h>
#include <MySensors.h>  

#define UV_PIN 2
#define CHILD_ID_UV 0

#define         READ_SAMPLE_INTERVAL         (10)    //define how many samples you are going to take in normal operation
#define         READ_SAMPLE_TIMES            (5)     //define the time interal(in milisecond) between each samples in  
#define         VREF 3.3f

#define CHILD_ID_LIGHT 0
#define LIGHT_SENSOR_ANALOG_PIN 1

unsigned long SLEEP_TIME = 600000; // Sleep time between reads (in milliseconds)
#define SKETCH_NAME "UV #19"                // Change to a fancy name you like
#define SKETCH_VERSION "1.3"                    // Your version

MyMessage uvMsg(CHILD_ID_UV, V_UV);
MyMessage msg(CHILD_ID_LIGHT, V_LIGHT_LEVEL);

int lastLightLevel;
int exValue [12] = { 50, 227, 318, 408, 503, 606, 696, 795, 881, 976, 1079, 1170};

float last_UVIndex = 0;
uint16_t uvIndexValue [12] = { 50, 227, 318, 408, 503, 606, 696, 795, 881, 976, 1079, 1170}; 

//=========================
// BATTERY VOLTAGE DIVIDER SETUP
// 1M, 470K divider across battery and using internal ADC ref of 1.1V
// Sense point is bypassed with 0.1 uF cap to reduce noise at that point
// ((1e6+470e3)/470e3)*1.1 = Vmax = 3.44 Volts
// 3.44/1023 = Volts per bit = 0.003363075
#define VBAT_PER_BITS 0.003363075  
#define VMIN 1.9                                  //  Vmin (radio Min Volt)=1.9V (564v)
#define VMAX 3.0                                  //  Vmax = (2xAA bat)=3.0V (892v)
int batteryPcnt = 0;                              // Calc value for battery %
int batLoop = 0;                                  // Loop to help calc average
int batArray[3];                                  // Array to store value for average calc.
int BATTERY_SENSE_PIN = A0;                       // select the input pin for the battery sense point
//=========================

void presentation()  {
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo(SKETCH_NAME, SKETCH_VERSION);

  // Register all sensors to gateway (they will be created as child devices)
  present(CHILD_ID_UV, S_UV);
  present(CHILD_ID_LIGHT, S_LIGHT_LEVEL);
  
}

void setup()  {
analogReference(INTERNAL);             // For battery sensing
pinMode(D4, OUTPUT);
}

void loop()
{
  
  
  //ANALOG PINS FOR UV/LIGHT/BAT A0, A1, A2 ???

  //Set power on digital pin for lightsensor.
  digitalWrite(D4 , HIGH);
  //Read UV
  sendUVIRMeasurements(true);
  wait(500);
  //Read Light
  readLightLevel();
  wait(500);
  //Read batteries
  batM();
  //Set digital pin low to save power.
  digitalWrite(D4 , LOW);
  //Sleep!
  sleep(SLEEP_TIME);
  
}

void readLightLevel()      
{     
  int lightLevel = (1023-analogRead(LIGHT_SENSOR_ANALOG_PIN))/10.23; 
  Serial.print("Light: ");
  Serial.println(lightLevel);
  if (lightLevel != lastLightLevel) {
      send(msg.set(lightLevel));
      lastLightLevel = lightLevel;
  }
}

float UVRead()
{
  int i;
  float rs = 0;

  for (i = 0; i < READ_SAMPLE_TIMES; i++) {
    rs += analogRead(UV_PIN);
    delay(READ_SAMPLE_INTERVAL);
  }

  rs = rs / READ_SAMPLE_TIMES;

  return rs;
}

float GetUVIndexFromVoltage(float voltage)
{
  float UV = 0;
  int i;
  for (i = 0; i < 12; i++)
  {
    if (voltage <= uvIndexValue[i])
    {
      UV = float(i);
      break;
    }
  }

  //calculate 1 decimal if possible
  if (i > 0) {
    float vRange = float(uvIndexValue[i] - uvIndexValue[i - 1]);
    float vCalc = voltage - uvIndexValue[i - 1];
    UV += (1.0f / vRange) * vCalc - 1.0f;
  }
  return UV;
}

void sendUVIRMeasurements(bool force)
{
  bool tx = force;

  float sensorValue = UVRead();
  float voltage = sensorValue * (VREF / 1023.0f) * 1000.0f; //mV
  if (voltage > 1170)
    voltage = 1170;
  //Serial.print("UV Analog reading: ");
  //Serial.println(voltage,2);
  float UVIndex = GetUVIndexFromVoltage(voltage);

  float diffUV = abs(last_UVIndex - UVIndex);
#ifdef MY_DEBUG
  Serial.print(F("diffUV :")); Serial.println(diffUV);
#endif
  if (diffUV > 0.1) tx = true;

  if (!tx)
    return;

  last_UVIndex = UVIndex;

  Serial.print("UV: ");
  Serial.println(UVIndex, 2);
  send(uvMsg.set(UVIndex,2));
  
  //Avarage 
  // raUV.addValue(UVIndex);
  // send(msgUV.set(raUV.getAverage(), 2));
}

void batM() //The battery calculations
{
   delay(500);
   // Battery monitoring reading
   int sensorValue = analogRead(BATTERY_SENSE_PIN);    
   delay(500);
   
   // Calculate the battery in %
   float Vbat  = sensorValue * VBAT_PER_BITS;
   int batteryPcnt = static_cast<int>(((Vbat-VMIN)/(VMAX-VMIN))*100.);
   Serial.print("Battery percent: "); Serial.print(batteryPcnt); Serial.println(" %");  
   
   // Add it to array so we get an average of 3 (3x20min)
   batArray[batLoop] = batteryPcnt;
  
   if (batLoop > 2) {  
     batteryPcnt = (batArray[0] + batArray[1] + batArray[2] + batArray[3]);
     batteryPcnt = batteryPcnt / 3;
 
   if (batteryPcnt > 100) {
     batteryPcnt=100;
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
