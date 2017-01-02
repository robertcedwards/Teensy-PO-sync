#include<FastLED.h>
const uint8_t kMatrixWidth = 16;
const uint8_t kMatrixHeight = 1;
#define MAX_DIMENSION ((kMatrixWidth>kMatrixHeight) ? kMatrixWidth : kMatrixHeight)
#define NUM_LEDS (kMatrixWidth * kMatrixHeight)
// Param for different pixel layouts
const bool    kMatrixSerpentineLayout = true;


uint16_t XY( uint8_t x, uint8_t y)
{
  uint16_t i;

  if( kMatrixSerpentineLayout == false) {
    i = (y * kMatrixWidth) + x;
  }

  if( kMatrixSerpentineLayout == true) {
    if( y & 0x01) {
      // Odd rows run backwards
      uint8_t reverseX = (kMatrixWidth - 1) - x;
      i = (y * kMatrixWidth) + reverseX;
    } else {
      // Even rows run forwards
      i = (y * kMatrixWidth) + x;
    }
  }

  return i;
}

// The leds
CRGB leds[kMatrixWidth * kMatrixHeight];

// The 32bit version of our coordinates
static uint16_t x;
static uint16_t y;
static uint16_t z;


uint16_t speed = 20; // a nice starting speed, mixes well with a scale of 100
uint16_t scale = 311;
uint8_t noise[MAX_DIMENSION][MAX_DIMENSION];
#define NUM_LEDS 60
#define DATA_PIN 1
byte tick = 0;
byte PO_value;
byte nano_value;

const byte clock_byte = 248;
const byte start_byte = 250;
const byte continue_byte = 251;
const byte stop_byte = 252;

const byte LED_pin = 11;
const byte SYNC_pin = 0;
int DDRF;
int PORTF;

void setup() {
  usbMIDI.setHandleRealTimeSystem(doRealTime);
  DDRB = 0xFF;
  DDRF = 0xFF;
  pinMode(LED_pin, OUTPUT);
  digitalWrite(LED_pin, HIGH);
  pinMode(SYNC_pin, OUTPUT);
  digitalWrite(SYNC_pin, HIGH);
  // uncomment the following lines if you want to see FPS count information
  // Serial.begin(38400);
  // Serial.println("resetting!");
  delay(3000);
  LEDS.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  LEDS.setBrightness(96);

  // Initialize our coordinates to some random values
  x = random16();
  y = random16();
  z = random16();
}
// Fill the x/y array of 8-bit noise values using the inoise8 function.
void fillnoise8() {
  for(int i = 0; i < MAX_DIMENSION; i++) {
    int ioffset = scale * i;
    for(int j = 0; j < MAX_DIMENSION; j++) {
      int joffset = scale * j;
      noise[i][j] = inoise8(x + ioffset,y + joffset,z);
    }
  }
  z += speed;
}

void loop() {
  usbMIDI.read();
}

void doRealTime(byte value) {
  if (value == start_byte || value == continue_byte) {
    tick = 0;
  }

  if (value == stop_byte) {
    PORTB = 0;
    PORTF = 0;
  }


  if (value == clock_byte) {
    tick++;

    if (tick == 24) {
      tick = 0;
    }

    // PORTB = 255 * (1 - ((tick / 6) & 1));
    // PORTF = 255 * (1 - ((tick / 2) & 1));
    // digitalWriteFast(LED_pin, 1 - ((tick / 12) & 1));
    digitalWriteFast(SYNC_pin, 1 - ((tick / 6) & 1));
     static uint8_t ihue=0;
  fillnoise8();
  for(int i = 0; i < kMatrixWidth; i++) {
    for(int j = 0; j < kMatrixHeight; j++) {

      leds[XY(i,j)] = CHSV(noise[j][i],255,noise[i][j]);
    }
  }
  ihue+=1;

  LEDS.show();
  // delay(10);
  }
}
