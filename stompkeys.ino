

/**
 * The code for my usb keyboard/guitar pedal
 * scott jeide
 */

#include <Bounce2.h>    // https://github.com/thomasfredericks/Bounce2/wiki
//#include <Keyboard.h>   //https://www.arduino.cc/reference/en/language/functions/usb/keyboard/

#define ARRAY_LENGTH(x) (sizeof(x) / sizeof(x[0]))

// The footswitch buttons
const int LeftButton = 6;     // D1 pin marker on teensy
const int RightButton = 5;    // D0

// The rotary switches that select which keys to send
// (Left ones are wrong and not wired up yet)
const int LTSwitch1 = 21;  // F0
const int LTSwitch2 = 20;  // F1
const int LBSwitch1 = 0;   // B0
const int LBSwitch2 = 1;   // B1

const int RTSwitch1 = 21;  // F0
const int RTSwitch2 = 20;  // F1
const int RBSwitch1 = 0;   // B0
const int RBSwitch2 = 1;   // B1


const int InputPins[] = {
  LeftButton,
  RightButton,
  LTSwitch1,
  LTSwitch2,
  LBSwitch1,
  LBSwitch2,
};

typedef struct {
  const int pinNumber;
  const unsigned int keyCount;
  const unsigned int* keys;
} KeyMap;


/* The key sequence(s) to be sent when the given button is pressed
 * The TSwitches are typically for the key modifiers & the BSwitches are typically for the actual keys
 * It doesn't really matter since all of the Left keys will be sent when the Left button is pressed, etc
 * Just put a single value of 0 in the array to not send anything for that switch selection
 */
const unsigned int LTS1Keys[] = {0};
const unsigned int LTS2Keys[] = {0}; //{MODIFIERKEY_SHIFT, KEY_LEFT_CTRL};
const unsigned int LBS1Keys[] = {'z'};
const unsigned int LBS2Keys[] = {'/'};


const unsigned int RTS1Keys[] = {KEY_LEFT_SHIFT};
const unsigned int RTS2Keys[] = {0}; //{MODIFIERKEY_SHIFT, KEY_LEFT_CTRL};
const unsigned int RBS1Keys[] = {'z', 'y'};
const unsigned int RBS2Keys[] = {'/'};

const KeyMap LeftSwitches2Keys[] = {
  {LTSwitch1, ARRAY_LENGTH(LTS1Keys), LTS1Keys},
  {LTSwitch2, ARRAY_LENGTH(LTS2Keys), LTS2Keys},
  {LBSwitch1, ARRAY_LENGTH(LBS1Keys), LBS1Keys},
  {LBSwitch2, ARRAY_LENGTH(LBS2Keys), LBS2Keys},
};

const KeyMap RightSwitches2Keys[] = {
  {RTSwitch1, ARRAY_LENGTH(RTS1Keys), RTS1Keys},
  {RTSwitch2, ARRAY_LENGTH(RTS2Keys), RTS2Keys},
  {RBSwitch1, ARRAY_LENGTH(RBS1Keys), RBS1Keys},
  {RBSwitch2, ARRAY_LENGTH(RBS2Keys), RBS2Keys},
};


Bounce leftButtonBounce = Bounce(); 
Bounce rightButtonBounce = Bounce();


// using this to flash the LED in various patterns while getting things figured out
void flashLed(int times) {
  for (int n=0; n < times; n++) {
    digitalWrite(LED_BUILTIN, 1);
    delay(250);
    digitalWrite(LED_BUILTIN, 0);
    delay(500);
  } 
}


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
  
  // get the keyboard ready
  Keyboard.begin();
}


void sendKeys(const KeyMap* keyMap, unsigned int switchCount) {
  for (unsigned int switchIdx = 0; switchIdx < switchCount; switchIdx++) {
    const KeyMap& currentMap = keyMap[switchIdx];

    // Key selector switches are pulled high when open, switched to low when they are selected
    int val = digitalRead(currentMap.pinNumber);
    if (val == LOW) {
      // Send the corresponding keys for this switch
      for (unsigned int keyIdx = 0; keyIdx < currentMap.keyCount; keyIdx++) {
        if (currentMap.keys[keyIdx] != 0) {
          Keyboard.press(currentMap.keys[keyIdx]);
        }
      }
    }
  }
  
  Keyboard.releaseAll();
}

void loop() {
  leftButtonBounce.update();
  if (leftButtonBounce.fell()) {
    flashLed(1);
    sendKeys(LeftSwitches2Keys, ARRAY_LENGTH(LeftSwitches2Keys));    
  }

  rightButtonBounce.update();
  if (rightButtonBounce.fell()) {   
    flashLed(1);
    sendKeys(RightSwitches2Keys, ARRAY_LENGTH(RightSwitches2Keys));
  }
}
