#include <SPI.h>
#include <SoftwareSerial.h>

//Software Serial for RS232 to TTL board, define output pins
SoftwareSerial mySerial(6, 7); // RX, TX

String tvPower = "00"; //Power 00=off 01=on
int tvVolume = 0; //Volume
const String tvid = "01"; //The ID you set in your TV
unsigned long previousMillis = 0; // last time update
int messageToSend = 0;

void setup()  
{   
  //Software Serial for RS232 to TTL board, begin
  mySerial.begin(9600);
  Serial.begin(115200);  
  Serial.print("Setup done");
mySerial.println("P");
} 


void loop() { // run over and over
  if (mySerial.available()) {
    Serial.print("I recieved: ");
    Serial.write(mySerial.read());
  }
  if (Serial.available()) {
  Serial.print("Send");
    mySerial.write(Serial.read());
  }
}

