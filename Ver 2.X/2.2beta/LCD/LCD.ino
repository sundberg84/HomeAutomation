// Enable debug prints to serial monitor
#define MY_DEBUG

// Enable and select radio type attached
#define MY_RADIO_NRF24
//#define MY_RADIO_RFM69

#define MY_REPEATER_FEATURE                   // Repeater, remove if you dont want repeater mode.
#define MY_NODE_ID 105                        // Node ID, or remove to have AUTO.
#define MY_PARENT_NODE_ID 100
#define SKETCH_NAME "LCD #105"                // Change to a fancy name you like
#define SKETCH_VERSION "1.2"

#include <SPI.h>
#include <MyConfig.h>
#include <MySensors.h>
//#include <Time.h>
#include <DallasTemperature.h>
#include <OneWire.h>

#define _Digole_Serial_I2C_
#include <DigoleSerial.h>

//I2C setup
//(can be used differently! - Se datasheet for digole screen)
//I2C:Arduino UNO: SDA (data line) is on analog input pin 4, and SCL (clock line) is on analog input pin 5 on UNO and Duemilanove
#include <Wire.h>
DigoleSerialDisp mydisp(&Wire, '\x27');

//LCD
const unsigned char fonts[] = {6, 10, 18, 51, 120, 123}; //Digole fonts
const char *fontdir[] = {"0\xb0", "90\xb0", "180\xb0", "270\xb0"}; //Digole fonts
long userpresent = 0;                         // variable when user was last present, fades screen.
long dimLcdTime = 10000;                      // how long user is not present before fade screen.
int currentBackLight = 100;                   // Current status backlight of the LCD
int selected = 0;                             // Current selected switch in LCD

//Fetch V_TEXT (Outside temp in my case)
const byte msg_v_Text = 8 ;                 // Text from ControllerLCD
MyMessage textMsg(msg_v_Text, V_TEXT);      // message for Sending Text to Controller
bool newTempAvailable = false ;               // We have a new message available = flag in loop
String temp = "0";                            // Variable got from controller before inserted in LCD
unsigned long lastTempUpdate = -860000UL;     // Last time temp vas updated from controller (every 15min loop)

//Buttons
#define BUTTON_DOWN  3                        // Arduino Digital I/O pin for button/reed switch
#define BUTTON_OK  4                          // Arduino Digital I/O pin for button/reed switch
#define BUTTON_UP  5                          // Arduino Digital I/O pin for button/reed switch
#define ERROR_LED  6                          // Arduino Digital I/O pin for button/reed switch

// ========================================================================================
// === USER NEEDS TO DEFINE BELOW =========================================================
// ========================================================================================
// Define the wirtual switches that should be displayed in the LCD:

// howManyOptions is how many menues you want.
int howManyOptions = 5;

//options[] holds the menunames. (Try to make each array in options[] 15 char long)
String options[] = {"Kök (Belys)    ", "Köksbord lampa", "Fönsterlampor  ", "Sovrumslampor  ", "Godnatt/Larma  "};

// Add one child / options above (Start with 0!)
MyMessage Msg1(0, V_TRIPPED);
MyMessage Msg2(1, V_TRIPPED);
MyMessage Msg3(2, V_TRIPPED);
MyMessage Msg4(3, V_TRIPPED);
MyMessage Msg5(4, V_TRIPPED);

// ========================================================================================
// === USER NEEDS TO DEFINE ABOVE =========================================================
// ========================================================================================

bool sending = false;                       // Variable to determine if LCD/buttons should be locked (during sending)
long lastDebounceTime = 0;                  // The last time the output pin was toggled

//Time
boolean timeReceived = false;
unsigned long lastUpdate = 0, lastRequest = 0;
unsigned long lastClockUpdate = 0;

//Dallas Temp (Internal AC temp measure)
#define COMPARE_TEMP 0                      // Send temperature only if changed? 1 = Yes 0 = No
#define ONE_WIRE_BUS 8                      // Pin where dallase sensor is connected 
#define MAX_ATTACHED_DS18B20 1              // Number of sensors attached
unsigned long SLEEP_TIME = 30000;           // Sleep time between reads (in milliseconds)
OneWire oneWire(ONE_WIRE_BUS);              // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
DallasTemperature sensors(&oneWire);        // Pass the oneWire reference to Dallas Temperature.

float lastTemperature[MAX_ATTACHED_DS18B20];
int numSensors = 0;
boolean receivedConfig = false;
boolean metric = true;
// Initialize temperature message
MyMessage MsgDallas(howManyOptions, V_TEMP);

void presentation() {

  // Send the Sketch Version Information to the Gateway
  sendSketchInfo(SKETCH_NAME, SKETCH_VERSION);

  // present(CHILD_ID, S_DOOR);
  present(msg_v_Text, S_INFO, "LCDInfoFromController");

  int i;
  for (i = 0; i < howManyOptions; i++) {
    //Serial.print("Presenting: "); Serial.println(i);
    present(i, S_DOOR);
    wait(200);
  }
  present(howManyOptions, S_TEMP);
  //Serial.print("Presenting (Temp): "); Serial. println(howManyOptions);
}

void setup() {

  // Set Pinmodes
  pinMode(BUTTON_DOWN, INPUT);
  digitalWrite(BUTTON_DOWN, HIGH);
  pinMode(BUTTON_OK, INPUT);
  digitalWrite(BUTTON_OK, HIGH);
  pinMode(BUTTON_UP, INPUT);
  digitalWrite(BUTTON_UP, HIGH);
  pinMode(ERROR_LED, OUTPUT);
  digitalWrite(ERROR_LED, HIGH);

  // LCD Init
  mydisp.begin();
  wait(1500); //delay
  mydisp.clearScreen();
  wait(1500); //delay
  mydisp.setBackLight(currentBackLight);  //set brightness of backlight.
  mydisp.setMode('C'); //set graphic Drawing Mode to COPY
  mydisp.setRot180();
  mydisp.displayConfig(0);
  mydisp.disableCursor(); //disable cursor, enable cursore use: enableCursor();
  mydisp.setPrintPos(0, 0, _TEXT_);

  // Build Screen
  mydisp.setPrintPos(0, 0, _TEXT_);
  mydisp.setColor(196);
  mydisp.setFont(fonts[2]);
  mydisp.drawStr(0, 0, "Sundberg Control");

  mydisp.setPrintPos(0, 1, _TEXT_);
  mydisp.setColor(1);
  mydisp.setFont(fonts[1]);
  mydisp.print("Startar..."); // =Init...

  mydisp.setPrintPos(0, 4, _TEXT_);
  mydisp.setColor(1);
  mydisp.setFont(fonts[1]);
  mydisp.print("Ber om tiden..."); // =Asking for time...

  requestTime();
  lastRequest = millis();

  wait(100); //Use wait to not block process()

  // Write Time on LCD
  if (timeReceived == true) {
    mydisp.setColor(1);
    mydisp.setFont(fonts[1]);
    mydisp.setPrintPos(15, 4, _TEXT_);
    mydisp.print(hour());
    mydisp.setPrintPos(17, 4, _TEXT_);
    mydisp.print(":");
    mydisp.setPrintPos(18, 4, _TEXT_);
    mydisp.print(minute());
    digitalWrite(ERROR_LED, LOW);
  }

  mydisp.setPrintPos(0, 6, _TEXT_);
  mydisp.setColor(1);
  mydisp.setFont(fonts[1]);
  mydisp.print("Ber om utomhustemp..."); // Ask for V_TEXT child...
  request(msg_v_Text, V_TEXT, 0);                    // request valueOK from controller
  wait(1000); //Use wait to not block process()
  digitalWrite(ERROR_LED, HIGH);

  if (newTempAvailable == true) {
    //serial.println("Ini-temp");
    mydisp.print(temp); // display a char array
    newTempAvailable = false;
  }

  wait(5000);

  //Shift from Init screen and build screen we wants...
  mydisp.clearScreen();
  mydisp.setRot180();

  mydisp.setPrintPos(0, 0, _TEXT_);
  mydisp.setColor(196);
  mydisp.setFont(fonts[2]);
  mydisp.drawStr(0, 0, "Sundberg Control");

  mydisp.setColor(1);
  mydisp.setFont(fonts[2]);

  mydisp.setPrintPos(0, 1, _TEXT_);
  if (hour() < 10) {
    mydisp.print("0");
  }
  mydisp.print(hour());
  mydisp.print(":");
  if (minute() < 10) {
    mydisp.print("0");
  }
  mydisp.print(minute());
  lastClockUpdate = millis();

  mydisp.setFont(fonts[1]);
  mydisp.setPrintPos(10, 2, _TEXT_);
  mydisp.print("Utomhus: ");
  mydisp.print(temp);
  mydisp.print( "C");

  mydisp.setFont(fonts[1]);
  mydisp.setPrintPos(0, 5, _TEXT_);
  mydisp.print("      # ");
  mydisp.print(options[selected - 1]);

  mydisp.setColor(124);
  mydisp.setPrintPos(0, 7, _TEXT_);
  mydisp.setFont(fonts[2]);
  mydisp.print("    ");
  mydisp.print(options[selected]);

  mydisp.setColor(1);
  mydisp.setFont(fonts[1]);
  mydisp.setPrintPos(0, 9, _TEXT_);
  mydisp.print("      # ");
  mydisp.print(options[selected + 1]);

  mydisp.setPrintPos(0, 13, _TEXT_);
  mydisp.setColor(1);
  mydisp.setFont(fonts[1]);
  mydisp.print("    UPP  /  OK  /  NER"); // Buttons

  userpresent = millis();

  //Temp

  // Startup up the OneWire library
  sensors.begin();
  // requestTemperatures() will not block current thread
  sensors.setWaitForConversion(false);
  getTemp();
}

void loop() {
  unsigned long now = millis();

  // If no time has been received yet, request it every 10 second from controller
  // When time has been received, request update every hour
  if ((!timeReceived && (now - lastRequest) > (10UL * 1000UL))
      || (timeReceived && (now - lastRequest) > (60UL * 1000UL * 60UL))) {
    // Request time from controller.
    digitalWrite(ERROR_LED, HIGH);
    requestTime();
    lastRequest = now;
  }

  //IF we are sending we want 2sek "debouce" = User not able to do anything.
  //This to avoid double sendings and overload.
  if (sending) sendingOnOff();

  //Only perform actions if we are not sending anything.
  if (!sending) {

    // Check pressed buttons:
    // Check if button (DOWN) is pressed.
    int valueDOWN = digitalRead(BUTTON_DOWN);
    if (valueDOWN == LOW) {
      if ((millis() - lastDebounceTime) > 200) {
        //User is present (not dim LCD)
        lastDebounceTime = millis();
        userpresent = millis();
        if (currentBackLight < 100) {
          currentBackLight = 100;
          mydisp.setBackLight(currentBackLight);
        }
        setSelected(1);
      }
    }

    // Check if button (OK) is pressed.
    int valueOK = digitalRead(BUTTON_OK);
    if (valueOK == LOW) {

      //If lcd is dimmed, light it up!
      if (currentBackLight < 100) {
        currentBackLight = 100;
        mydisp.setBackLight(currentBackLight);
      }
      else if ((millis() - lastDebounceTime) > 200) {

        sending = true;//Lock buttons and screen for 2 seconds (sending = true)

        switch (selected) {
          case 0:
            send(Msg1.set(true));
            break;
          case 1:
            send(Msg2.set(true));
            break;
          case 2:
            send(Msg3.set(true));
            break;
          case 3:
            send(Msg4.set(true));
            break;
          case 4:
            send(Msg5.set(true));
            break;
        }
        //Change text from Green to Blue to indicate it was pressed
        mydisp.setColor(7);
        mydisp.setFont(fonts[1]);
        mydisp.setPrintPos(0, 7, _TEXT_);
        mydisp.setFont(fonts[2]);
        mydisp.print(" >  ");
        mydisp.print(options[selected]);
      }
      lastDebounceTime = millis();
      userpresent = millis();
    }

    // Check if button (UP) is pressed.
    int valueUP = digitalRead(BUTTON_UP);
    if (valueUP == LOW) {
      if ((millis() - lastDebounceTime) > 200) {
        //User is present (not dim LCD)
        lastDebounceTime = millis();
        userpresent = millis();
        if (currentBackLight < 100) {
          currentBackLight = 100;
          mydisp.setBackLight(currentBackLight);
        }
        setSelected(-1);
      }
    }

    if (!newTempAvailable && lastTempUpdate + 900000UL < now) {
      //serial.println("Requesting temp/LCD");
      request(msg_v_Text, V_TEXT, 0); // request tempvalue from controller every 15 min
      digitalWrite(ERROR_LED, HIGH);
      //serial.println("3");
      getTemp();

      // Every 15 min (if user not preset), update the meny depending on users experience
      // === User NEEDS to set these!
      if (currentBackLight < 100) {
        if (hour() < 8) {
          selected = 0;
        }
        if (hour() >= 8 && hour() <= 10) {
          selected = 1;
        }
        if (hour() >= 11 && hour() <= 13) {
          selected = 0;
        }
        if (hour() >= 14 && hour() <= 18) {
          selected = 1;
        }
        if (hour() >= 19 && hour() <= 20) {
          selected = 3;
        }
        if (hour() >= 21) {
          selected = 4;
        }
        setSelected(selected);
      }
    }

    // There has been a new temp fetched from the controller (and for some reason not updated to LCD)
    // This is double... updateTemp() is called already in revieve() - safetynet?
    if (newTempAvailable == true) {
      updateTemp();
    }

    //Update time on LCD 1/min
    if (lastClockUpdate + 60000UL < now) {
      updateClock();
    }

    //Dims the LCD to 10% if user not present.
    if ((userpresent + dimLcdTime < now) && (currentBackLight > 10)) {
      currentBackLight = 10;
      mydisp.setBackLight(currentBackLight);
    }

  }
}

//Here is time recieved from controller
void receiveTime(unsigned long time) {
  digitalWrite(ERROR_LED, LOW);
  // Ok, set incoming time
  setTime(time);
  timeReceived = true;

}

// This is called when a message (Temp) is received
void receive(const MyMessage &message) {
  //serial.println("Incoming Temp");
  digitalWrite(ERROR_LED, LOW);
  if (message.sensor == msg_v_Text) {
    if (message.type == V_TEXT) {                   // Text content
      temp = message.getString();        // copy it in
      temp.remove(temp.indexOf('.') + 2); //Due to bug in 1.6.0, remove if updated!
      newTempAvailable = true ;
      lastTempUpdate = millis();
      updateTemp();
    }
  }
}

//Update clocks to LCD
void updateClock() {
  digitalWrite(ERROR_LED, LOW);
  mydisp.setColor(1);
  mydisp.setFont(fonts[2]);
  mydisp.setPrintPos(0, 1, _TEXT_);
  if (hour() < 10) {
    mydisp.print("0");
  }
  mydisp.print(hour());
  mydisp.print(":");
  if (minute() < 10) {
    mydisp.print("0");
  }
  mydisp.print(minute());
  lastClockUpdate = millis();
}

//Updating LCD with new temp numbers
void updateTemp() {
  digitalWrite(ERROR_LED, LOW);
  mydisp.setFont(fonts[1]);
  mydisp.setPrintPos(10, 2, _TEXT_);
  mydisp.print("Utomhus: ");
  mydisp.print(temp);
  mydisp.print( "C");
  newTempAvailable = false;
}

//Updated LCD with meny scroll when buttons is pressed.
void setSelected(int newSelection) {
  selected += newSelection;
  if (selected < 0) {
    selected = 0;
  }
  if (selected > howManyOptions - 1) {
    selected = howManyOptions - 1;
  }

  //Print LCD
  //Print Line1 (selected-1) (small text)
  mydisp.setColor(1);
  if (selected > 0) {
    mydisp.setFont(fonts[1]);
    mydisp.setPrintPos(0, 5, _TEXT_);
    mydisp.print("                   ");
    mydisp.setPrintPos(0, 5, _TEXT_);
    mydisp.print("      # ");
    mydisp.print(options[selected - 1]);
  }
  else {
    //Upper limit, dont print something
    mydisp.setFont(fonts[1]);
    mydisp.setPrintPos(0, 5, _TEXT_);
    mydisp.print("                     ");
  }

  //Print Line1 (selected) (Big green text)
  mydisp.setFont(fonts[1]);
  mydisp.setColor(124);
  mydisp.setPrintPos(0, 7, _TEXT_);
  mydisp.setFont(fonts[2]);
  mydisp.print("    ");
  mydisp.print(options[selected]);
  mydisp.print("                     ");

  mydisp.setColor(1);
  if (selected < 4) {
    mydisp.setFont(fonts[1]);
    mydisp.setPrintPos(0, 9, _TEXT_);
    mydisp.print("                   ");
    mydisp.setPrintPos(0, 9, _TEXT_);
    mydisp.print("      # ");
    mydisp.print(options[selected + 1]);
  }
  else
  {
    mydisp.setFont(fonts[1]);
    mydisp.setPrintPos(0, 9, _TEXT_);
    mydisp.print("                     ");
  }
}

void sendingOnOff() {

  // Give it a debounce time of 2sek when lcd and buttons is locked.
  if ((millis() - lastDebounceTime) > 2000) {
    switch (selected) {
      case 0:
        send(Msg1.set(false));
        break;
      case 1:
        send(Msg2.set(false));
        break;
      case 2:
        send(Msg3.set(false));
        break;
      case 3:
        send(Msg4.set(false));
        break;
      case 4:
        send(Msg5.set(false));
        break;
    }
    sending = false;
    
    //Change back color on selected to green.
    mydisp.setColor(124);
    mydisp.setFont(fonts[1]);
    mydisp.setPrintPos(0, 7, _TEXT_);
    mydisp.setFont(fonts[2]);
    mydisp.print("    ");
    mydisp.print(options[selected]);
    wait(2000);
  }
}

void getTemp() {

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

