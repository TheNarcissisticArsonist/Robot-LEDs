#include <Adafruit_NeoPixel.h>

#define NUMLEDS 16

const int ledPin = 13;
const int statePin = 0;
const int leftPin = 1;
const int rightPin = 2;

bool lowVoltage = false;
const double lowVoltageBrightnessDrop = 5.0;

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

  if(lowVoltage)
  {
    for(i=0; i<NUMLEDS; ++i)
    {
      for(j=0; j<3; ++j)
      {
        leds[i][j] = static_cast<int>(static_cast<double>(leds[i][j])/lowVoltageBrightnessDrop);
      }
    }
  }

  for (i=0; i<NUMLEDS; ++i)
  {
    strip.setPixelColor(i, leds[i][0], leds[i][1], leds[i][2]);
  }
  strip.show();
}

/*void stripClear() {
  for(int i=0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
  }
  strip.show();
}

void explode() {
  for (int i = 0; i<strip.numPixels(); i++) {
    if (i < 12) {
      strip.setPixelColor(i, strip.Color(255, 0, 0));
      } else {
      strip.setPixelColor(i, strip.Color(80, 80, 80));
      }
    }
    strip.show();
    delay(70);
    strip_clear();
    for (int i = 0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(255, 255, 255));
    }
    strip.show();
    delay(70);
  }
} some useful functions*/

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
  pinMode(statePin, INPUT);
  pinMode(leftPin, INPUT);
  pinMode(rightPin, INPUT);
}

void loop()
{
  int state;
  int stateRead = analogRead(statePin);
  //Data read from RoboRIO goes here
  switch(stateRead) {
    case 1:
      state = SHOOTER_ON_STATE;
      break;
    case 2:
      state = SHOOTER_OFF_STATE;
      break;
    case 3:
      lowVoltage = true;
    default:
      state = IDLE_STATE;
      break;
  }

  //State machine from here on out...
  if(state == IDLE_STATE)
  {
    //analogRead from leftPin and rightPin for wheel speeds
  }
  else if(state == SHOOTER_ON_STATE)
  {
    //analogRead from leftPin for shooter speeds
  }
  else if(state == SHOOTER_OFF_STATE)
  {
    //analogRead from leftPin for shooter speeds
  }
}
