#include <Wire.h>
#include "Adafruit_MPR121.h"
#include <MicroOscSlip.h>

#ifndef _BV
#define _BV(bit) (1 << (bit)) 
#endif

// Create an instance of the MPR121 and OSC library
Adafruit_MPR121 cap = Adafruit_MPR121();
MicroOscSlip<128> monOsc(&Serial);  // OSC communication over Serial

// Keeps track of the last pins touched
uint16_t lasttouched = 0;
uint16_t currtouched = 0;

void setup() {
  // Initialize serial communication for debugging and OSC
  Serial.begin(115200); // Match this baud rate in TouchDesigner
  
  while (!Serial) { 
    delay(10); // Wait for serial to initialize
  }
  
  // Serial.println("Adafruit MPR121 Capacitive Touch sensor test");
  
  // Initialize the MPR121 with its I2C address
  if (!cap.begin(0x5A)) { // Default I2C address of MPR121
    // Serial.println("MPR121 not found, check wiring?");
    while (1); // Halt execution if sensor is not found
  }
  // Serial.println("MPR121 found!");
}
void touchdetector(char * addr,int i){
   if ((currtouched & _BV(i)) && !(lasttouched & _BV(i))) {
      monOsc.sendInt(addr, 1);
    }
    else if (!(currtouched & _BV(i)) && (lasttouched & _BV(i))) {
      monOsc.sendInt(addr, 0);
    }
}
void loop() {
  // Get the current touch state of the MPR121
  currtouched = cap.touched();
  touchdetector("/touch0", 0);

  // Check each of the 12 touch inputs
 /* for (uint8_t i = 0; i < 12; i++) {
    // If a pad is touched now but wasn't before
    if ((currtouched & _BV(i)) && !(lasttouched & _BV(i))) {
      // Send a touch event via OSC
      monOsc.sendInt("/touch", i); // "/touch" message with the pad index
      // Serial.print(i); Serial.println(" touched");
    }
    if (!(currtouched & _BV(i)) && (lasttouched & _BV(i))) {
      monOsc.sendInt("/release", i); // "/release" message with the pad index
      // Serial.print(i); Serial.println(" released");
    }

  }*/
  // Update the last touch state
  lasttouched = currtouched;
  // Add a small delay for stability
  delay(20);
}
