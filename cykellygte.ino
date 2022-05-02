#include <Adafruit_NeoPixel.h>
#include <ezButton.h>

Adafruit_NeoPixel leftBlinker(2, 12, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel rightBlinker(2, 13, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel frontLight(20, 14, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel brakeLight(20, 27, NEO_GRBW + NEO_KHZ800);

ezButton leftSwitch(25);
ezButton rightSwitch(26);

int activeBlinkLight = 0;

const byte numberOfFlashes = 6;
static byte sequencePosition = numberOfFlashes * 2;

void setup() {
  leftBlinker.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  rightBlinker.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  
  frontLight.begin();
  brakeLight.begin();

  frontLight.fill(frontLight.Color(255, 255, 255, 255));
  frontLight.show();

  brakeLight.fill(frontLight.Color(255, 0, 0, 0));
  brakeLight.show();
  
  Serial.begin(9600);
  leftSwitch.setDebounceTime(5);
  rightSwitch.setDebounceTime(5);
}

void loop() {
  leftSwitch.loop();
  rightSwitch.loop();

  if(leftSwitch.isPressed()){
    sequencePosition = 0;
    activeBlinkLight = 0;
  }

  if(rightSwitch.isPressed()) {
    sequencePosition = 0;
    activeBlinkLight = 1;
  }
  
  ledSequence();
  
  leftBlinker.show();   // Send the updated pixel colors to the hardware.
  rightBlinker.show();   // Send the updated pixel colors to the hardware.
}

void ledSequence() {
  const long interval = 250; // time between flashes in ms.
  static bool ledState = false;
  static unsigned long previousMillis = 0;
  unsigned long currentMillis = millis();
  
  if ((currentMillis - previousMillis >= interval)) {
    if (sequencePosition < numberOfFlashes * 2) { // blink
      if(ledState)
        if(activeBlinkLight == 0)
          leftBlinker.fill(leftBlinker.Color(255, 100, 0, 0));
        else
          rightBlinker.fill(rightBlinker.Color(255, 100, 0, 0));
      else {
        leftBlinker.clear();
        rightBlinker.clear();
      } 
      
      ledState = !ledState;
      sequencePosition++;
    }
    else if (sequencePosition == numberOfFlashes * 2) {
      ledState = false;
      leftBlinker.clear();
      rightBlinker.clear();
    }
    previousMillis = currentMillis;
  }
}
