/* D15_Test
**
**   A simple sketch to try out using a D15
**   Will echo any serial sent to it
**   typing 0..9 will change the rate that the LED flashes on the unit.
*/

const int ledPin = 8;

void setup() {
  // initialize serial:
  Serial.begin(9600);
  // make the pins outputs:
  pinMode(ledPin, OUTPUT);
}

int timerval = 1;
long countdown = 0;
int mode = 0;

void loop()
{
  int ch = 0;
  
  if( mode == 0 ) digitalWrite( ledPin, HIGH );
  if( mode == 1 ) digitalWrite( ledPin, LOW );
  
  if( millis() > countdown ) {
    // reset the countdown
    countdown = millis() + (timerval * 100);
    
    // toggle the pin
    if( mode == 0 ) mode = 1;
    else mode = 0;
  }
    
  // if there's any serial available, read it:
  if (Serial.available() > 0) {
    ch = Serial.read();
    
    Serial.write( ch );
    
    if( ch <= '9' && ch >= '0' )
    {
      Serial.write( "\n changing timer value to " );
      Serial.write( ch );
      Serial.write( "\n" );
      timerval = ch - '0';
    }
  }
}








