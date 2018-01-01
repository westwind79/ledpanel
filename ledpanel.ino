

#define LED_PIN     6
#define BRIGHTNESS  96
// Global maximum brightness value, maximum 255
#define MAXBRIGHTNESS 120
#define STARTBRIGHTNESS 65

#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define hueTime 6
#define DELAY_PATTERNS 20
#define EEPROMDELAY 2000

#include <FastLED.h>
#include <LEDMatrix.h>
#include <EEPROM.h>
#include "palettes.h"
#include "messages.h"
#include "font.h"

#include "XYmap.h"
#include "utils.h"

#include "effects.h"


void setup() {
  delay(3000);
    // check to see if EEPROM has been used yet
  // if so, load the stored settings
  byte eepromWasWritten = EEPROM.read(0);
  if (eepromWasWritten == 99) {
    currentEffect = EEPROM.read(1);
    autoCycle = EEPROM.read(2);
    currentBrightness = EEPROM.read(3);
  }


  LEDS.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  //LEDS.setBrightness(STARTBRIGHTNESS);
  FastLED.setBrightness( scale8(currentBrightness, MAXBRIGHTNESS) );
  // Also, RainbowColors_p, RainbowStripeColors_p, OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p
  resetvars();

  X = Xorig;
  Y = Yorig;
    // Initialize our coordinates to some random values
  x = random16();
  y = random16();
  z = random16();
}

typedef void (*SimplePatternList[])();

SimplePatternList gPatterns = {
  //  circnoise1,
  //  circnoise2,
 // fireworks,

  // scrollTextZero,
  // scrollTextOne,
  // scrollTextTwo,
  colorwaves,
  amazing,
  slantBars2,
  tungsten,
  overcastSky,
  colorSnake,
  amazingNoise,
  starfield,
  slantBars,
  radiate2,
  rider,
  plasma2,
  snow,
  Fire2012WithPalette,
  three_sinVert,
  three_sin,
  vertThreeSine,
  threeSine,
  ripple,
  serendipitous,
  inoise8_fire,
  blendme,
  crazy2,
  spinPlasma,
  bpm,
  plasma,
  confetti,
  colorFill,
  drip,
  sinelon,
  radiate,
  rainbowStripes,
  fun1,
  rain,
  juggle
};


void loop() {
  currentMillis = millis(); // save the current timer value

  // Call the current pattern function once, updating the 'leds' array
  gPatterns[gCurrentPatternNumber]();

  // send the 'leds' array out to the actual LED strip

  // do some periodic updates
  EVERY_N_MILLISECONDS( hueTime ) {
    gHue++;  // slowly cycle the "base color" through the rainbow
  }
  EVERY_N_SECONDS( DELAY_PATTERNS ) {
    nextPattern();  // change patterns periodically
  }

//  updateButtons();          // read, debounce, and process the buttons
//  doButtons();              // perform actions based on button state
  checkEEPROM();            // update the EEPROM if necessary
  LEDS.show();
  // insert a delay to keep the framerate modest
  LEDS.delay(1000 / FRAMES_PER_SECOND);

}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern() {
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}
