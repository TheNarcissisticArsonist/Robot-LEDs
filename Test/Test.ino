#include <Adafruit_NeoPixel.h>

#define NUMLEDS 16

const int ledPin = 13;

int leds[NUMLEDS][3] = {};
int interval = 4;
int offset = 0;
int initialTimeDelay = 100;
int timeDelay = initialTimeDelay;
int timeDelayIncrement = 1;
int minTimeDelay = 25;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMLEDS, ledPin, NEO_GRB + NEO_KHZ800);

void updateLEDs()
{
  int i, j;

  for (i = 0; i < NUMLEDS; ++i)
  {
    strip.setPixelColor(i, leds[i][0], leds[i][1], leds[i][2]);
  }
  strip.show();
}

void setup()
{
  int i, j;

  strip.begin();
  strip.show();
  updateLEDs();
}

void loop()
{
  int i;

  for (i = 0; i < sizeof(leds) / sizeof(leds[0]); ++i)
  {
    if ((i + offset) % interval == 0)
    {
      leds[i][0] = 0;
      leds[i][1] = 0;
      leds[i][2] = 255;
    }
    else
    {
      leds[i][0] = 0;
      leds[i][1] = 0;
      leds[i][2] = 0;
    }
  }

  ++offset;

  updateLEDs();
  delay(timeDelay);
  if (timeDelay > minTimeDelay)
  {
    timeDelay -= timeDelayIncrement;
  }
  else {
    timeDelay = minTimeDelay;
  }
}

