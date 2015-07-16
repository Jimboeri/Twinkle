
/*
This software program controls the LED's in the passageway/hall and makes them 'Twinkle'
An external potentiometer in the hall controls the brightness of the arrays.
The LED's are arranged in groups of 4, i.e. there are 4 separately controlled LED strings

The system can also accept controls from the serial channel and output status messages to the serial channel
The commands accepted are:

SYNC - Synchronise flickering on all strings
RAND - Randomise flickering (default operation)
SLOW - Slow fade in & out of all strings
MAX - Pot set at max limit, update range accordingly
ON - all LED's on
OFF - All LED's off
SHOW - Show main parameters

*/

// Include libraries

//Constant definitions
const char* VERSION_STRING = "Passageway Lights Twinkler: Version 1.0 16/7/2015";
const char* PROMPT = "Cmd>";
const int maxPWMvalue = 4095;

//Light mode values. Each light will have one of these modes defined
const int OFF = 0;
const int ON = 1;
const int UP = 2;
const int DOWN = 3;


// Define structure associated with string of lights
struct LIGHT_STRUCT{
  int pinNumber;                  // Specifies the pin number to which the light (or light driver) is attached
  int PWMValue ;                  // Contains this light's PWM value from 0 (off) to 4095 (full on).
  int currentMode;				// Current mode for light string
  int msCycle;					// milliseconds per cycle
  int msStep;					// milliseconds per step
};

LIGHT_STRUCT lights[]= {
  0,0,UP,200,10,
  1,0,UP,180,10,
  2,0,UP,220,10,
  3,0,UP,240,10,
  4,0,UP,160,10};  
  

void setup(){
  // Tell anyone who cares that we are in here...
  Serial.begin(9600);
  sprintf(buffer,"\n\n\r%s\n\r%s",VERSION_STRING,PROMPT);
  Serial.print(buffer);
}
  

void loop() {

}

  

void doSerialInput(){
  byte aChar;
  // Check for a Serial command.
  if (Serial.available() >0) {
    // get the char
    aChar=Serial.read();
    if (processKeyboardChar(aChar)){
      // It was a terminator. Tail the buffer and parse it
      Serial.println();
      inputBuffer[inputBufferPtr++]=0;
      doInputBufferParser();
      inputBufferPtr=0;
      inputBuffer[0]=0;
      Serial.print(PROMPT);
    }
  }
}

int processKeyboardChar(char theChar) {
  /*
    This function actions an input character. Buffers the char if it's
   a printable one. If it's a control char, then actions it or ignores
   it. If it's a CR char, then returns 1, else it returns zero.
   */
  if ((32==theChar) || (theChar=='*') || (isalnum(theChar))){
    /*
    This is a printable char just buffer and echo the char if that
     would not make the string too long. Convert to upper case if 
     alphabetic.
     */
    if (isalpha(theChar)){
      bitClear(theChar,5);
    }
    if (inputBufferPtr < sizeof(inputBuffer)){
      inputBuffer[inputBufferPtr++]=theChar;
      inputBuffer[inputBufferPtr]=0; 
      Serial.print(theChar);
      return(0);
    }
  }  
  else
  {
    // It's a control character
    switch  (theChar) {

    case 13: // Carriage return (new line)
      return(1);
      break;

    case 18: // CTRL/R
      Serial.println("^R"); // New line
      Serial.print(inputBuffer);
      break;

    case 8: // Backspace
      // Only process this if there is something in the buffer.
      if (inputBufferPtr > 0) {
        inputBuffer[--inputBufferPtr]=0; // Kill the char in the buffer.
        Serial.print(theChar); // Send the backspace on screen
        Serial.print(" ");
        Serial.print(theChar);
      }
      break;
    }
    return(0);
  }
}

void doSerial2 () {

  // Input serial information:
  if (Serial.available() > 0){
    inByte = Serial.read();
    // only input if a letter, number, =,?,+ are typed!
    if ((inByte >= 65 && inByte <= 90) || (inByte >=97 && inByte <=122) || (inByte >= 48 && inByte <=57) || inByte == 43 || inByte == 61 || inByte == 63) {
      command.concat(inByte);
    }
  }// end serial.available
