#include <Adafruit_NeoPixel.h>

#define NUMLEDS 16

const int ledPin = 13;

int leds[NUMLEDS][3] = {};

enum states {
  IDLE_STATE,         //Driving around, analog line 2 represents speed
  SHOOTER_ON_STATE,   //Shooter just got activated, pattern TBD. Analog line 2 could represent shooter wheel speed?
  SHOOTER_OFF_STATE   //Shooter just got deactivated, pattern TBD. Analog line 2 could represent shooter wheel speed?
};

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMLEDS, ledPin, NEO_GRB + NEO_KHZ800);

void updateLEDs()
{
  int i, j;

  for (i=0; i<NUMLEDS; ++i)
  {
    strip.setPixelColor(i, leds[i][0], leds[i][1], leds[i][2]);
  }
  strip.show();
}

void idleState(double analog1, double analog2)
{
  //
}
void shooterOnState(double analog1)
{
  //
}
void shooterOffState(double analog1)
{
  //
}

void setup()
{
  int i, j;
  for(i=0; i<NUMLEDS; ++i)
  {
    for(j=0; j<3; ++j)
    {
      leds[i][j] = 0;
    }
  }
  strip.begin();
  strip.show();
  updateLEDs();
}

void loop()
{
  int state = IDLE_STATE;
  //Data read from RoboRIO goes here


  //State machine from here on out...
  if(state == IDLE_STATE)
  {
    //
  }
  else if(state == SHOOTER_ON_STATE)
  {
    //
  }
  else if(state == SHOOTER_OFF_STATE)
  {
    //
  }
}
