/*************************************************** 
  This is an implementation of the game Lights Out for 4 Trellises
  Press a key to turn on/off the adjacent buttons, try to turn all
  the LEDs off!

  Designed specifically to work with the Adafruit Trellis 
  ----> https://www.adafruit.com/products/1616
  ----> https://www.adafruit.com/products/1611

  These displays use I2C to communicate, 2 pins are required to  
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Tony Sherwood for Adafruit Industries.  
  MIT license, all text above must be included in any redistribution
 ****************************************************/

#include <Wire.h>
#include "Adafruit_Trellis.h"
#include "Adafruit_NeoTrellis.h"

#define Y_DIM 12  //number of rows of key
#define X_DIM 4   //number of columns of keys
#define MOMENTARY 0
#define LATCHING 1
// set the mode here
#define MODE LATCHING

Adafruit_Trellis matrix0 = Adafruit_Trellis();
Adafruit_Trellis matrix1 = Adafruit_Trellis();
Adafruit_Trellis matrix2 = Adafruit_Trellis();
Adafruit_Trellis matrix3 = Adafruit_Trellis();
Adafruit_Trellis matrix4 = Adafruit_Trellis();
Adafruit_Trellis matrix5 = Adafruit_Trellis();
Adafruit_Trellis matrix6 = Adafruit_Trellis();
Adafruit_Trellis matrix7 = Adafruit_Trellis();

Adafruit_TrellisSet trellis = Adafruit_TrellisSet(&matrix0, &matrix1, &matrix2, &matrix3, &matrix4, &matrix5, &matrix6, &matrix7);

#define NUMTRELLIS 8

#define numKeys (NUMTRELLIS * 16)
// create a matrix of trellis panels
Adafruit_NeoTrellis t_array[Y_DIM / 4][X_DIM / 4] = {

  { Adafruit_NeoTrellis(0x46, &Wire1) },
  { Adafruit_NeoTrellis(0x36, &Wire1) },
  { Adafruit_NeoTrellis(0x2E, &Wire1) }


};
//pass this matrix to the multitrellis object
Adafruit_MultiTrellis neotrellis((Adafruit_NeoTrellis *)t_array, Y_DIM / 4, X_DIM / 4);
uint32_t Wheel(byte WheelPos) {
  if (WheelPos < 85) {
    return seesaw_NeoPixel::Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return seesaw_NeoPixel::Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
    WheelPos -= 170;
    return seesaw_NeoPixel::Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  return 0;
}
//define a callback for key presses
TrellisCallback blink(keyEvent evt) {

  if (evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING)
    neotrellis.setPixelColor(evt.bit.NUM, Wheel(map(evt.bit.NUM, 0, X_DIM * Y_DIM, 0, 255)));  //on rising
  else if (evt.bit.EDGE == SEESAW_KEYPAD_EDGE_FALLING)
    neotrellis.setPixelColor(evt.bit.NUM, 0);  //off falling

  neotrellis.show();
  return 0;
}
/*
  In order for this layout to work, you will have to have soldered your 4 boards
  together in the same order that I did.  Below is the top view of the circuit, with
  each board's address, and lines indicating which boards are connected to each other.
  If you soldered your boards together differently, you will need to edit the chessboard
  array below until the LED wipe when you start up your sketch turns on all of the LEDs
  in order.

             [0x73]--[0x72]
                       |
  [ARDUINO]--[0x70]--[0x71]
 
*/


// Connect Trellis Vin to 5V and Ground to ground.
// Connect the INT wire to pin #5 (can change later
#define INTPIN 41
// Connect I2C SDA pin to your Arduino SDA line
// Connect I2C SCL pin to your Arduino SCL line
// All Trellises share the SDA, SCL and INT pin!
// Even 8 tiles use only 3 wires max
// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;  // will store last time LED was updated

// constants won't change:
const long interval = 30;  // interval at which to blink (milliseconds)


void setup() {
  Serial.begin(9600);
  Serial.println("Trellis Lights Out");

  // INT pin requires a pullup
  pinMode(INTPIN, INPUT);
  digitalWrite(INTPIN, HIGH);

  trellis.begin(0x70, 0x72, 0x73, 0x74, 0x71, 0x75, 0x76, 0x77);

  if (!neotrellis.begin()) {
    Serial.println("failed to begin neotrellis");
    while (1) delay(1);
  }
  // light up all the LEDs in order
  for (uint8_t i = 0; i < numKeys; i++) {
    trellis.setLED(i);
    trellis.writeDisplay();
    delay(50);
  }
  // then turn them off
  for (uint8_t i = 0; i < numKeys; i++) {
    trellis.clrLED(i);
    trellis.writeDisplay();
    delay(50);
  }
  /* the array can be addressed as x,y or with the key number */
  for (int i = 0; i < Y_DIM * X_DIM; i++) {
    neotrellis.setPixelColor(i, Wheel(map(i, 0, X_DIM * Y_DIM, 0, 255)));  //addressed with keynum
    neotrellis.show();
    delay(50);
  }

  for (int y = 0; y < Y_DIM; y++) {
    for (int x = 0; x < X_DIM; x++) {
      //activate rising and falling edges on all keys
      neotrellis.activateKey(x, y, SEESAW_KEYPAD_EDGE_RISING, true);
      neotrellis.activateKey(x, y, SEESAW_KEYPAD_EDGE_FALLING, true);
      neotrellis.registerCallback(x, y, blink);
      neotrellis.setPixelColor(x, y, 0x000000);  //addressed with x,y
      neotrellis.show();                         //show all LEDs
      delay(50);
    }
  }
}



void loop() {
  //delay(2);  // 30ms delay is required, dont remove me!
  unsigned long currentMillis = millis();
  if (!digitalRead(INTPIN)) {
    neotrellis.read();
    Serial.println("reading neotrellis");
  }
  if (currentMillis - previousMillis >= interval) {

    // save the last time you blinked the LED
    previousMillis = currentMillis;
    if (MODE == MOMENTARY) {
      // If a button was just pressed or released...
      if (trellis.readSwitches()) {
        // go through every button
        for (uint8_t i = 0; i < numKeys; i++) {
          // if it was pressed, turn it on
          if (trellis.justPressed(i)) {
            Serial.print("v");
            Serial.println(i);
            trellis.setLED(i);
          }
          // if it was released, turn it off
          if (trellis.justReleased(i)) {
            Serial.print("^");
            Serial.println(i);
            trellis.clrLED(i);
          }
        }
        // tell the trellis to set the LEDs we requested
        trellis.writeDisplay();
      }
    }

    if (MODE == LATCHING) {
      // If a button was just pressed or released...
      if (trellis.readSwitches()) {
        // go through every button
        for (uint8_t i = 0; i < numKeys; i++) {
          // if it was pressed...
          if (trellis.justPressed(i)) {
            Serial.print("v");
            Serial.println(i);
            // Alternate the LED
            if (trellis.isLED(i))
              trellis.clrLED(i);
            else
              trellis.setLED(i);
          }
        }
        // tell the trellis to set the LEDs we requested
        trellis.writeDisplay();
      }
    }
  }
}
