#define RED 3
#define GRE 5
#define BLU 6

String inputString = "";      // a String to hold incoming data
/*
  The expected String is on this model
        LFF00FF*
  Start ^  î    ^ end
           L message 
*/
bool stringComplete = false;
char breaker = '*';

// create a class structure for colors
class Colors {
  public:
  int red;
  int green;
  int blue;
};
// declare one 
Colors color;

void setRgb (int r = 255, int g = 0, int b = 0){
  analogWrite(RED, r);
  analogWrite(GRE, g);
  analogWrite(BLU, b);
}

// parse the string and update color object
void getColors (String data){
  long number = (long) strtol( &data[1], '*', 16); 
  color.red = number >> 16;
  color.green = number >> 8 & 0xFF;
  color.blue = number & 0xFF;
}

void setup() {
  pinMode(RED, OUTPUT);
  pinMode(GRE, OUTPUT);
  pinMode(BLU, OUTPUT);

  Serial.begin(19200);

  // allocate buffer memory for manipulation
  inputString.reserve(200);
}

void loop() {
  if (stringComplete) {

    // check if input is a rgbLed information 
    if(inputString.charAt(0) == 'L'){
      // update color object with string we just received
      getColors(inputString);
      // update rgbLed display
      setRgb(color.red,color.green,color.blue);
    }
    
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
}

//auto execute between loops
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    inputString += inChar;
    // if char is breaker, execute code in main loop
    if (inChar == breaker) {
      stringComplete = true;
    }
  }
}