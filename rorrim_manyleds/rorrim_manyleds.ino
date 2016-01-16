#include "FastLED.h"
#include <MIDI.h>

// Based on FastLED "100-lines-of-code" demo reel by Mark Kriegsman, December 2014
// LED responding to incoming MIDI for Rorrim installation Hertzliya Museum 
// 
// HAR JAN 2016

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

MIDI_CREATE_INSTANCE(HardwareSerial, Serial, dueMidi);

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

#define NUM_STRIPS 8

#define DATA_PIN_1 25
#define DATA_PIN_2 26
#define DATA_PIN_3 27
#define DATA_PIN_4 28
#define DATA_PIN_5 14
#define DATA_PIN_6 15
#define DATA_PIN_7 29
#define DATA_PIN_8 11
//#define CLK_PIN   4
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS    144

CRGB strips[NUM_STRIPS][NUM_LEDS];

unsigned long previousMillis = 0;
const long interval = 100;      
const long glitterInterval = 300;
const long ledRefreshInterval = 30;
int ledState = LOW;
int level;
int newLevel;

#define BRIGHTNESS          96
#define FRAMES_PER_SECOND  120

void rainbow();
void rainbowWithGlitter();
void confetti();
void sinelon();
void juggle();
void bpm();
void nextPattern();
void setLeftHandValues();
void addGlitter(fract8 chanceOfGlitter);
void addTwoColorGlitter(fract8 chanceOfGlitter, CRGB firstColor, CRGB secondColor);

CHSVPalette16 leftHandPallete;
CHSVPalette16 rightHandPalette;

#define MODE_ORGASM 2
#define MODE_SINGLE_INTERACTION 1
#define MODE_DEFAULT 0
#define TEST 0
byte mode = MODE_DEFAULT;

byte orgasmStage = 0;
byte orgasmValue = 0;
byte orgasmLoopCounter = 0;

byte baseHue = 0;

byte rightHandStep;
byte rightHandValue = 0; // 10 - 255
byte leftHandX; // 0 - 255
byte leftHandY; // 0 - 255

void handleNoteOn(byte inChannel, byte inNote, byte inVelocity)
{
  previousMillis = millis();
  digitalWrite(13, HIGH);
  
  switch (inNote) {
    case 1:
    case 2:      
    case 3:      
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
      mode = MODE_SINGLE_INTERACTION;
      rightHandStep = inNote - 1;
      baseHue = inNote * 32;      
      break;
    case 10:
      mode = MODE_SINGLE_INTERACTION;
      rightHandValue = inVelocity * 2;
      break;
    case 11:
      mode = MODE_SINGLE_INTERACTION;
      leftHandX = inVelocity * 2;
      break;
    case 12:   
      mode = MODE_SINGLE_INTERACTION; 
      leftHandY = inVelocity * 2;
      break;
    case 40:
      mode = MODE_DEFAULT;
      break;
    case 69:
      mode = MODE_ORGASM;
      orgasmStage = 0;   
      orgasmLoopCounter = 0;   
      break;
  }  
}

void setup() {
  delay(3000); // 3 second delay for recovery

  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  dueMidi.setInputChannel(7);
  dueMidi.turnThruOff();
  dueMidi.setHandleNoteOn(handleNoteOn);
  dueMidi.begin(MIDI_CHANNEL_OMNI);
    
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN_1,COLOR_ORDER>(strips[0], NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.addLeds<LED_TYPE,DATA_PIN_2,COLOR_ORDER>(strips[1], NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.addLeds<LED_TYPE,DATA_PIN_3,COLOR_ORDER>(strips[2], NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.addLeds<LED_TYPE,DATA_PIN_4,COLOR_ORDER>(strips[3], NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.addLeds<LED_TYPE,DATA_PIN_5,COLOR_ORDER>(strips[4], NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.addLeds<LED_TYPE,DATA_PIN_6,COLOR_ORDER>(strips[5], NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.addLeds<LED_TYPE,DATA_PIN_7,COLOR_ORDER>(strips[6], NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.addLeds<LED_TYPE,DATA_PIN_8,COLOR_ORDER>(strips[7], NUM_LEDS).setCorrection(TypicalSMD5050);
//  FastLED.addLeds<LED_TYPE,DATA_PIN_10,COLOR_ORDER>(strips[9], NUM_LEDS).setCorrection(TypicalLEDStrip);
//  FastLED.addLeds<LED_TYPE,DATA_PIN_11,COLOR_ORDER>(strips[10], NUM_LEDS).setCorrection(TypicalLEDStrip);
//  FastLED.addLeds<LED_TYPE,DATA_PIN_12,COLOR_ORDER>(strips[11], NUM_LEDS).setCorrection(TypicalLEDStrip);
//  FastLED.addLeds<LED_TYPE,DATA_PIN_13,COLOR_ORDER>(strips[12], NUM_LEDS).setCorrection(TypicalLEDStrip);
//  FastLED.addLeds<LED_TYPE,DATA_PIN_14,COLOR_ORDER>(strips[13], NUM_LEDS).setCorrection(TypicalLEDStrip);
//  FastLED.addLeds<LED_TYPE,DATA_PIN_15,COLOR_ORDER>(strips[14], NUM_LEDS).setCorrection(TypicalLEDStrip);
//  FastLED.addLeds<LED_TYPE,DATA_PIN_16,COLOR_ORDER>(strips[15], NUM_LEDS).setCorrection(TypicalLEDStrip);
//  FastLED.addLeds<LED_TYPE,DATA_PIN_17,COLOR_ORDER>(strips[16], NUM_LEDS).setCorrection(TypicalLEDStrip);
//  FastLED.addLeds<LED_TYPE,DATA_PIN_18,COLOR_ORDER>(strips[17], NUM_LEDS).setCorrection(TypicalLEDStrip);
  
  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);

  FastLED.clear();
  FastLED.show();

  #if TEST
  byte hue = 0;  
  for (int z = 0; z < NUM_STRIPS; z++) {
    for (int i = 0; i < NUM_LEDS; i++){
      strips[z][i] = CHSV(hue, 255, 255);
    }
    hue += 32;
    FastLED.show();
    delay(1000);
    FastLED.clear();
  }
  #endif

  rightHandValue = 150;
  leftHandX = 200;
  leftHandY = 100;

  leftHandPallete = 
    CHSVPalette16(
      CHSV(128, 255, 255),
      CHSV(140, 255, 255),
      CHSV(152, 255, 255),
      CHSV(164, 255, 255),
      CHSV(176, 255, 255),
      CHSV(188, 255, 255),
      CHSV(200, 255, 255),
      CHSV(212, 255, 255),
      CHSV(224, 255, 255),
      CHSV(236, 255, 255),
      CHSV(248, 255, 255),
      CHSV(6, 255, 255),
      CHSV(18, 255, 255),
      CHSV(30, 255, 255),
      CHSV(42, 255, 255),
      CHSV(54, 255, 255));        
}

// List of patterns to cycle through.  Each is defined as a separate function below.
void (*gPatterns[6])(void) = { rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm }; 

uint8_t gCurrentPatternNumber = 5; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

bool firstRun = true;

unsigned long previousGlitterMillis = 0;
byte testLeftHandValue = 0;

void loop()
{  
  // Call the current pattern function once, updating the 'leds' array
  // gPatterns[gCurrentPatternNumber]();      

  unsigned long currentMillis = millis();
  // was 60
  EVERY_N_MILLISECONDS( 60 ) { 
    if (mode == MODE_DEFAULT) {
      for (int z = 0; z < NUM_STRIPS; z++) {
        fadeToBlackBy( strips[z], NUM_LEDS, 2);  
      }      
      if (currentMillis - previousGlitterMillis >= glitterInterval) {
        previousGlitterMillis = currentMillis;
        addTwoColorGlitter(60, CRGB::Crimson, CRGB::DarkBlue);
      }
      
    } else if (mode == MODE_SINGLE_INTERACTION) {
      #if TEST
      if (currentMillis - previousGlitterMillis >= glitterInterval) {
        previousGlitterMillis = currentMillis;
        leftHandX = random8();
        leftHandY = random8();
      }
      #endif
      setLeftHandValues();
      juggle();
    } else if (mode == MODE_ORGASM) {
      if (orgasmStage == 0) {                        
        for (int z = 0; z < NUM_STRIPS; z++) {
          fadeToBlackBy(strips[z], NUM_LEDS, 20);          
        }
        CRGB firstLedOfOuterStrips = strips[5][0];
        CRGB l = firstLedOfOuterStrips;
        if (l.r == 0 && l.g == 0 && l.b == 0) {
          // it is completely black, wait a bit           
          orgasmLoopCounter++;
          if (orgasmLoopCounter == 20) {
            orgasmLoopCounter = 0;
            orgasmStage++;
          }
          orgasmValue = 0x00;
        }
      }

      if (orgasmStage == 1){
        for (int z = 0; z < NUM_STRIPS; z++) {
          fill_solid(strips[z], NUM_LEDS, CHSV(192, 100, orgasmValue));
        }

        addGlitter(8); 
        
        orgasmValue = qadd8(orgasmValue, 8);           
        if (orgasmValue == 0xFF) {
          orgasmStage++;
        }
      }

      

      if (orgasmStage == 2) {
        if (orgasmLoopCounter == 0) {
          CRGB paintWithColor = strips[0][0] + CRGB(3, 0, 0);        
          for (int z = 0; z < NUM_STRIPS; z++) {
            fill_solid(strips[z], NUM_LEDS, paintWithColor);
          }
  
          addGlitter(8); 
  
          if (strips[0][0].r == 255) {
            orgasmLoopCounter++;
          } 
        } else if (orgasmLoopCounter == 1){
          CRGB paintWithColor = strips[0][0] - CRGB(0, 0, 3);        
          for (int z = 0; z < NUM_STRIPS; z++) {
            fill_solid(strips[z], NUM_LEDS, paintWithColor);
          }
  
          addGlitter(8); 
  
          if (strips[0][0].b <= 20) {
            orgasmStage++;
          }           
        }               
      }

      if (orgasmStage == 3) {
        CRGB paintWithColor = strips[0][0] + CRGB(0, 0, 3);        
        for (int z = 0; z < NUM_STRIPS; z++) {
          fill_solid(strips[z], NUM_LEDS, paintWithColor);
        }

        addGlitter(8); 

        if (strips[0][0].b == 255) {
          orgasmStage++;
        }      
      }

      if (orgasmStage == 4) {        
        for (int z = 0; z < NUM_STRIPS; z++) {
          fadeToBlackBy(strips[z], NUM_LEDS, 40);          
        }          

        if (strips[0][0].r < 10) {
          // completely black
          orgasmStage++;
          orgasmValue = 0x00;
        }
      }

      if (orgasmStage == 5) {                   
        for (int z = 0; z < NUM_STRIPS; z++) {
          fill_solid(strips[z], NUM_LEDS, CHSV(0, 0, orgasmValue));
        }        

        addGlitter(8); 

        orgasmValue = qadd8(orgasmValue, 10);
        if(orgasmValue == 0xFF) {
          // completely lit
          orgasmStage++;
        }
      }

      if (orgasmStage == 6) {        
        for (int z = 0; z < NUM_STRIPS; z++) {
          fill_solid(strips[z], NUM_LEDS, CHSV(0, 0, orgasmValue));
        }              

        orgasmValue = qsub8(orgasmValue, 5);
        if(orgasmValue <= 127) {
          // halfway lit
          orgasmStage++;
        }                
      }

      if (orgasmStage == 7) {        
        for (int z = 0; z < NUM_STRIPS; z++) {
          fill_solid(strips[z], NUM_LEDS, CHSV(0, 0, orgasmValue));
        }        

        addGlitter(8); 

        orgasmValue = qadd8(orgasmValue, 10);
        if(orgasmValue == 0xFF) {
          // completely lit
          orgasmStage++;
        }
      }

      if (orgasmStage == 8) {        
        for (int z = 0; z < NUM_STRIPS; z++) {
          fill_solid(strips[z], NUM_LEDS, CHSV(0, 0, orgasmValue));
        }              

        orgasmValue = qsub8(orgasmValue, 5);
        if(orgasmValue <= 127) {
          // halfway lit
          orgasmStage++;
        }                
      }

      if (orgasmStage == 9) {        
        for (int z = 0; z < NUM_STRIPS; z++) {
          fill_solid(strips[z], NUM_LEDS, CHSV(0, 0, orgasmValue));
        }        

        addGlitter(8); 

        orgasmValue = qadd8(orgasmValue, 10);
        if(orgasmValue == 0xFF) {
          // completely lit
          orgasmStage++;
        }
      }

      if (orgasmStage == 10) {
        for (int z = 0; z < NUM_STRIPS; z++) {
          fadeToBlackBy(strips[z], NUM_LEDS, 10);
        }
      }           
    }
    FastLED.show(); 
  } // update the display every 25 ms

  EVERY_N_MILLISECONDS(1) {
    dueMidi.read(); // read from midi input buffer every 20 ms  
  }
  

  

  if (currentMillis - previousMillis >= interval) {    
    digitalWrite(13, LOW);    
  }
}



byte transitionStep(byte source, byte destination, byte forwardStep, byte backwardsStep) {
  byte result;
  if (source < destination) {
    result = source + forwardStep;
    if (result > destination) {
      // went too far
      result = destination;
    }
  } else if (source > destination) {
    result = source - backwardsStep;
    if (result < destination) {
      // went too far
      result = destination;
    }
  } else {
    result = destination;
  }

  return result;
}

byte leftHandHue;
byte leftHandValue;

void setLeftHandValues() {
  leftHandHue = transitionStep(leftHandHue, leftHandX, 4, 4);
  leftHandValue = transitionStep(leftHandValue, leftHandY, 4, 4);
  for (int z = 4; z < 8; z++) {
    fadeToBlackBy( strips[z], NUM_LEDS, 10);
    for (int i = 0; i < NUM_LEDS; i++){
      strips[z][i] = ColorFromPalette(leftHandPallete, leftHandHue, leftHandValue, LINEARBLEND);
    }
  }
}

// This only controls the first 5 strips 
void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  for (int z = 0; z < 4; z++) {
    fadeToBlackBy( strips[z], NUM_LEDS, 25);
    byte dothue = baseHue;    
    for( int i = 0; i < 8; i++) {
      strips[z][beatsin8(i+4,0,NUM_LEDS)] |= CHSV(dothue, 200, rightHandValue);
      dothue += 8;
    }
  }
}


void rainbow() 
{
  // FastLED's built-in rainbow generator
  for (int i = 0; i < NUM_STRIPS; i++) {
    fill_rainbow( strips[i], NUM_LEDS, gHue, 7);  
  }  
}

void addTwoColorGlitter(fract8 chanceOfGlitter, CRGB firstColor, CRGB secondColor) {
  for (int i = 0; i < NUM_STRIPS; i++) {
    fract8 randomNumber = random8();
    if(randomNumber < chanceOfGlitter) {
      if (randomNumber < chanceOfGlitter / 2){
        strips[i][ random16(NUM_LEDS) ] += firstColor;  
      } else {
        strips[i][ random16(NUM_LEDS) ] += secondColor;  
      }      
    }
  }
}

void addGlitter( fract8 chanceOfGlitter) 
{
  for (int i = 0; i < NUM_STRIPS; i++) {
    if( random8() < chanceOfGlitter) {
      strips[i][ random16(NUM_LEDS) ] += CRGB::White;
    }
  }
}

