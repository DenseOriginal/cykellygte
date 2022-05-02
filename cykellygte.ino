#include <Adafruit_NeoPixel.h>
#include <ezButton.h>

Adafruit_NeoPixel leftBlinker(2, 12, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel rightBlinker(2, 13, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel frontLight(20, 14, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel brakeLight(20, 27, NEO_GRBW + NEO_KHZ800);

ezButton rightSwitch(26);
ezButton leftSwitch(25);

int activeBlinkLight = 0;

const byte numberOfFlashes = 6;
static byte sequencePosition = numberOfFlashes * 2;

int lightMode = 0;

void setup() {
  leftBlinker.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  rightBlinker.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  
  frontLight.begin();
  brakeLight.begin();

  fillUsingLightMode();
  
  Serial.begin(9600);
  rightSwitch.setDebounceTime(5);
  leftSwitch.setDebounceTime(5);
}

void loop() {
  rightSwitch.loop();
  leftSwitch.loop();

  if(leftSwitch.isPressed()){
    sequencePosition = 0;
    activeBlinkLight = 0;

    // Brightmode switch
    if(!rightSwitch.getState()) {
      lightMode = (lightMode + 1) % 3;

      fillUsingLightMode();
    }
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
      if(ledState) {
        if(activeBlinkLight == 0)
          blinkLeft();
        else
          blinkRight();
      } else {
        clearBlinkers();
      } 
      
      ledState = !ledState;
      sequencePosition++;
    }
    else if (sequencePosition == numberOfFlashes * 2) {
      ledState = false;
      clearBlinkers();
    }
    previousMillis = currentMillis;
  }
}

void blinkRight() {
  rightBlinker.fill(rightBlinker.Color(255, 100, 0, 0));
  brakeLight.setPixelColor(0, brakeLight.Color(255, 100, 0, 0));
  brakeLight.setPixelColor(1, brakeLight.Color(255, 100, 0, 0));
  brakeLight.setPixelColor(18, brakeLight.Color(255, 100, 0, 0));
  brakeLight.setPixelColor(19, brakeLight.Color(255, 100, 0, 0));
  brakeLight.show();
}

void blinkLeft() {
  leftBlinker.fill(leftBlinker.Color(255, 100, 0, 0));
  brakeLight.setPixelColor(8, brakeLight.Color(255, 100, 0, 0));
  brakeLight.setPixelColor(9, brakeLight.Color(255, 100, 0, 0));
  brakeLight.setPixelColor(10, brakeLight.Color(255, 100, 0, 0));
  brakeLight.setPixelColor(11, brakeLight.Color(255, 100, 0, 0));
  brakeLight.show();
}

void clearBlinkers() {
  leftBlinker.clear();
  rightBlinker.clear();

  if(lightMode == 2) {
    brakeLight.fill(frontLight.Color(255, 0, 0, 0));
    brakeLight.show();
  } else if(lightMode == 1) {
    brakeLight.fill(frontLight.Color(175, 0, 0, 0));
    brakeLight.show();
  } else if(lightMode == 0) {
    brakeLight.clear();
    brakeLight.show();
  }
}

void fillUsingLightMode() {
  if(lightMode == 2) {
    frontLight.fill(frontLight.Color(255, 255, 255, 255));
    frontLight.show();
  
    brakeLight.fill(frontLight.Color(255, 0, 0, 0));
    brakeLight.show();
  } else if(lightMode == 1) {
    frontLight.fill(frontLight.Color(0, 0, 0, 175));
    frontLight.show();
  
    brakeLight.fill(frontLight.Color(175, 0, 0, 0));
    brakeLight.show();
  } else if(lightMode == 0) {
    frontLight.clear();
    frontLight.show();

    brakeLight.clear();
    brakeLight.show();
  }
}
