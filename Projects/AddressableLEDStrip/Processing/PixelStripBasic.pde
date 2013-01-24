
/*
 * Pixel Strand Driver (Processing.pde)
 * Scott Lawrence yorgle@gmail.com January 2013
 */
import processing.serial.*;

Serial myPort; // comms to the micro
PImage colorsPic; // color wheel image
color lastColor = color( 255 ); // last clicked color
PFont fnt; // font for display

void setup()
{ 
  // setup window
  size(450, 370 );
  // setup serial
  println( Serial.list()[0] );
  println( Serial.list()[1] );
  println( Serial.list()[2] );
  println( Serial.list()[3] );
  println( Serial.list()[4] );
  myPort = new Serial( this, Serial.list()[4], 9600 );

  // setup the pic
  colorsPic = loadImage("color_sphere.png");
  image(colorsPic, 0, 0);
  loadPixels();

  // setup the font
  fnt = loadFont( "Glass_TTY_VT220-20.vlw" );
  textFont( fnt );
  noSmooth();
}

// for main color display
color frameColor;
int selector = 0;
// for effects
float r,g,b;
int flash;

void draw()
{
  // fill with the last color
  background( lastColor );

  // draw the image over it
  image( colorsPic, 0, 0 );

  // draw the "selector" number (0..9)
  fill( lastColor );
  String s = "" + selector;
  text( s, 5, 20 );

  // if we're doing the "flash" animation, handle that now
  if( flash != 0 ) {
    if( flash == 1 ) {
      flash = 2;
      fillPixels( 1.0, 1.0, 1.0 );
    } else {
      flash = 1;
      fillPixels( 0.0, 0.0, 0.0 );
    }
  }
}

void keyPressed()
{
  // key 0-9 picks from the first 10 LEDs to change
  if( key >= '0' && key <='9' ) {
    selector = key - '0';
  }

  // key "a" means that the color picked will affect ALL LEDs 
  if( key == 'a' ) { selector = -1; }

  // key "F" will toggle flashing all of the LEDs
  if( key == 'f' ) {
    flash = (flash == 0)?1:0;
  }

  // keys r,g,b will toggle the mask to full intensity 
  if( key == 'r' ) { r=(r==1.0)?0.0:1.0; sendRGBMask(); }
  if( key == 'g' ) { g=(g==1.0)?0.0:1.0; sendRGBMask(); }
  if( key == 'b' ) { b=(b==1.0)?0.0:1.0; sendRGBMask(); }
}

// for the "full intensity" flags, send that down

void sendRGBMask()
{
  fillPixels( r, g, b );
}

// send down a "fill all pixels" command with the specified color
void fillPixels( float r, float g, float b )
{
  int rr = int( r * 254.0 );
  int gg = int( g * 254.0 );
  int bb = int( b * 254.0 );

  myPort.write( "f" );
  myPort.write( char(rr) );
  myPort.write( char(gg) );
  myPort.write( char(bb) );
}

// send down a "set this pixel" command with the specified color
void setPixel( int index, float r, float g, float b )
{
  int rr = int( r * 254.0 );
  int gg = int( g * 254.0 );
  int bb = int( b * 254.0 );

  myPort.write( "p" );
  myPort.write( char(index) );
  myPort.write( char(rr) );
  myPort.write( char(gg) );
  myPort.write( char(bb) );
}

// on mouse press, set the color under the cursor, and send it down
void mousePressed()
{
  lastColor = get( mouseX, mouseY );
  if( selector >= 0 ) {
    setPixel( selector, red(lastColor)/255.0,
                        green(lastColor)/255.0,
                        blue(lastColor)/255.0 );
  } else {
    fillPixels( red(lastColor)/255.0,
                green(lastColor)/255.0,
                blue(lastColor)/255.0 );
  }
} 

// handle dragging the mouse as well
void mouseDragged(){
  mousePressed();
}

