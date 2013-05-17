

// set these to the time you download/start running the sketch
#define kStartHour (1)
#define kStartMinute (48)

// pins for the enable lines on the display (one per digit)
int enables[11] = { 6, 5, 4, 3 ,2, A5, A4, A3, A2, A1, A0 };

// pins for the segment lines on the display (one per segment a-h)
int segments[8] = { 12, 11, 10, 9, 8, 7, 0, 1 /* dp */ };


// turn on a digit
void enableDigit( int d )
{
  // turn off all others
  for( int a=0 ; a<11 ; a++ )
  {
    pinMode( enables[a], INPUT );
  }
  
  // turn on this one
  if( d >= 0 ) {
    pinMode( enables[ d ], OUTPUT );
    digitalWrite( enables[ d ] , LOW );
  }
}

// turn off a digit
void disableDigit( int d )
{
  pinMode( enables[d], INPUT );
}

// toffset is the time offset to start. computed at startup time
long toffset = 0;

// constants for computing a displayable time
long day = 86400000; // 86400000 milliseconds in a day
long hour = 3600000; // 3600000 milliseconds in an hour
long minute = 60000; // 60000 milliseconds in a minute
long second =  1000; // 1000 milliseconds in a second

//////////////////////////////////////////////////////////////////////////

void setup() {
  // set up output segments to be off
  for( int s=0 ; s<8 ; s++ )
  {
    pinMode( segments[s], OUTPUT );
    digitalWrite( segments[s], LOW );
  }
  
  // compute start time
  toffset = (day * 0) + (hour * kStartHour) + (minute * kStartMinute);
  
  // clear all digits
  enableDigit( -1 );
}

// seven segment display
//
//      1
//    4   5
//      2
//    0   6
//      3    7

// lookup table
// the layout is above of the segments.
// [0] if 1, turns on the bottom left segment, as seen above
// [7] if 1, turns on the decimal point
// [8] is the byte used for finding the associated LUT row in the list
unsigned char lut[][9] = {
   /* 0 */ { 1, 1, 0, 1, 1, 1, 1, 0, '0' }, 
   /* 1 */ { 0, 0, 0, 0, 0, 1, 1, 0, '1' },
   /* 2 */ { 1, 1, 1, 1, 0, 1, 0, 0, '2' },
   /* 3 */ { 0, 1, 1, 1, 0, 1, 1, 0, '3' },
   /* 4 */ { 0, 0, 1, 0, 1, 1, 1, 0, '4' },
   /* 5 */ { 0, 1, 1, 1, 1, 0, 1, 0, '5' },
   /* 6 */ { 1, 1, 1, 1, 1, 0, 1, 0, '6' },
   /* 7 */ { 0, 1, 0, 0, 0, 1, 1, 0, '7' },
   /* 8 */ { 1, 1, 1, 1, 1, 1, 1, 0, '8' },
   /* 9 */ { 0, 1, 1, 1, 1, 1, 1, 0, '9' },
   /*   */ { 0, 0, 0, 0, 0, 0, 0, 0, ' ' }, /* 10 */
   /* . */ { 0, 0, 0, 0, 0, 0, 0, 1, '.' }, /* 11 */
   /* - */ { 0, 0, 1, 0, 0, 0, 0, 0, '-' },
   /* a */ { 1, 1, 1, 0, 1, 1, 1, 0, 'a' },
   /* b */ { 1, 0, 1, 1, 1, 0, 1, 0, 'b' },
   /* c */ { 1, 0, 1, 1, 0, 0, 0, 0, 'c' },
   /* d */ { 1, 0, 1, 1, 0, 1, 1, 0, 'd' },
   /* e */ { 1, 1, 1, 1, 1, 0, 0, 0, 'e' },
   /* f */ { 1, 1, 1, 0, 1, 0, 0, 0, 'f' },
   /* h */ { 1, 0, 1, 0, 1, 0, 1, 0, 'h' },
   /* l */ { 1, 0, 0, 1, 1, 0, 0, 0, 'l' },
   /* o */ { 1, 0, 1, 1, 0, 0, 1, 0, 'o' },
           { 0, 0, 0, 0, 0, 1, 0, 1, '!' },
           { 0, 1, 0, 0, 0, 1, 0, 1, '?' },
           
//      1
//    4   5
//      2
//    0   6
//      3    7
           // Animations.
           { 1, 0, 0, 0, 0, 0, 0, 0, 'A' }, // clockwise circle animation
           { 0, 0, 0, 0, 1, 0, 0, 0, 'B' },
           { 0, 1, 0, 0, 0, 0, 0, 0, 'C' },
           { 0, 0, 0, 0, 0, 1, 0, 0, 'D' },
           { 0, 0, 0, 0, 0, 0, 1, 0, 'E' },
           { 0, 0, 0, 1, 0, 0, 0, 0, 'F' },
           
           { 1, 0, 0, 0, 0, 0, 0, 0, 'G' }, // figure 8 animation
           { 0, 0, 1, 0, 0, 0, 0, 0, 'H' },
           { 0, 0, 0, 0, 0, 1, 0, 0, 'I' },
           { 0, 1, 0, 0, 0, 0, 0, 0, 'J' },
           { 0, 0, 0, 0, 1, 0, 0, 0, 'K' },
           { 0, 0, 1, 0, 0, 0, 0, 0, 'L' },
           { 0, 0, 0, 0, 0, 0, 1, 0, 'M' },
           { 0, 0, 0, 1, 0, 0, 0, 0, 'N' },
           
           { 0, 0, 0, 1, 0, 0, 0, 0, 'O' }, // bar up
           { 1, 0, 0, 1, 0, 0, 1, 0, 'P' },
           { 1, 0, 1, 1, 0, 0, 1, 0, 'Q' },
           { 1, 0, 1, 1, 1, 1, 1, 0, 'R' },
           { 1, 1, 1, 1, 1, 1, 1, 0, 'S' },
           { 1, 1, 1, 0, 1, 1, 1, 0, 'T' }, // empty from bottom
           { 0, 1, 1, 0, 1, 1, 0, 0, 'U' },
           { 0, 1, 0, 0, 1, 1, 0, 0, 'V' },
           { 0, 1, 0, 0, 0, 0, 0, 0, 'W' },
           { 0, 0, 0, 0, 0, 0, 0, 0, 'X' },

   { -1 },
};

// our buffer.  make it bigger than 11 digits for safety, and for decimal point overlapping
char buffer[32] = "hello.";


// look for the character specified in the lookup table
// return the index
int findInLut( char ch )
{
  if( (ch & 0x80) == 0x80 ) ch &=0x7f;
  
  for( int i=0 ; lut[i][0] != -1 ; i++ )
  {
    if( ch == lut[i][8] ) return i;
  }
  /* not found. return something */
  return findInLut( '-' );
}

// pad out short strings to fill 11 digits with spaces
void pad( char * buf, int len )
{
  int nulled = 0;
  for( int i=0 ; i<len ; i++ )
  {
    if( buf[i] == '\0' ) nulled = 1;
    if( nulled )  buf[i] = ' ';
  }
}

// take the decimal points in the string, and 
// compress them onto the previous character by hinting them
void dpHint( char * buf, int len )
{
  for( int i=0 ; i < len ; i++ )
  {
    if( buf[i+1] == '.' ) {
      buf[i++] |= 0x80;
      // shift the rest of the buffer down
      for( int j=i ; j < len ; j++ ) {
        buf[j] = buf[j+1];
      }
    }
  }
}


// convert the buffer to segments on the display
void segDisplay( char * buf )
{ 
  /* foreach digit */
  for( int d=0 ; d<11 ; d++ )
  {
    // find the index in the LUT of the current character
    int ch = findInLut( buf[d] );
    
    // once we found it, turn on the segments as we need them
    for( int s=0 ; s<8 ; s++ )
    {
      digitalWrite( segments[s], lut[ch][s] );
    }
    
    // convert the decimal point hint back to the actual DP dot
    if( (buffer[d] & 0x80) == 0x80 ) {
      digitalWrite( segments[7],  HIGH );
    }
  
    // show it quickly
    enableDigit( d );
    delay( 1 );
    disableDigit( d );
  }
}


// fill the buffer with the current time with flashing dash for display
void fill_time()
{
  long timeNow = millis() + toffset;
  
  int days = timeNow / day ;                                //number of days
  int hours = (timeNow % day) / hour;                       //the remainder from days division (in milliseconds) divided by hours, this gives the full hours
  int minutes = ((timeNow % day) % hour) / minute ;         //and so on...
  int seconds = (((timeNow % day) % hour) % minute) / second;
 
  int c = ((millis()/500)&1)?'-':' ';
  
  sprintf( buffer, " %2d%c%02d%c%02d", hours, c, minutes, c, seconds );
}

// fill the buffer with circle animation A
void fill_anim_A()
{
  for( int i=0 ; i<11 ; i++ )
  {
    // simple animation
    buffer[i] = 'A' + ((millis()/40 + i) % 6); // circle animation 
  }
}

// fill the buffer with figure-8 animation G
void fill_anim_G()
{
  for( int i=0 ; i<11 ; i++ )
  {
    // simple animation
    buffer[i] = 'G' + ((millis()/50 +i) % 8); // figure 8 animation
  }
}

void fill_anim_O()
{
  for( int i=0 ; i<11 ; i++ )
  {
    // simple animation
    buffer[i] = 'O' + ((millis()/30 +i) % 10); // figure 8 animation
  }
}

void fill_anim_dash()
{
  int nit = (millis()/30) % 22;
  
  for( int i=0 ; i<11 ; i++ )
  {
    // simple animation
    
    if( nit < 11 ) {
      buffer[i] = (i<nit)?'-':' ';
    } else {
      buffer[i] = (i<(nit-11))?' ':'-';
    }
  }
}


// main arduino loop point
void loop() 
{
  // one animation step per 4 seconds (4000 ms)
  int animationSelector = (millis()/3000) % 8;
  
  // fill the buffer
  switch( animationSelector ) {
    case( 0 ): case( 2 ):  case( 4 ): case( 6 ): fill_time(); break;
    case( 1 ): fill_anim_A(); break;
    case( 3 ): fill_anim_O(); break;
    case( 5 ): fill_anim_G(); break;
    case( 7 ): fill_anim_dash(); break;
  }
  
  // convert decimal points to hints in the string
  dpHint( buffer, 11 );
  
  // pad out the string to fill 11 characters
  pad( buffer, 11 );
  
  // show the buffer to the display
  segDisplay( buffer );  
}

