
#include <M5Atom.h>

CRGB pixel;

unsigned long monChronoMessages;

#include <MicroOscSlip.h>
MicroOscSlip<1024> monOsc(&Serial);

#include <M5_PbHub.h>
M5_PbHub myPbHub;




void maReceptionMessageOsc(MicroOscMessage& oscMessage) {

  if (oscMessage.checkOscAddress("/message")) {
  }
}


void setup() {
  // put your setup code here, to run once:
  M5.begin(false, false, false);
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(&pixel, 1);  // Ajouter le pixel du M5Atom à FastLED


  unsigned long chronoDepart = millis();
  while (millis() - chronoDepart < 5000) {
    pixel = CRGB(255, 255, 255);
    FastLED.show();
    delay(100);

    pixel = CRGB(255, 0, 0);
    FastLED.show();
    delay(100);
  }


  Serial.begin(115200);

  Wire.begin();

  myPbHub.begin();

  for (int outIndex = 0; outIndex < 6; outIndex++) {
    myPbHub.digitalWrite(outIndex, LOW);
  }
}




void loop() {
  // put your main code here, to run repeatedly:
  M5.update();

  monOsc.onOscMessageReceived(maReceptionMessageOsc);

  if (millis() - monChronoMessages >= 20) {
    monChronoMessages = millis();



// MAXIMUM DE 5 CHANNELS (25 BITS SUR 32 BITS MAX)
#define ACTIVE_CHANNELS 3

    bool maMatriceDeConnexions[ACTIVE_CHANNELS][ACTIVE_CHANNELS];

    int monInt32DeConnexions = 0;

    for (int outIndex = 0; outIndex < ACTIVE_CHANNELS; outIndex++) {
      myPbHub.digitalWrite(outIndex, HIGH);
      for (int inIndex = 0; inIndex < ACTIVE_CHANNELS; inIndex++) {
        int lecture = myPbHub.digitalRead(inIndex);
        monInt32DeConnexions = (monInt32DeConnexions << 1) | lecture;  // STUFF THE BITS
        maMatriceDeConnexions[outIndex][inIndex] = lecture;
      }
      myPbHub.digitalWrite(outIndex, LOW);
    }

    if(monInt32DeConnexions == 273) {
      monOsc.sendInt("/connection", 1);
          pixel = CRGB(0, 255, 0);
          FastLED.show();
    } else {
      monOsc.sendInt("/connection", 0);
      pixel = CRGB(255, 0, 0);
    }

  }



}
