#include <M5Atom.h>               // Include the M5Atom library
#include <FastLED.h>              // Include FastLED library
#include <MicroOscSlip.h>         // Include the MicroOsc library

CRGB pixel;                       // Define the RGB LED pixel
unsigned long monChronoMessages;  // Used to limit OSC message sending frequency
#define MA_BROCHE_ANGLE 32        // Define the analog input pin

// Define button pins
#define BUTTON_1_PIN 26
#define BUTTON_2_PIN 33
#define BUTTON_3_PIN 25

MicroOscSlip<128> monOsc(&Serial);

void setup() {
  M5.begin(false, false, false);                      // Initialize M5Atom without options
  Serial.begin(115200);                               // Start the serial connection
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(&pixel, 1);  // Add M5Atom's pixel to FastLED

  // Set up buttons as input
  pinMode(BUTTON_1_PIN, INPUT_PULLUP);
  pinMode(BUTTON_2_PIN, INPUT_PULLUP);
  pinMode(BUTTON_3_PIN, INPUT_PULLUP);

  // Startup animation
  while (millis() < 4000) {
    pixel = CRGB(255, 40, 0); // Red
    FastLED.show();
    delay(10);
    pixel = CRGB(0, 255, 0); // Green
    FastLED.show();
    delay(10);
  }
}

void maReceptionMessageOsc(MicroOscMessage& oscMessage) {
  if (oscMessage.checkOscAddress("/rgb")) {
    int r = oscMessage.nextAsInt();
    int g = oscMessage.nextAsInt();
    int b = oscMessage.nextAsInt();
    pixel = CRGB(r, g, b);
    FastLED.show();
  }
}

void loop() {
  M5.update();  // Always include M5.update() at the start of the loop
  monOsc.onOscMessageReceived(maReceptionMessageOsc);

  if (millis() - monChronoMessages >= 40) {
    monChronoMessages = millis();  // Reset the timer

    // Read the button states
    int button1State = digitalRead(BUTTON_1_PIN) == LOW ? 1 : 0;
    int button2State = digitalRead(BUTTON_2_PIN) == LOW ? 1 : 0;
    int button3State = digitalRead(BUTTON_3_PIN) == LOW ? 1 : 0;

    // Read the potentiometer value
    int maLectureAngle = analogRead(MA_BROCHE_ANGLE);

    // Send OSC messages
    monOsc.sendInt("/pot", maLectureAngle);
    monOsc.sendInt("/button1", button1State);
    monOsc.sendInt("/button2", button2State);
    monOsc.sendInt("/button3", button3State);
  }
}
