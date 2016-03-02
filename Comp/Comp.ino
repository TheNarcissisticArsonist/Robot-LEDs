//Include the NeoPixel library
#include <Adafruit_NeoPixel.h>

//Include the I2C library
#include <Wire.h>

//Used throughout the program to reference the number of LEDS on the strand
//This is completely modular -- any number works
#define lNUMLEDS 80
#define rNUMLEDS 80

//This is the wheel diameter of the robot's main drive wheels
#define WHEEL_DIAMETER 6

//Distance between two leds. Please keep this constant. Pretty please?
#define LED_PITCH 0.6875

//Led data pins
#define LEFT_LED_PIN 13
#define RIGHT_LED_PIN 12

//I2C address
#define I2C_ADDRESS 8

//These are used for calculating dT
long startTime;
long currentTime;
long lastTime;

const int minTickTime = 1; //milliseconds. If dT is less than this, nothing happens

//Used for low voltage problems with the robots
bool lowVoltage = false;
const double lowVoltageBrightnessDrop = 5.0;

const int idleStateColor[] = {0, 42, 255}; //Columbia Blue (about)
double currentBrightnessConstant = 1.0; //0 to 1
const double brightnessIncreasingMultiplicativeFactor = 1.01; //Determines the speed at which it gets brighter/dimmer
const double minBrightnessConstant = 0.01;
bool increasing;

const int driveStateColor[] = {0, 42, 255}; //More or less Columbia Blue
const int driveModeSpacing = 4; //Space between the lit LEDs during drive mode

//2D arrays storing information about what the LED state is
//Instead of messing with commands in library, just update this array
//and call updateLEDs() to display it on the strand
//Index 1 is the LED, Index 2 is [RR, GG, BB]
int lLED[lNUMLEDS][3] = {};
int rLED[rNUMLEDS][3] = {};

enum states {
  IDLE_STATE,       //Do this before and after the match (detect when voltage is less than some constant)
  DRIVE_STATE,      //Do this while the robot is driving around
  SHOOTING_STATE,   //Do this when the wheel is active
  OFF_STATE         //If there's critically low battery, shut it down
};

//Initialize the two Adafruit_NeoPixel objects, one for each side
Adafruit_NeoPixel lStrip = Adafruit_NeoPixel(lNUMLEDS, LEFT_LED_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel rStrip = Adafruit_NeoPixel(rNUMLEDS, RIGHT_LED_PIN, NEO_GRB + NEO_KHZ800);

//Number of bytes received from RoboRIO
#define SIGNAL_LENGTH 5

//Bytes from RoboRIO are stored here
char dataFromRoboRIO[SIGNAL_LENGTH];

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
    for(i=0; i<rNUMLEDS; ++i) {
      rStrip.setPixelColor(i, rLED[i][0], rLED[i][1], rLED[i][2]);
    }
  }
  lStrip.show();
  rStrip.show();
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

void idleState(int dT) {
  int i, j;
  if(currentBrightnessConstant >= 1.0) {
    increasing = false;
  }
  else if(currentBrightnessConstant <= minBrightnessConstant) {
    increasing = true;
  }
  if(increasing) {
    currentBrightnessConstant = currentBrightnessConstant*brightnessIncreasingMultiplicativeFactor;
  }
  else {
    currentBrightnessConstant = currentBrightnessConstant/brightnessIncreasingMultiplicativeFactor;
  }
  for(j=0; j<3; ++j) {
    for(i=0; i<lNUMLEDS; ++i) {
      lLED[i][j] = idleStateColor[j]*currentBrightnessConstant;
    }
    for(i=0; i<rNUMLEDS; ++i) {
      rLED[i][j] = idleStateColor[j]*currentBrightnessConstant;
    }
  }
  updateLEDs();
}
void driveState(int dT) {}
void shootingState(int dT) {}

void decodeI2C(int numBytes) {
  int i = 0;
  while(Wire.available() > 0) {
    dataFromRoboRIO[i] = Wire.read();
    ++i;
  }
}

void setup() {
  int i;

  lStrip.begin(); //Start the left and right led strips
  rStrip.begin();
  clearStrips(); //Make them blank

  startTime = millis();
  currentTime = startTime;
  lastTime = startTime;

  Wire.begin(I2C_ADDRESS);
  Wire.onReceive(decodeI2C);

  for(i=0; i<SIGNAL_LENGTH; ++i) {
    dataFromRoboRIO[i] = 0;
  }
}

void loop() {
  //Declare variables
  int state;
  int dT;

  //Deal with time stuff
  currentTime = millis();
  dT = currentTime - lastTime;

  //Read the state and set up stuff
  if(dataFromRoboRIO[1] == 2) {
    state = OFF_STATE;
  }
  else if(dataFromRoboRIO[0] != 1) {
    state = IDLE_STATE;
  }
  else if(dataFromRoboRIO[4] > 10) {
    state = SHOOTING_STATE;
  }
  else {
    state = DRIVE_STATE;
  }
  if(dataFromRoboRIO[1] > 0) {
    lowVoltage = true;
  }
  else {
    lowVoltage = false;
  }

  //Call the proper function(s)
  if(dT > minTickTime) {
    lastTime = currentTime;
    if(state != OFF_STATE) {
      switch(state) {
        case IDLE_STATE:
          idleState(dT);
          break;
        case DRIVE_STATE:
          driveState(dT);
          break;
        case SHOOTING_STATE:
          shootingState(dT);
          break;
      }
    }
  }
}
