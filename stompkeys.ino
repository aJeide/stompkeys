

/**
 * The code for my usb keyboard/guitar pedal
 * scott jeide
 */

#include <Bounce2.h>    // https://github.com/thomasfredericks/Bounce2/wiki
#include <Keyboard.h>   //https://www.arduino.cc/reference/en/language/functions/usb/keyboard/

#define ARRAY_LENGTH(x) (sizeof(x) / sizeof(x[0]))

const int LeftButton = 6;     // D1 pin marker on teensy
const int RightButton = 5;    // D0
const int LeftTSwitch1 = 21;  // F0
const int LeftTSwitch2 = 22;  // F1
const int LeftBSwitch1 = 0;   // B0
const int LeftBSwitch2 = 1;   // B1

const int InputPins[] = {
  LeftButton,
  RightButton,
  LeftTSwitch1,
  LeftTSwitch2,
  LeftBSwitch1,
  LeftBSwitch2,
};


/* The key sequence(s) to be sent when the given button is pressed
 * The TSwitches are typically for the key modifiers 
 * The BSwitches are typically for the actual keys
 * It doesn't really matter since all of the L keys will be sent when the L button is pressed
 * Just put a single value of 0 in the array to not send anything for that switch selection
 */
const int LeftTSwitch1Keys[] = {0};
const int LeftTSwitch2Keys[] = {0}; //{MODIFIERKEY_SHIFT, KEY_LEFT_CTRL};
const int LeftBSwitch1Keys[] = {'z'};
const int LeftBSwitch2Keys[] = {'/'};


Bounce leftButtonBounce = Bounce(); 
Bounce rightButtonBounce = Bounce();


// using this to toggle the led for each button press while I get things going
int ledState = LOW;

void setup() {
  // setup the input pins
  for (unsigned int i = 0; i < ARRAY_LENGTH(InputPins); i++) {
    pinMode(InputPins[i], INPUT_PULLUP);
  }

  leftButtonBounce.attach(LeftButton, INPUT_PULLUP);
  leftButtonBounce.interval(25);

  rightButtonBounce.attach(RightButton, INPUT_PULLUP);
  rightButtonBounce.interval(25);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, ledState);
  
  // get the keyboard ready
  Keyboard.begin();
 
}



void sendKeys() {
  
}

void loop() {
  leftButtonBounce.update();
  if (leftButtonBounce.fell()) {
    ledState = !ledState;
    digitalWrite(LED_BUILTIN, ledState);

    // Read which keys selectors are set
    // Send the corresponding keys
    
    //Keyboard.press('n');
    //Keyboard.press(CTRL);
    //Keyboard.release();
  }

  rightButtonBounce.update();
  if (rightButtonBounce.fell()) {
    ledState = !ledState;
    digitalWrite(LED_BUILTIN, ledState);
  }
}
