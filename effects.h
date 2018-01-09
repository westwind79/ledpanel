uint8_t xd[NUM_LEDS];
uint8_t yd[NUM_LEDS];
void circnoise1() {

  uint8_t scale = 210;                               // the "zoom factor" for the noise

  for (uint16_t i = 0; i < NUM_LEDS; i++) {

    uint16_t shift_x = beatsin8(17);                  // the x position of the noise field swings @ 17 bpm
    uint16_t shift_y = millis() / 100;                // the y position becomes slowly incremented

    uint32_t real_x = (xd[i] + shift_x) * scale;       // calculate the coordinates within the noise field
    uint32_t real_y = (yd[i] + shift_y) * scale;       // based on the precalculated positions

    uint8_t noise = inoise16(real_x, real_y, 4223) >> 8;           // get the noise data and scale it down

    uint8_t index = noise * 3;                        // map led color based on noise data
    uint8_t bri   = noise;

    leds[i] = ColorFromPalette(currentPalette, index, bri, LINEARBLEND);   // With that value, look up the 8 bit colour palette value and assign it to the current LED.
  }
}

// void circnoise2() {

//   uint8_t scale = 250;                               // the "zoom factor" for the noise

//   for (uint16_t i = 0; i < NUM_LEDS; i++) {

//     uint16_t shift_x = millis() / 10;                 // x as a function of time
//     uint16_t shift_y = 0;

//     uint32_t real_x = (xd[i] + shift_x) * scale;       // calculate the coordinates within the noise field
//     uint32_t real_y = (yd[i] + shift_y) * scale;       // based on the precalculated positions

//     uint8_t noise = inoise16(real_x, real_y, 4223) >> 8;           // get the noise data and scale it down

//     uint8_t index = noise * 3;                        // map led color based on noise data
//     uint8_t bri   = noise;

//     leds[i] = ColorFromPalette(currentPalette, index, bri, LINEARBLEND);   // With that value, look up the 8 bit colour palette value and assign it to the current LED.

//   }
// }
#define MAXCHANGES 16
// Frequency, thus the distance between waves:
#define MUL1 7
#define MUL2 6
#define MUL3 5

void fun1() {
  FastLED.clear();
  if (effectInit == false) {
    effectInit = true;
    effectDelay = 0;
    FastLED.clear();
    // selectRandomPalette();
    fadingActive = true;
  }
  static int wave1 = 1;                                                // Current phase is calculated.
  static int wave2 = 2;
  static int wave3 = 3;

  static CRGBPalette16 currentPalette(CRGB::Black);
  static CRGBPalette16 targetPalette(PartyColors_p);

 
    nblendPaletteTowardPalette( currentPalette, targetPalette, MAXCHANGES);

    wave1 += beatsin8(10, -4, 4);
    wave2 += beatsin8(15, -2, 2);
    wave3 += beatsin8(12, -3, 3);

    for (int k = 0; k < NUM_LEDS; k++) {
      uint8_t tmp = sin8(MUL1 * k + wave1) + sin8(MUL2 * k + wave2) + sin8(MUL3 * k + wave3);
      leds[k] = ColorFromPalette(currentPalette, tmp, 255);
    }
  

  uint8_t secondHand = (millis() / 1000) % 60;
  static uint8_t lastSecond = 99;

  if ( lastSecond != secondHand) {
    lastSecond = secondHand;
    CRGB p = CHSV( HUE_PURPLE, 255, 255);
    CRGB g = CHSV( HUE_GREEN, 255, 255);
    CRGB u = CHSV( HUE_BLUE, 255, 255);
    CRGB b = CRGB::Black;
    CRGB w = CRGB::White;

    switch (secondHand) {
      case  0: targetPalette = RainbowColors_p; break;
      case  5: targetPalette = CRGBPalette16( u, u, b, b, p, p, b, b, u, u, b, b, p, p, b, b); break;
      case 10: targetPalette = OceanColors_p; break;
      case 15: targetPalette = CloudColors_p; break;
      case 20: targetPalette = LavaColors_p; break;
      case 25: targetPalette = ForestColors_p; break;
      case 30: targetPalette = PartyColors_p; break;
      case 35: targetPalette = CRGBPalette16( b, b, b, w, b, b, b, w, b, b, b, w, b, b, b, w); break;
      case 40: targetPalette = CRGBPalette16( u, u, u, w, u, u, u, w, u, u, u, w, u, u, u, w); break;
      case 45: targetPalette = CRGBPalette16( u, p, u, w, p, u, u, w, u, g, u, w, u, p, u, w); break;
      case 50: targetPalette = CloudColors_p; break;
      case 55: targetPalette = CRGBPalette16( u, u, u, w, u, u, p, p, u, p, p, p, u, p, p, w); break;
      case 60: break;
    }
  }
}

void starfield() { 
  if (effectInit == false) {
    effectInit = true;
    effectDelay = 0;
    //fadingActive = true;
  }
  CRGB brightnessSubtractor;
 // float starSpeed[NUM_LEDS];
  int fadeSpeed = 100;
  int decayRate = 260;
  int starRate = 1000; // stars per second
  brightnessSubtractor = CHSV(0, 100, fadeSpeed);

  if (millis() % (2000 / starRate) == 0) {
    leds[random(NUM_LEDS)] = CHSV(0, 0, 100); // create white stars
  }

  if (millis() % (2000 / decayRate) == 0)
  {
    for (int i = 0; i < NUM_LEDS; i++)
    {
      leds[i] -= brightnessSubtractor;
    }
  }
}


#define rainDir2 1
void rain() {
  FastLED.clear();
  if (effectInit == false) {
    effectInit = true;    
     FastLED.clear();
    effectDelay = 45;
    selectRandomPalette();
    fadingActive = true;
  }
  
  scrollArray(rainDir2);
  byte randPixel = random8(kMatrixHeight);
  for (byte y = 0; y < kMatrixHeight; y++) leds[XY((kMatrixWidth - 1) * rainDir2, y)] = CRGB::Black;
  leds[XY((kMatrixWidth - 1)*rainDir2, randPixel)] = CHSV(gHue, 255, 255);
}

void rainbowStripes() {
  FastLED.clear();
    if (effectInit == false) {
      effectInit = true;
       FastLED.clear();
      effectDelay = 50;
      fadingActive = true;
    }
    uint32_t ms = millis();
    int32_t yHueDelta32 = ((int32_t)cos16( ms * (27/1) ) * (350 / kMatrixWidth));
    int32_t xHueDelta32 = ((int32_t)cos16( ms * (39/1) ) * (310 / kMatrixHeight));
    DrawOneFrame( ms / 65536, yHueDelta32 / 32768, xHueDelta32 / 32768);
    FastLED.show();
}

void radiate() {
  FastLED.clear();
  if (effectInit == false) {
    effectInit = true;
     FastLED.clear();
    effectDelay = 45;
    // selectRandomPalette();
    fadingActive = true;
  }
  static byte offset  = 0;
  static int plasVector = 0;

  int xOffset = (cos8(plasVector)-512)/2;
  int yOffset = (sin8(plasVector)-512)/2;

//  int xOffset = 0;
//  int yOffset = 4;

  for (int x = 0; x < kMatrixWidth; x++) {
    for (int y = 0; y < kMatrixHeight; y++) {
      byte color = sin8(sqrt(sq(((float)x - 7.5) * 12 + xOffset) + sq(((float)y - 2) * 12 + yOffset)) + offset);
      leds[XY(x, y)] = ColorFromPalette(currentPalette, color, 255);
    }
  }
  offset++;
  plasVector += 1;
}

void sinelon(){
  FastLED.clear();
  if (effectInit == false) {
    effectInit = true;
    FastLED.clear();
    effectDelay = 0; 
    fadingActive = true;
  }
  
  fadeToBlackBy( leds, NUM_LEDS, 10);
  
  int pos = beatsin16( 13, 0, NUM_LEDS - 1 );
  leds[pos] += CHSV( gHue, 255, 192);
}

void juggle() {
  FastLED.clear();
  if (effectInit == false) {
    effectInit = true;
    FastLED.clear(); 
    effectDelay = 45;
    selectRandomPalette();
    fadingActive = true;
  }
  
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for ( int i = 0; i < 8; i++) {
    leds[beatsin16( i + 7, 0, NUM_LEDS - 1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

void colorFill() {
  FastLED.clear();
  static byte currentColor = 0;
  static byte currentRow = 0;
  static byte currentDirection = 0;

  if (effectInit == false) {
    effectInit = true;
    FastLED.clear(); 
    effectDelay = 0;
    currentColor = 0;
    currentRow = 0;
    currentDirection = 0;
    currentPalette = RainbowColors_p;
    fadingActive = true;
  }
  
  if (!(currentDirection & 1)) {
    effectDelay = 40; // slower since vertical has fewer pixels
    for (byte x = 0; x < kMatrixWidth; x++) {
      byte y = currentRow;
      if (currentDirection == 2) y = kMatrixHeight - 1 - currentRow;
      leds[XY(x, y)] = currentPalette[currentColor];
    }
  }

  if (currentDirection & 1) {
    effectDelay = 20; // faster since horizontal has more pixels
    for (byte y = 0; y < kMatrixHeight; y++) {
      byte x = currentRow;
      if (currentDirection == 3) x = kMatrixWidth - 1 - currentRow;
      leds[XY(x, y)] = currentPalette[currentColor];
    }
  }

  currentRow++;

  if ((!(currentDirection & 1) && currentRow >= kMatrixHeight) || ((currentDirection & 1) && currentRow >= kMatrixWidth)) {
    currentRow = 0;
    currentColor += random8(3, 6);
    if (currentColor > 15) currentColor -= 16;
    currentDirection++;
    if (currentDirection > 3) currentDirection = 0;
    effectDelay = 200;
  }
}

#define COOLING  35
#define SPARKING 40
void Fire2012WithPalette() {
  FastLED.clear();
  CRGBPalette16 gPal;
  gPal = HeatColors_p;

  if (effectInit == false) {
    effectInit = true;
    FastLED.clear(); 
    effectDelay = 0;
    fadingActive = true;
  }
// Array of temperature readings at each simulation cell
  static byte heat[kMatrixWidth];

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < kMatrixHeight; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / kMatrixWidth) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= kMatrixHeight - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    for( int x = 0; x < kMatrixWidth; x++) {
    for( int h = 0; h < kMatrixWidth; h++) {
      // Scale the heat value from 0-255 down to 0-240
      // for best results with color palettes.
      byte colorindex = scale8( heat[h], 240);
      CRGB color = ColorFromPalette( gPal, colorindex);
      int pixelnumber;
      pixelnumber = h;
      
      leds[XY(x,pixelnumber)] = color;
    }
  }
}

void snow() {
  FastLED.clear();
  static unsigned int snowCols[kMatrixWidth] = {0};
 
  if (effectInit == false) {
    effectInit = true;
    FastLED.clear(); 
    effectDelay = 22;
    fadingActive = true;
  }

  CRGB snowColor = CRGB::White;

  FastLED.clear();

  for (byte i = 0; i < kMatrixWidth; i++) {
    if (snowCols[i] > 0) {
      snowCols[i] += random(16,0);
    } else {
      if (random8(0,100) == 0) snowCols[i] = 1;
    }
    byte tempY = snowCols[i] >> 8;
    byte tempRem = snowCols[i] & 0xFF;
    if (tempY <= kMatrixHeight) leds[XY(i,-tempY+15)] = snowColor % dim8_raw(255-tempRem);
    if (tempY < kMatrixHeight) leds[XY(i,-tempY)] = snowColor % dim8_raw(tempRem);
    if (tempY > kMatrixHeight) snowCols[i] = 15;
  }
}

void drip() {
  FastLED.clear();
  static unsigned int snowCols[kMatrixWidth] = {0};

  if (effectInit == false) {
    effectInit = true;
     FastLED.clear();
    effectDelay = 20;
    fadingActive = true;
  }

  CRGB snowColor = CRGB::SteelBlue;

  FastLED.clear();

  for (byte i = 0; i < kMatrixWidth; i++) {
    if (snowCols[i] > 0) {
      snowCols[i] += random(16,0);
    } else {
      if (random8(0,100) == 0) snowCols[i] = 1;
    }
    byte tempY = snowCols[i] >> 3;
    byte tempRem = snowCols[i] & 0xFF;
    if (tempY <= kMatrixHeight) leds[XY(i,tempY-1)] = snowColor % dim8_raw(255-tempRem);
    if (tempY < kMatrixHeight) leds[XY(i,tempY)] = snowColor % dim8_raw(tempRem);
    if (tempY > kMatrixHeight) snowCols[i] = 0;
  }
}

void confetti() {
  FastLED.clear();
  if (effectInit == false) {
    effectInit = true;
    effectDelay = 45;
    FastLED.clear();    
    currentPalette = RainbowColors_p;
    fadingActive = true;
  }

  for (byte i = 0; i < 4; i++) {
    leds[XY(random16(kMatrixWidth), random16(kMatrixHeight))] = ColorFromPalette(currentPalette, random16(255), 255); //CHSV(random16(255), 255, 255);
    random16_add_entropy(1);
  }
}

void rider() {
  FastLED.clear();
  if (effectInit == false) {
    effectInit = true;
    FastLED.clear();
    effectDelay = 0;
    currentPalette = RainbowColors_p;
    fadingActive = true;
  }

  static byte riderPos = 0;

  for (byte y = 0; y < kMatrixWidth; y++) {
    int brightness = abs(y * (256 / kMatrixWidth) - triwave8(riderPos * 2 + 127)) * 3;
    if (brightness > 255) brightness = 255;
    brightness = 255 - brightness;
    CRGB riderColor = CHSV(gHue, 255, brightness);
    for (byte x = 0; x < kMatrixWidth; x++) {
      leds[XY(y, x)] = riderColor;
    }
  }
  riderPos++;
}

void plasma() {
  FastLED.clear();
  if (effectInit == false) {
    effectInit = true;
    FastLED.clear(); 
    effectDelay = 0;
    fadingActive = true;
  }
  
  static byte offset  = 0;
  static int plasVector = 0;

  int xOffset = cos8(plasVector / 256);
  int yOffset = sin8(plasVector / 512);

  for (int x = 0; x < kMatrixWidth; x++) {
    for (int y = 0; y < kMatrixHeight; y++) {
      byte color = sin8(sqrt(sq(((float)x - 7.5) * 10 + xOffset - 127) + sq(((float)y - 2) * 10 + yOffset - 127)) + offset);
      leds[XY(x, y)] = CHSV(color, 255, 255);
    }
  }
  offset++;
  plasVector += 16;
}

void plasma2() {
  FastLED.clear();
  if (effectInit == false) {
    effectInit = true;
    FastLED.clear(); 
    effectDelay = 0;
    fadingActive = true;
  }
  static byte offset  = 0;
  static int plasVector = 0;
  
  int xOffset = 0;
  int yOffset = cos8(plasVector / 512);

  for (int x = 0; x < kMatrixWidth; x++) {
    for (int y = 0; y < kMatrixHeight; y++) {
      byte color = sin8(sqrt(sq(((float)x - 7.5) * 10 + xOffset - 127) + sq(((float)y - 2) * 10 + yOffset - 127)) + offset);
      leds[XY(x, y)] = CHSV(color, 255, 255);
    }
  }
  offset++;
  plasVector += 16;
}

void bpm() {
  FastLED.clear();
  bool gReverseDirection = false;
  
  if (effectInit == false) {
    effectInit = true;
    FastLED.clear(); 
    effectDelay = 0;
    fadingActive = true;
  }

  uint8_t gHue = 120;

  uint8_t BeatsPerMinute = 65;
  CRGBPalette16 palette = RainbowColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);

  for( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));

    int pixelnumber;
    
    if( gReverseDirection ) {
      pixelnumber = (NUM_LEDS - 1) - i;
    } else {
      pixelnumber = i;
    }
    leds[pixelnumber] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
  }  
}

void radiate2() {
  FastLED.clear();
  static byte offset  = 0;
  static int plasVector = 0;
 
  if (effectInit == false) {
    effectInit = true;
     FastLED.clear();
    effectDelay = 10;
    // selectRandomPalette();
    fadingActive = true;
  }

  int xOffset = (cos8(plasVector)-kMatrixWidth)*2;
  int yOffset = (cos8(plasVector)-kMatrixWidth)*2;
 
  for (int x = 0; x < kMatrixWidth; x++) {
    for (int y = 0; y < kMatrixHeight; y++) {
      byte color = sin8(sqrt(sq(((float)x - 12) * 12 + xOffset) + sq(((float)y - 2) * 12 + yOffset)) + offset);
      leds[XY(x, y)] = ColorFromPalette(currentPalette, color, 255);
    }
  }
  offset++;
  plasVector += 1;
}

// Draw slanting bars scrolling across the array, uses current hue
byte slantPos = 0;
void slantBars() {
  FastLED.clear();
  // startup tasks
  if (effectInit == false) {
    effectInit = true;
    FastLED.clear();
    effectDelay = 30;
  }

  for (byte x = 0; x < kMatrixWidth; x++) {
    for (byte y = 0; y < kMatrixHeight; y++) {
      leds[XY(x,y)] = CHSV(gHue, 255, quadwave8(x*32+y*48+slantPos));
    }
  }

  slantPos-=4;

}
// Draw slanting bars scrolling across the array, uses current hue
byte slantPos3 = 1;
void slantBars3() {
  FastLED.clear();
  // startup tasks
  if (effectInit == false) {
    effectInit = true;
    FastLED.clear();
    effectDelay = 15;
  }

  for (byte x = 0; x < kMatrixWidth; x++) {
    for (byte y = 0; y < kMatrixHeight; y++) {
      leds[XY(x,y)] = CHSV(gHue, 255, quadwave8(x*12+y*26+slantPos3));
    }
  }

  slantPos3+=16;

}

// Draw slanting bars scrolling across the array, uses current hue
byte slantPos2 = 0;
void slantBars2() {
  FastLED.clear();
  // startup tasks
  if (effectInit == false) {
    effectInit = true;
    FastLED.clear();
    effectDelay = 30;
  }

  for (byte x = 0; x < kMatrixWidth; x++) {
    for (byte y = 0; y < kMatrixHeight; y++) {
      leds[XY(x,y)] = CHSV(gHue, 255, cos8(x-y-slantPos2));
    }
  }

  slantPos2-=3;

}

void spinPlasma() {
  FastLED.clear();
  static byte offset  = 0;
  static int plasVector = 0;

  if (effectInit == false) {
    effectInit = true;
    FastLED.clear();
    effectDelay = 10;
    // selectRandomPalette();
    fadingActive = true;
  }

  int xOffset = (cos8(plasVector)-NUM_LEDS)/2;
  int yOffset = (sin8(plasVector)-NUM_LEDS)/2;

  //int xOffset = 0;
  //int yOffset = 0;
  
  for (int x = 0; x < kMatrixWidth; x++) {
    for (int y = 0; y < kMatrixHeight; y++) {
      byte color = sin8(sqrt(sq(((float)x - 7.5) * 12 + xOffset) + sq(((float)y - 2) * 12 + yOffset)) + offset);
      leds[XY(x, y)] = ColorFromPalette(currentPalette, color, 255);
    }
  }
  offset++;
  plasVector += 1;
}
//
//const byte upvoteBitmap[5] = {
//  0b00010000,
//  0b00111000,
//  0b01111100,
//  0b00111000,
//  0b00111000
//};
//
//void upvote() {
//  
//  if (effectInit == false) {
//    effectInit = true;
//    effectDelay = 10;
//    fadingActive = true;
//  }
//  
//  CRGB currentColor;
//
//  for (byte y = 0; y < 5; y++) {
//    for (byte x = 0; x < 8; x++) {
//      if (bitRead(upvoteBitmap[y], 7 - x) == 1) {
//        currentColor = CRGB::Green;
//      } else {
//        currentColor = CRGB::Black;
//      }
//
//      leds[XY(x, y)] = currentColor;
//      leds[XY(15 - x, y)] = currentColor;
//    }
//  }
//}
//

void three_sin(){
  FastLED.clear();
  if (effectInit == false) {
    effectInit = true;
    effectDelay = 0;
    FastLED.clear();
    // selectRandomPalette();
   // CRGBPalette16 targetPalette(PartyColors_p);
    fadingActive = true;
  }   
  wave1 += beatsin8(10,-4,4);
  wave2 += beatsin8(15,-2,2);
  wave3 += beatsin8(12,-3, 3);
  for (byte x = 0; x<kMatrixHeight; x++) {
    for (int j=0; j<kMatrixWidth; j++) {
      uint8_t tmp = sin8(mul1*j + wave1) + sin8(mul1*j + wave2) + sin8(mul1*j + wave3);
      leds[XY(j,x)] = ColorFromPalette(currentPalette, tmp, 255);
    }    
  }
} // three_sin()
void three_sinVert(){
  FastLED.clear();
  if (effectInit == false) {
    effectInit = true;
    effectDelay = 0;
    FastLED.clear();
    // selectRandomPalette();
   // CRGBPalette16 targetPalette(PartyColors_p);
    fadingActive = true;
  }   
  wave1 += beatsin8(10,-4,4);
  wave2 += beatsin8(15,-2,2);
  wave3 += beatsin8(12,-3, 3);
  for (byte x = 0; x<kMatrixWidth; x++) {
    for (int y=0; y<kMatrixHeight; y++) {
      uint8_t tmp = sin8(mul1*y + wave1) + sin8(mul1*y + wave2) + sin8(mul1*y + wave3);
      leds[XY(x,y)] = ColorFromPalette(currentPalette, tmp, 255);
    }    
  }
} // three_sin()


void crazy2() { 
  FastLED.clear();

  if (effectInit == false) {
    effectInit = true;
    effectDelay = 0;
    FastLED.clear();
    // selectRandomPalette();
   // CRGBPalette16 targetPalette(PartyColors_p);
    fadingActive = true;
  }
     
   ChangeMe();
    EVERY_N_MILLISECONDS(thisdelay) {
    nblendPaletteTowardPalette( currentPalette, targetPalette, maxChanges);
    three_sin(); 
  }
  
  FastLED.show();
} 

void colorRotation() {
  FastLED.clear();
  if (effectInit == false) {
    effectInit = true;
    effectDelay = 10;
    FastLED.clear();
    // selectRandomPalette();
    // CRGBPalette16 targetPalette(PartyColors_p);
    fadingActive = true;
  }

  for(int i = 0; i < NUM_LEDS; i = i + 1) {
     
    leds[i] = CHSV(gHue++, 255,255);
    // leds[whiteLed] = CRGB::White;
    FastLED.show();
    delay(10);
    leds[i] = CRGB::Black;
  }
  // fillAll(CHSV(10, 100,100));
}

void vertThreeSine() {
  FastLED.clear();
  if (effectInit == false) {
    effectInit = true;
    effectDelay = 0;
    FastLED.clear();
    // selectRandomPalette();
   // CRGBPalette16 targetPalette(PartyColors_p);
    fadingActive = true;
  }
  static byte sineOffset = 0;

  for (byte x = 0; x < kMatrixWidth; x++) {
    for (int y = 0; y < kMatrixHeight; y++) {

      // Calculate "sine" waves with varying periods
      // sin8 is used for speed; cos8, quadwave8, or triwave8 would also work here
      byte sinDistanceR = qmul8(abs(x * (255 / kMatrixWidth) - quadwave8(sineOffset * 8 + y * 9)), 2);
      byte sinDistanceG = qmul8(abs(x * (255 / kMatrixWidth) - quadwave8(sineOffset * 9 + y * 9)), 2);
      byte sinDistanceB = qmul8(abs(x * (255 / kMatrixWidth) - quadwave8(sineOffset * 10 + y * 9)), 2);
 
     leds[XY(x, y)] = CRGB(255 - sinDistanceR, 255 - sinDistanceG, 255 - sinDistanceB);
     // leds[XY(x, y)] = CRGB(255 - sinDistanceR, 255 - sinDistanceG, 255 - sinDistanceB);
    }
  }
  sineOffset++;
}

void threeSine() {
  FastLED.clear();
  if (effectInit == false) {
    effectInit = true;
    effectDelay = 0;
    FastLED.clear();
    // selectRandomPalette();
   // CRGBPalette16 targetPalette(PartyColors_p);
    fadingActive = true;
  }
  static byte sineOffset = 0;

  for (byte x = 0; x < kMatrixWidth; x++) {
    for (int y = 0; y < kMatrixHeight; y++) {

      // Calculate "sine" waves with varying periods
      // sin8 is used for speed; cos8, quadwave8, or triwave8 would also work here
      byte sinDistanceR = qmul8(abs(y * (255 / kMatrixHeight) - quadwave8(sineOffset * 9 + x * 18)), 2);
      byte sinDistanceG = qmul8(abs(y * (255 / kMatrixHeight) - quadwave8(sineOffset * 10 + x * 18)), 2);
      byte sinDistanceB = qmul8(abs(y * (255 / kMatrixHeight) - quadwave8(sineOffset * 11 + x * 18)), 2);

      leds[XY(x, y)] = CRGB(255 - sinDistanceR, 255 - sinDistanceG, 255 - sinDistanceB);
     // leds[XY(x, y)] = CRGB(255 - sinDistanceR, 255 - sinDistanceG, 255 - sinDistanceB);
    }
  }
  sineOffset++;
}

uint8_t angle = 16;
void amazing() {
  FastLED.clear();
  if (effectInit == false) {
    effectInit = true;
    effectDelay = 10;
    FastLED.clear();
    // selectRandomPalette();
    //currentPalette = retro2_16_gp;
    fadingActive = true;
  }
  uint8_t h = sin8(angle);
  
  for (int16_t x= kMatrixWidth; x>=0; --x) {
    for (int16_t y= kMatrixHeight; y>=0; --y) {
      leds[XY(x, y)] = CHSV(h, 255, 255);
      h += 32;
    }
  }
  angle += 4;
}

void blendme() {
  FastLED.clear();
  if (effectInit == false) {
    effectInit = true;
    effectDelay = 0;
    FastLED.clear();
    // selectRandomPalette();
   // CRGBPalette16 targetPalette(PartyColors_p);
    fadingActive = true;
  }
  uint8_t starthue = beatsin8(20, 0, 255);
  uint8_t endhue = beatsin8(35, 0, 255);
  if (starthue < endhue) {
    fill_gradient(leds, NUM_LEDS, CHSV(starthue,255,255), CHSV(endhue,255,255), FORWARD_HUES);    // If we don't have this, the colour fill will flip around
  } else {
    fill_gradient(leds, NUM_LEDS, CHSV(starthue,255,255), CHSV(endhue,255,255), BACKWARD_HUES);
  }
} // blendme()

uint32_t xscale = 2;                                          // How far apart they are
uint32_t yscale = 100;                                           // How fast they move
uint8_t index1 = 0;
void inoise8_fire() {
  FastLED.clear();
  if (effectInit == false) {
    effectInit = true;
    effectDelay = 0;
    FastLED.clear();
    // selectRandomPalette();
   // CRGBPalette16 targetPalette(PartyColors_p);
    fadingActive = true;
  }
  for(int i = 0; i < NUM_LEDS; i++) {
    index1 = inoise8(i*yscale,millis()*xscale*NUM_LEDS/255);                                           // X location is constant, but we move along the Y at the rate of millis()
    leds[i] = ColorFromPalette(currentPalette, min(i*(index1)>>6, 255), i*255/NUM_LEDS, LINEARBLEND);  // With that value, look up the 8 bit colour palette value and assign it to the current LED.
  }
} // inoise8_fire()

#define qsubd(x, b)  ((x>b)?b:0) // A digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsuba(x, b)  ((x>b)?x-b:0) // Unsigned subtraction macro. if result <0, then => 0
unsigned long previousMillis; // Store last time the strip was updated.
 
void two_sin() {
  if (effectInit == false) {
    effectInit = true;
    effectDelay = 0;
    FastLED.clear();
    // selectRandomPalette();
   // CRGBPalette16 targetPalette(PartyColors_p);
    fadingActive = true;
  }
    thisdir ? thisphase += beatsin8(thisspeed, 2, 10) : thisphase -= beatsin8(thisspeed, 2, 10);
    thatdir ? thatphase += beatsin8(thisspeed, 2, 10) : thatphase -= beatsin8(thatspeed, 2, 10);
    thishue += thisrot;                                        // Hue rotation is fun for thiswave.
    thathue += thatrot;                                        // It's also fun for thatwave.
  
  for (int k=0; k<NUM_LEDS-1; k++) {
    int thisbright = qsuba(cubicwave8((k*allfreq)+thisphase), thiscutoff);      // qsub sets a minimum value called thiscutoff. If < thiscutoff, then bright = 0. Otherwise, bright = 128 (as defined in qsub)..
    int thatbright = qsuba(cubicwave8((k*allfreq)+128+thatphase), thatcutoff);  // This wave is 180 degrees out of phase (with the value of 128).

    leds[k] = ColorFromPalette(thisPalette, thishue, thisbright, currentBlending);
    leds[k] += ColorFromPalette(thatPalette, thathue, thatbright, currentBlending);
  }
     nscale8(leds,NUM_LEDS,fadeval);
     
} // two_sin()

void twoSin() {
  if (effectInit == false) {
    effectInit = true;
    effectDelay = 0;
    FastLED.clear();
    // selectRandomPalette();
   // CRGBPalette16 targetPalette(PartyColors_p);
    fadingActive = true;
  }
  ChangeMe2();

  EVERY_N_MILLISECONDS(thisdelay) {
    two_sin();                                                // Routine is still delay based, but at least it's now a non-blocking day.
  }

  FastLED.show();

}

void ripple() {
  FastLED.clear();
  if (effectInit == false) {
    effectInit = true;
    effectDelay = 0;
    FastLED.clear();
    // selectRandomPalette();
   // CRGBPalette16 targetPalette(PartyColors_p);
    fadingActive = true;
  }
  fadeToBlackBy(leds, NUM_LEDS, fadeval);                             // 8 bit, 1 = slow, 255 = fast
  
  switch (step) {

    case -1:                                                          // Initialize ripple variables.
      center = random(NUM_LEDS);
      colour = random8();
      step = 0;
      break;

    case 0:
      leds[center] = ColorFromPalette(currentPalette, colour, myfade, currentBlending);
      
      step ++;
      break;

    case maxsteps:                                                    // At the end of the ripples.
      step = -1;
      break;

    default:                                                          // Middle of the ripples.
      leds[(center + step + NUM_LEDS) % NUM_LEDS] += ColorFromPalette(currentPalette, colour, myfade/step*2, currentBlending);       // Simple wrap from Marc Miller
      leds[(center - step + NUM_LEDS) % NUM_LEDS] += ColorFromPalette(currentPalette, colour, myfade/step*2, currentBlending);
      step ++;                                                         // Next step.
      break;  
  } // switch step
  
} // ripple()

void serendipitous () {
  FastLED.clear();
  if (effectInit == false) {
    effectInit = true;
    effectDelay = 0;
    FastLED.clear();
    // selectRandomPalette();
   // CRGBPalette16 targetPalette(PartyColors_p);
    fadingActive = true;
  }
  EVERY_N_SECONDS(5) {
    uint8_t baseC = random8();
    targetPalette = CRGBPalette16(CHSV(baseC-3, 255, random8(192,255)), CHSV(baseC+2, 255, random8(192,255)), CHSV(baseC+5, 192, random8(192,255)), CHSV(random8(), 255, random8(192,255)));

    X = Xorig;
    Y = Yorig;    
  }

//  Xn = X-(Y/2); Yn = Y+(Xn/2);
//  Xn = X-Y/2;   Yn = Y+Xn/2;
//  Xn = X-(Y/2); Yn = Y+(X/2.1);
  Xn = X-(Y/3); Yn = Y+(X/1.5);
//  Xn = X-(2*Y); Yn = Y+(X/1.1);
  
  X = Xn;
  Y = Yn;

  index2=(sin8(X)+cos8(Y))/2;                            // Guarantees maximum value of 255

  CRGB newcolor = ColorFromPalette(currentPalette, index2, 255, LINEARBLEND);
  
//  nblend(leds[X%NUM_LEDS-1], newcolor, 224);          // Try and smooth it out a bit. Higher # means less smoothing.
  nblend(leds[map(X,0,65535,0,NUM_LEDS)], newcolor, 224); // Try and smooth it out a bit. Higher # means less smoothing.
  
  fadeToBlackBy(leds, NUM_LEDS, 16);                    // 8 bit, 1 = slow, 255 = fast

} // serendipitous()




void amazingNoise() {
  FastLED.clear();
  if (effectInit == false) {
    effectInit = true;
    effectDelay = 0;
    FastLED.clear();
    // selectRandomPalette();
   // CRGBPalette16 targetPalette(PartyColors_p);
    fadingActive = true;
  }
  // Periodically choose a new palette, speed, and scale
  ChangePaletteAndSettingsPeriodically();

  // generate noise data
  fillnoise8();
  
  // convert the noise data to colors in the LED array
  // using the current palette
  mapNoiseToLEDsUsingPalette();

}


void colorSnake() {
  FastLED.clear();
  if (effectInit == false) {
    effectInit = true;
    effectDelay = 0;
    FastLED.clear();
    // selectRandomPalette();
   // CRGBPalette16 targetPalette(PartyColors_p);
    fadingActive = true;
  }
  static uint8_t hue = 10;
  //Serial.print("x");
  // First slide the led in one direction
  for(int i = 0; i < NUM_LEDS; i++) {
    // Set the i'th led to red 
    leds[i] = CHSV(hue++, 255, 255);
    // Show the leds
    FastLED.show(); 
    // now that we've shown the leds, reset the i'th led to black
    // leds[i] = CRGB::Black;
    fadeall();
    // Wait a little bit before we loop around and do it again
    delay(0);
  }
  //Serial.print("x");

  // Now go in the other direction.  
  for(int i = (NUM_LEDS)-1; i >= 0; i--) {
    // Set the i'th led to red 
    leds[i] = CHSV(hue++, 255, 255);
    // Show the leds
    FastLED.show();
    // now that we've shown the leds, reset the i'th led to black
    // leds[i] = CRGB::Black;
    fadeall();
    // Wait a little bit before we loop around and do it again
    delay(0);
  }
  FastLED.clear();
}

#define TEMPERATURE_1 Tungsten100W
void tungsten() {
  FastLED.clear();
  if (effectInit == false) {
    effectInit = true;
    effectDelay = 0;
    FastLED.clear();
    // selectRandomPalette();
   // CRGBPalette16 targetPalette(PartyColors_p);
    fadingActive = true;
  }
  // draw a generic, no-name rainbow
  static uint8_t starthue = 0;
  fill_rainbow( leds + 5, NUM_LEDS - 5, --starthue, 20);
  FastLED.setTemperature( TEMPERATURE_1 ); // first temperature
  leds[0] = TEMPERATURE_1; // show indicator pixel
}


#define TEMPERATURE_2 OvercastSky
void overcastSky() {
  FastLED.clear();
  if (effectInit == false) {
    effectInit = true;
    effectDelay = 0;
    FastLED.clear();
    // selectRandomPalette();
   // CRGBPalette16 targetPalette(PartyColors_p);
    fadingActive = true;
  }
  // draw a generic, no-name rainbow
  static uint8_t starthue = 0;
  fill_rainbow( leds + 5, NUM_LEDS - 5, --starthue, 20);

  FastLED.setTemperature( TEMPERATURE_2 ); // second temperature
  leds[0] = TEMPERATURE_2; // show indicator pixel
}


#define NORMAL 0
#define RAINBOW 1
#define charSpacing 2
// Scroll a text string
void scrollText(byte message, byte style, CRGB fgColor, CRGB bgColor) {
  static byte currentMessageChar = 0;
  static byte currentCharColumn = 0;
  static byte paletteCycle = 0;
  static CRGB currentColor;
  static byte bitBuffer[16] = {0};
  static byte bitBufferPointer = 0;


  // startup tasks
  if (effectInit == false) {
    effectInit = true;
    effectDelay = 35;
    currentMessageChar = 0;
    currentCharColumn = 0;
    selectFlashString(message);
    loadCharBuffer(loadStringChar(message, currentMessageChar));
    currentPalette = RainbowColors_p;
    for (byte i = 0; i < kMatrixHeight; i++) bitBuffer[i] = 0;
  }


  paletteCycle += 10;

  if (currentCharColumn < 16) { // characters are 5 pixels wide
    bitBuffer[(bitBufferPointer + kMatrixWidth - 1) % kMatrixWidth] = charBuffer[currentCharColumn]; // character
  } else {
    bitBuffer[(bitBufferPointer + kMatrixWidth - 1) % kMatrixWidth] = 0; // space
  }

  CRGB pixelColor;
  for (byte x = 0; x < kMatrixWidth; x++) {
    for (byte y = 0; y < 32; y++) { // characters are 5 pixels tall
      if (bitRead(bitBuffer[(bitBufferPointer + x) % kMatrixWidth], y) == 1) {
        if (style == RAINBOW) {
          pixelColor = ColorFromPalette(currentPalette, paletteCycle+y*16, 255);
        } else {
          pixelColor = fgColor;
        }
      } else {
        pixelColor = bgColor;
      }
      leds[XY(x, y)] = pixelColor;
    }
  }

  currentCharColumn++;
  if (currentCharColumn > (4 + charSpacing)) {
    currentCharColumn = 0;
    currentMessageChar++;
    char nextChar = loadStringChar(message, currentMessageChar);
    if (nextChar == 0) { // null character at end of strong
      currentMessageChar = 0;
      nextChar = loadStringChar(message, currentMessageChar);
    }
    loadCharBuffer(nextChar);
  }

  bitBufferPointer++;
  if (bitBufferPointer > 32) bitBufferPointer = 0;

}


void scrollTextZero() {
  scrollText(0, NORMAL, CRGB::Red, CRGB::Black);
}

void scrollTextOne() {
  scrollText(1, RAINBOW, 0, CRGB::Black);
}

void scrollTextTwo() {
  scrollText(2, NORMAL, CRGB::Green, CRGB(0,0,8));
}
