/*
 * Firmware for D15-based LED strand controller. (Arduino.ino)
 * Scott Lawrence yorgle@gmail.com January 2013
 */

#include "SPI.h"
#include "WS2801.h"

/*****************************************************************************
based on the example sketch for driving WS2801 pixels
*****************************************************************************/

int dataPin = 12;
int clockPin = 11;
int indicatorPin = 8; // LED indicator on the D15 node

// Set the first variable to the NUMBER of pixels. 25 = 25 pixels in a row
WS2801 strip = WS2801(42, dataPin, clockPin);

void setup()
{ 
  // set up the indicator for output 
  pinMode( indicatorPin, OUTPUT );
  digitalWrite( indicatorPin, LOW );

  // set up the strip
  strip.begin();

  // Update LED contents, to start they are all 'off'
  strip.show();
  // ready light
  allColored( Color( 0, 0, 0 ));
  setLED( 0, Color( 0, 255, 0 ));

  // start Serial IO
  Serial.begin( 9600 );
}

// Create a 24 bit color value from R,G,B
uint32_t Color(byte r, byte g, byte b)
{
  uint32_t c;
  c = r;
  c <<= 8;
  c |= g;
  c <<= 8;
  c |= b;
  return c;
}

void setLED( int idx, uint32_t c ){
  strip.setPixelColor( idx, c );
  strip.show();
}

int getSerial()
{
  while( !Serial.available() );
  return Serial.read();
}

void allColored( uint32_t c )
{
  int i;
  for (i=0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c); 
  }
  strip.show();
}

void loop()
{
  int cmd; int ii, rr,gg,bb;

  // blink the indicator LED
  digitalWrite( indicatorPin, millis() & 0x80 );

  if( Serial.available() ) {
    cmd = Serial.read();

    // current commands:
    // p<index><red><green><blue>
    //     set a single pixel a color
    // f<red><green><blue>
    //     flood fill with this color

    if( cmd == 'p' ) {
      ii = getSerial();
      rr = getSerial();
      gg = getSerial();
      bb = getSerial();
      setLED( ii, Color(rr, gg, bb) );
    }

    if( cmd == 'f' ) {
      rr = getSerial();
      gg = getSerial();
      bb = getSerial();
      allColored( Color(rr, gg, bb) ); 
    }
  }
}
