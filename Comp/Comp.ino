//Include the neopixel library
#include <Adafruit_NeoPixel.h>

//Used throughout the program to reference the number of LEDS on the strand
//This is completely modular -- any number works
#define lNUMLEDS 80
#define rNUMLEDS 80

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

void updateLEDs() { //This is a general purpose function to make everything easier. Instead of
                    //having to deal with the library's functions, I can do everything throughout
                    //the arrays I defined above, and then just update when necessary.
  int i, j;
  if(lowVoltage) { //If the robot battery has low voltage, drop the brightness to save power
    for(i=0; i<lNUMLEDS; ++i) { //For each of the leds on the left side
      for(j=0; j<3; ++j) {
        lLED[i][j] = static_cast<int>(static_cast<double>(lLED[i][j])/lowVoltageBrightnessDrop); //The static_casts probably aren't necessary, but I want to make sure the types are correct
      }
      lStrip.setPixelColor(i, lLED[i][0], lLED[i][1], lLED[i][2]); //Update the left strand pixels
    }
    for(i=0; i<rNUMLEDS; ++i) { //For each of the leds on the right side
      for(j=0; j<3; ++j) {
        rLED[i][j] = static_cast<int>(static_cast<double>(rLED[i][j])/lowVoltageBrightnessDrop); //The static_casts probably aren't necessary, but I want to make sure the types are correct
      }
      rStrip.setPixelColor(i, rLED[i][0], rLED[i][1], rLED[i][2]); //Update the right strand pixels
    }
  }
  else { //If voltage isn't low, FULL BRIGHTNESS :O
    for(i=0; i<lNUMLEDS; ++i) {
      lStrip.setPixelColor(i, lLED[i][0], lLED[i][1], lLED[i][2]);
    }
    for(j=0; j<rNUMLEDS; ++j) {
      rStrip.setPixelColor(i, rLED[i][0], rLED[i][1], rLED[i][2]);
    }
  }
}

void clearStrips() { //Basically turn off the strips by setting all the leds to RGB [0, 0, 0]
  int i, j;
  for(j=0; j<3; ++j) { //For the R value, then the G value, then the B value
    for(i=0; i<lNUMLEDS; ++i) { //For all leds on the left
      lLED[i][j] = 0;
    }
    for(i=0; i<rNUMLEDS; ++i) { //For all leds on the right
      rLED[i][j] = 0;
    }
  } //Ordering it like this should make it slightly faster, which doesn't hurt anything
  updateLEDs();
}

void idleState() {}
void driveState(double rawAnalog1, double rawAnalog2) {}
void shooterOnState(double rawAnalog1, double rawAnalog2) {}
void shooterOffState(double rawAnalog1, double rawAnalog2) {}


void setup() {
  lStrip.begin();
  rStrip.begin();
  clearStrips();
}
void loop() {
  int state, stateRead;


}
