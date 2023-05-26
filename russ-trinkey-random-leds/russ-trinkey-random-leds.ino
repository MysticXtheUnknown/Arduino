//neo trinkey, mood light, adjust brightness with touch1, touch2

#include "Adafruit_FreeTouch.h"
#include <Adafruit_NeoPixel.h>
//for the proximity detector
//#include <Adafruit_APDS9960.h> 
//#include "random.h"

// Create the two touch pads on pins 1 and 2:
Adafruit_FreeTouch qt_1 = Adafruit_FreeTouch(1, OVERSAMPLE_4, RESISTOR_50K, FREQ_MODE_NONE);
Adafruit_FreeTouch qt_2 = Adafruit_FreeTouch(2, OVERSAMPLE_4, RESISTOR_50K, FREQ_MODE_NONE);

//Adafruit_APDS9960 apds; //proximity

#define NUMPIXELS 4

Adafruit_NeoPixel pixels(NUMPIXELS, PIN_NEOPIXEL, NEO_RGB);

bool current_state = false;

  int red=100;
  int green=0;
  int blue=0;

void random_pixels() {
    red = red + random(3) -1;
    green = green + random(3) -1;
    blue = blue + random(3) -1;
    if (red < 0) { red = 0; }
    if (red > 255) { red = 255; }
    if (green < 0) { green = 0; }
    if (green > 255) { green = 255; }
    if (blue < 0) { blue = 0; }
    if (blue > 255) { blue = 255; } 
  for (int i=0 ; i<NUMPIXELS ; i++) {

    pixels.setPixelColor(i, pixels.Color(red,green,blue));  
  }
  pixels.show();
  delay(50);
}

void setup() {
  //apds.begin();
  //apds.enableProximity(true);
 // Keyboard.begin();
  pixels.begin();

  pixels.setPixelColor(0, pixels.Color(0,10,0));
  pixels.show();
  // put your setup code here, to run once:
  //pinMode(1,INPUT);
  qt_1.begin();
  qt_2.begin();
 // pixels.fill(0,0,255);
 // pixels.show();
  delay(1000);
  pixels.setBrightness(20);
}

int brightness = 10;

void loop() {

  // measure the captouches
  uint16_t touch1 = qt_1.measure();
  uint16_t touch2 = qt_2.measure();
  srand(touch1 + touch2);
  //int proximity = apds.readProximity(); //value of 0 - 51 from 6" - 0"
    // If the first pad is touched, reduce brightness
  delay(20);
  //pixels.setBrightness(proximity * 5);
  if (touch1 > 500 && touch2 < 500) {
    brightness = brightness - 10;
    if (brightness < 0) {brightness = 0; }
    pixels.setBrightness(brightness);
  } else if (touch2 > 500 && touch1 < 500) {
    brightness = brightness + 10;
    if (brightness > 255) { brightness=255; }
    pixels.setBrightness(brightness);
  }
 //  
      
  random_pixels();
  
}  