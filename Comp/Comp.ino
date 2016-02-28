//Include the neopixel library
#include <Adafruit_NeoPixel.h>

//Used throughout the program to reference the number of LEDS on the strand
//This is completely modular -- any number works
#define lNUMLEDS 16
#define rNUMLEDS 16

const int lLedPin = 13; //Left side leds
const int rLedPin = 12; //Right side leds
const int statePin = 0; //Used to get state from the RoboRIO (analog)
const int leftPin = 1; //Used to get data from the RoboRIO (analog)
const int rightPin = 2; //Used to get data from the RoboRIO (analog)

//Drop the brightness if the robot starts to lose voltage
bool lowVoltage = false;
const double lowVoltageBrightnessDrop = 5.0;

//2D arrays storing information about what the LED state is
//Instead of messing with commands in library, just update this array
//and call updateLEDs() to display it on the strand
//Index 1 is the LED, Index 2 is 8-bit [R, G, B]
int lLED[lNUMLEDS][3] = {};
int rLED[rNUMLEDS][3] = {};

enum states {
  IDLE_STATE,         //Do this before and after the match (detect when voltage is less than some constant)
  DRIVE_STATE,        //Do this while the robot is driving around
  SHOOTER_ON_STATE,   //Do this once the shooter gets turned on
  SHOOTER_OFF_STATE   //Do this when the shooter gets turned off
};

//Initialize the two Adafruit_NeoPixel objects, one for each side
Adafruit_NeoPixel lStrip = Adafruit_NeoPixel(lNUMLEDS, lLedPin, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel rStrip = Adafruit_NeoPixel(rNUMLEDS, rLedPin, NEO_GRB + NEO_KHZ800);

void updateLEDs() {
  int i, j;
  if(lowVoltage) {
    for(i=0; i<lNUMLEDS; ++i) {
      for(j=0; j<3; ++j) {
        lLED[i][j] = static_cast<int>(static_cast<double>(lLED[i][j])/lowVoltageBrightnessDrop);
      }
      lStrip.setPixelColor(i, lLED[i][0], lLED[i][1], lLED[i][2]);
    }
    for(i=0; i<rNUMLEDS; ++i) {
      for(j=0; j<3; ++j) {
        rLED[i][j] = static_cast<int>(static_cast<double>(rLED[i][j])/lowVoltageBrightnessDrop);
      }
      rStrip.setPixelColor(i, rLED[i][0], rLED[i][1], rLED[i][2]);
    }
  }
  else {
    for(i=0; i<lNUMLEDS; ++i) {
      lStrip.setPixelColor(i, lLED[i][0], lLED[i][1], lLED[i][2]);
    }
    for(j=0; j<rNUMLEDS; ++j) {
      rStrip.setPixelColor(i, rLED[i][0], rLED[i][1], rLED[i][2]);
    }
  }
}

void setup() {}
void loop() {}
