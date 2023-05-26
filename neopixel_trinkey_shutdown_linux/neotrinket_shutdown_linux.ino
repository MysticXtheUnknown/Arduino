#include "Keyboard.h"
#include <Adafruit_NeoPixel.h>
#include "Adafruit_FreeTouch.h"

#define NUMPIXELS 4  // Increase the number of pixels to 2

Adafruit_NeoPixel pixels(NUMPIXELS, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);

// Create the two touch pads on pins 1 and 2:
Adafruit_FreeTouch qt_1 = Adafruit_FreeTouch(1, OVERSAMPLE_4, RESISTOR_50K, FREQ_MODE_NONE);
Adafruit_FreeTouch qt_2 = Adafruit_FreeTouch(2, OVERSAMPLE_4, RESISTOR_50K, FREQ_MODE_NONE);

// Input a value 0 to 255 to get a color value.
// The colors are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< pixels.numPixels(); i++) {
      pixels.setPixelColor(i, Wheel(((i * 256 / pixels.numPixels()) + j) & 255));

      uint16_t touch1 = qt_1.measure();
      uint16_t touch2 = qt_2.measure();

      if (touch1 > 500) {
        pixels.setBrightness(100);
        delay(500);
      }

      if (touch2 > 500) {
        pixels.setBrightness(5);
        delay(500);
      }

    }
    pixels.show();
    delay(wait);
  }
}


void setup() {
  //touch sensors
  qt_1.begin();
  qt_2.begin();

  // put your setup code here, to run once:
  
  pixels.begin();
  pixels.show();
  pixels.setBrightness(10);
  pixels.fill(255,0,0);
  pixels.show();
  //open a terminal and do sudo shutdown.
  
  delay(3000);
  Keyboard.press(KEY_LEFT_CTRL);
  delay(1000);
  Keyboard.press(KEY_LEFT_ALT);
  delay(1000);
  Keyboard.press('t');
  //delay(1000);
  Keyboard.releaseAll();
  delay(20 * 1000); //delay ten seconds
  Keyboard.println("sudo shutdown");
  delay(5 * 1000); //delay 5 seconds
  Keyboard.println("0000"); //in case it asks for the unix password on my own machine

  //show the leds

}

void loop() {
  



  rainbowCycle(20);
  // put your main code here, to run repeatedly:
  //delay(1000);
}
