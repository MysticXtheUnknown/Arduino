//for the proximity detector
//#include <Adafruit_APDS9960.h> 
#include <Arduino_APDS9960.h>

//for the neopixels
#include <Adafruit_NeoPixel.h>
//for touch sensors
#include "Adafruit_FreeTouch.h"

//#include <Keyboard.h> //keyboard functionality

#include <Wire.h> //for serial

//#include <Mouse.h> //for moues emulation

#include <HID-Project.h> //for media keys



//#include "Adafruit_TinyUSB.h" //for hid

//Adafruit_USB_HID_KEYBOARD usb_hid;

// Single Report (no ID) descriptor
//uint8_t const desc_hid_report[] = {TUD_HID_REPORT_DESC_KEYBOARD(HID_REPORT_ID(1))}; //HID

//Adafruit_APDS9960 apds; //proximity

//Arduino_APDS9960 apds;
//neopixels
Adafruit_NeoPixel pixels(2, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);

Adafruit_FreeTouch qt_1 = Adafruit_FreeTouch(1, OVERSAMPLE_4, RESISTOR_50K, FREQ_MODE_NONE);
Adafruit_FreeTouch qt_2 = Adafruit_FreeTouch(2, OVERSAMPLE_4, RESISTOR_50K, FREQ_MODE_NONE);

char alphabet[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

void flash(uint16_t n, uint8_t r, uint8_t g,uint8_t b) {

  pixels.setPixelColor(n,r,g,b);
  pixels.show();
  delay(10);
  pixels.clear();
  pixels.show();
}

void setup() {
  //HID
  Consumer.begin();
  Keyboard.begin();
  //usb_hid.begin();
  //usb_hid.setPollInterval(2);

  // Set up HID report descriptor
  //usb_hid.setReportDescriptor(desc_hid_report, sizeof(desc_hid_report));

  //the rest:

  Serial.begin(9600);
  Mouse.begin();
  // put your setup code here, to run once:
  APDS.begin();
  //apds.enableProximity(true); //known conflict with gestures.
  ///delay(100);
  //apds.enableGesture(true);
  //delay(100);
  
  //apds.setProxGain(APDS9960_PGAIN_4X);

  qt_1.begin(); //TOUCH SENSORS
  qt_2.begin();
  
  pixels.begin();
  pixels.setBrightness(100);
  pixels.setPixelColor(0,5,0,5);
  //pixels.Color(0,255,0);
  pixels.show();
  delay(1000);
  pixels.clear();
  pixels.show();
  //Keyboard.print("#"); //insert the pound for the start of the typing.
  
//  delay(1000);
}

int letter = 0;
String mode = "keyb";
int proximity = 0;
int r = 0;
int g = 0;
int b = 0;

void fix_letter() {
    if (letter < 0) {letter = 25; }
    if (letter > 25) {letter = 0; }
  }

void loop() {
  //DO NOT ENABLE OR HAVE TO REFLASH THE ARDUINO by dragging the circuit piython file.

  int gesture = -1;

  if (mode == "keyb")
  {
    pixels.setPixelColor(0,0,5,0);
    pixels.show();
  } 
  else if (mode == "mouse")
  {
    pixels.setPixelColor(0,0,0,5);
    pixels.show();
  }
  else if (mode == "media")
  {
    pixels.setPixelColor(0,0,5,5);
    pixels.show();
  }
  //pixels.show();
//  apds.resetCounts(); 
  //proximity = APDS.readProximity(); //value of 0 - 51ch###efrom 6" - 0"
  
  if (APDS.proximityAvailable()) {
    proximity = APDS.readProximity();
  }
  
  //delay(50);
  Serial.println(proximity);   //TEST PROXIMITY
  //delay(500);

  //int gesture = APDS.readGesture();
  
  if (APDS.gestureAvailable()) {
    gesture = APDS.readGesture();
  }
/*
  if (APDS.colorAvailable()) {
    APDS.readColor(r, g, b);
    Serial.print("R");
    Serial.print(r);
    Serial.print("G");
    Serial.print(g);
    Serial.print("B");
    Serial.println(b);
    pixels.setPixelColor(1,r/3,g/3,b/3);
    pixels.show();
  }
  */
  uint16_t touch1 = qt_1.measure();
  uint16_t touch2 = qt_2.measure();

if (touch1 > 500 || touch2 > 500)
{
  delay(100);
  touch1 = qt_1.measure();
  touch2 = qt_2.measure();
}
 //delay(100);
 //Serial.println(proximity);
/*
if (proximity > 200 && proximity < 240 && gesture == -1) //mouse wheel up
  { if (mode == "keyb") {
     
      letter--;
      fix_letter();
      Keyboard.write(8);
      Keyboard.print(alphabet[letter]);
      delay(750);
      
  } else if (mode == "mouse") {
      Mouse.move(0,0,1);  //mouse wheel
      delay(250);
  } else if (mode == "media") {
      Consumer.write(MEDIA_VOLUME_UP);
      delay(200);
  }
} else if (proximity > 150 && proximity < 200 && gesture == -1) {
  if (mode == "keyb") {
    
    letter++;
    fix_letter();
    Keyboard.write(8);
    Keyboard.print(alphabet[letter]);
    delay(750);
    
  }
  else if (mode == "mouse") {
    Mouse.move(0,0,-1);
    delay(250);
  } else if (mode == "media") {
    Consumer.write(MEDIA_VOLUME_DOWN);
    delay(200);
  }
} 
*/

if (touch1 > 500 && touch2 < 500) { //touch1, change mode
  if (mode == "keyb") { mode = "mouse"; } else if (mode == "mouse") { mode = "media"; }
  else if (mode == "media") {mode = "keyb"; }
  Serial.println(mode);
  flash(0,5,5,0); //yellow
  delay(500);
  

} else if (touch2 > 500 && touch1 < 500) { // 'enter
    if (mode == "keyb") {
      Keyboard.println("");
      flash(1,0,5,0); //green?
      delay(500);
    } else if (mode == "mouse") {
      Mouse.click();
    } else if (mode == "media") { //touch2 pause/play
      Consumer.write(MEDIA_PLAY_PAUSE);
      delay(200);
      }
} else if (touch1 > 500 && touch2 > 500) {//double touch
    if (mode == "mouse")
    {
      Mouse.click(MOUSE_RIGHT);
    } else if (mode == "media") {
      Consumer.write(MEDIA_VOLUME_MUTE);
      delay(200);
    }

} else if (gesture == GESTURE_DOWN) { //left
    if (mode == "keyb") {
      Keyboard.write(8);
      flash(1,5,0,0);
    } else
    if (mode == "mouse") {
      Mouse.move(-30,0,0);
    } else if (mode == "media") {
      Consumer.write(MEDIA_VOLUME_DOWN);
      delay(100);
    }

} else if (gesture == GESTURE_UP) { //right

    if (mode == "keyb") { //space
      Keyboard.print(" "); 
      flash(1,5,0,0);
    } else if (mode == "mouse") { //move to right
      Mouse.move(30,0,0);
    } else if (mode == "media") { //vol up
      Consumer.write(MEDIA_VOLUME_UP);
      delay(100);
    }
    //pixels.setPixelColor(1,5,0,0);

} else if (gesture == GESTURE_LEFT) {//green is up away from me cycle further
    //UP/AWAY
    
    if (mode == "keyb") {
      letter++;
      fix_letter();
      Keyboard.write(8);
      Keyboard.print(alphabet[letter]);
      flash(1,0,5,0);
    } else if (mode == "mouse") {
      Mouse.move(0,-30,0);
    } else if (mode == "media") {
      Consumer.write(MEDIA_NEXT);
      delay(100);
    }

     // pixels.setPixelColor(1,0,5,0);
} else if (gesture == GESTURE_RIGHT) {//white Towards me, DN //cycle back
    if (mode == "keyb") {
      letter--;
      fix_letter();
      Keyboard.write(8);
      Keyboard.print(alphabet[letter]);
      flash(1,5,0,5);
    } else if (mode == "mouse") {
      Mouse.move(0,30,0);
    } else if (mode == "media") {  //gesture towards 
      Consumer.write(MEDIA_PREV);
      delay(100);
    }

}
} //ends main function

/*
 CONSUMER_POWER = 0x30,  
  CONSUMER_SLEEP = 0x32,  

  MEDIA_RECORD = 0xB2,  
  MEDIA_FAST_FORWARD = 0xB3,  
  MEDIA_REWIND = 0xB4,  
  MEDIA_NEXT = 0xB5,  
  MEDIA_PREVIOUS = 0xB6,  
  MEDIA_PREV = 0xB6, // Alias  
  MEDIA_STOP = 0xB7,  
  MEDIA_PLAY_PAUSE = 0xCD,  
  MEDIA_PAUSE = 0xB0,  

  MEDIA_VOLUME_MUTE = 0xE2,  
  MEDIA_VOL_MUTE = 0xE2, // Alias  
  MEDIA_VOLUME_UP = 0xE9,  
  MEDIA_VOL_UP = 0xE9, // Alias  
  MEDIA_VOLUME_DOWN = 0xEA,  
  MEDIA_VOL_DOWN = 0xEA, // Alias  

  CONSUMER_BRIGHTNESS_UP = 0x006F,  
  CONSUMER_BRIGHTNESS_DOWN = 0x0070,  

  CONSUMER_SCREENSAVER = 0x19e,  

  CONSUMER_PROGRAMMABLE_BUTTON_CONFIGURATION = 0x182,  
  CONSUMER_CONTROL_CONFIGURATION = 0x183,  
  CONSUMER_EMAIL_READER = 0x18A,  
  CONSUMER_CALCULATOR = 0x192,  
  CONSUMER_EXPLORER = 0x194,  

  CONSUMER_BROWSER_HOME = 0x223,  
  CONSUMER_BROWSER_BACK = 0x224,  
  CONSUMER_BROWSER_FORWARD = 0x225,  
  CONSUMER_BROWSER_REFRESH = 0x227,  
  CONSUMER_BROWSER_BOOKMARKS = 0x22A
  */