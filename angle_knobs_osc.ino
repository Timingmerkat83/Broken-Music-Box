#include <M5Atom.h>      // Inclure la librairie M5 (version pour M5Atom)
#include <MicroOscSlip.h> // Inclure la librairie MicroOscSlip pour OSC
#include <M5_PbHub.h>     // Inclure la librairie pour PbHub

#define DATA_PIN 27       // Broche pour le contrôle des LED
#define CHANNEL_ANGLE_1 1 // PbHub Channel for the first angle unit
#define CHAN_KEY 5        // Canal pour le bouton Key
#define CHANNEL_ANGLE_2 2 // PbHub Channel for the second angle unit
#define CHANNEL_ANGLE_3 3 // PbHub Channel for the third angle unit

CRGB pixel;               // Définir un pixel CRGB
unsigned long monChronoMessage;
bool isGreen1 = false;    // État du premier capteur (vert)
bool isGreen2 = false;    // État du deuxième capteur (vert)
bool isGreen3 = false;    // État du troisième capteur (vert)

M5_PbHub myPbHub;         // Initialiser PbHub
MicroOscSlip<128> monOsc(&Serial);  // Initialiser OSC via Serial

void setup() {
  M5.begin(false, false, false);  // Démarrer M5Atom avec toutes les options désactivées
  Serial.begin(115200);  // Initialiser la communication série

  Wire.begin();
  myPbHub.begin();

  // Animation de démarrage
  unsigned long chronoDepart = millis();
  while (millis() - chronoDepart <= 5000) {
    // Animation (blanc -> rouge -> blanc)
    myPbHub.setPixelColor(CHAN_KEY, 0, 255, 0, 0);  // Rouge pendant l'animation
    delay(100);
    myPbHub.setPixelColor(CHAN_KEY, 0, 255, 255, 255);  // Blanc pendant l'animation
    delay(100);
  }
}

void loop() {
  M5.update();  // Toujours inclure M5.update() au début de loop()

  int maLectureKey = myPbHub.digitalRead(CHAN_KEY);  // Lire l'état du bouton Key
  
  // Vérification des angles
  if (millis() - monChronoMessage >= 50) {
    monChronoMessage = millis();

    // Lire la valeur de l'angle du capteur 1
    int maLectureAngle1 = myPbHub.analogRead(CHANNEL_ANGLE_1);

    monOsc.sendInt("/pot", maLectureAngle1);


    // Vérifier si le capteur 1 est dans la plage (vert)
    if (maLectureAngle1 >= 0 && maLectureAngle1 <= 2100) {
      isGreen1 = true;  // Le capteur 1 est dans la plage
    } else {
      isGreen1 = false;
    }

    // Lire la valeur de l'angle du capteur 2
    int maLectureAngle2 = myPbHub.analogRead(CHANNEL_ANGLE_2);
 

    // Vérifier si le capteur 2 est dans la plage (vert)
    if (maLectureAngle2 >= 2100 && maLectureAngle2 <= 4095) {
      isGreen2 = true;  // Le capteur 2 est dans la plage
    } else {
      isGreen2 = false;
    }

    // Lire la valeur de l'angle du capteur 3
    int maLectureAngle3 = myPbHub.analogRead(CHANNEL_ANGLE_3);
 
    monOsc.sendInt("/pot", maLectureAngle3);

    // Vérifier si le capteur 3 est dans la plage (vert)
    if (maLectureAngle3 >= 1500 && maLectureAngle3 <= 3100) {
      isGreen3 = true;  // Le capteur 3 est dans la plage
    } else {
      isGreen3 = false;
    }

    // Mise à jour de la couleur du bouton Key
    if (isGreen1 && isGreen2 && isGreen3) {
      // Si tous les capteurs sont dans les plages spécifiées, la couleur devient verte
      myPbHub.setPixelColor(CHAN_KEY, 0, 0, 255, 0);  // Vert (0, 255, 0)
    } else {
      // Sinon, la couleur reste rouge
      myPbHub.setPixelColor(CHAN_KEY, 0, 255, 0, 0);  // Rouge (255, 0, 0)
    }
  }

  // Vérifier si le bouton est pressé
  if (myPbHub.digitalRead(CHAN_KEY) == LOW) {
    // Vérification de l'état des capteurs
    if (isGreen1 && isGreen2 && isGreen3) {
      // Envoyer un message OSC pour indiquer un succès
      monOsc.sendInt("/success", 1); // Message OSC : "/success" avec la valeur 1
      // Mise à jour du bouton Key avec un message de succès (Vert)
      myPbHub.setPixelColor(CHAN_KEY, 0, 0, 255, 0);  // Vert pour succès
    } else {
      // Envoyer un message OSC pour indiquer un échec
      monOsc.sendInt("/success", 0); // Message OSC : "/failure" avec la valeur 0
      // Mise à jour du bouton Key avec un message d'échec (Rouge)
      myPbHub.setPixelColor(CHAN_KEY, 0, 255, 0, 0);  // Rouge pour échec
    }
  }

  // Gérer les messages OSC entrants (si applicable)
  monOsc.onOscMessageReceived([](MicroOscMessage& oscMessage) {
    if (oscMessage.checkOscAddress("/test")) {
      int value = oscMessage.nextAsInt();
    }
  });
}

