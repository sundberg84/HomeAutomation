#include <SPI.h>

// Enable debug prints to serial monitor
#define MY_DEBUG

// Enable and select radio type attached
//#define MY_RADIO_NRF24
#define MY_RADIO_RFM69
#define MY_RFM69_FREQUENCY RFM69_433MHZ

// Enable serial gateway
#define MY_GATEWAY_SERIAL

// Define a lower baud rate for Arduino's running on 8 MHz (Arduino Pro Mini 3.3V & SenseBender)
#if F_CPU == 8000000L
#define MY_BAUD_RATE 38400
#endif

#include <MySensors.h>

void setup()
{
    // Setup locally attached sensors
}

void presentation()
{
    // Present locally attached sensors
}

void loop()
{
    // Send locally attached sensor data here
}
