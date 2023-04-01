#include <Arduino.h>
#include <WlanHandler.h>
#include <FastAccelStepper.h>
#include <ShellyManager.h>
#include <HardwareButtonManager.h>
#include <Bounce2.h>
#include <StepperController.h>

FastAccelStepperEngine engine = FastAccelStepperEngine();
FastAccelStepper* stepper = NULL;

HardwareButtonManager* hardwareButtonManager;
WiFiManager* wifiManager;
ShellyManager* shellyManager;
StepperController* stepperController;

void setup() {
  Serial.begin(115200);
  wifiManager = new WiFiManager(Serial);
  if(!wifiManager->autoConnect("Ad-Hoc")) {
    wifiManager->startConfigPortal();
  }

  Serial.println("Connected to " + wifiManager->getWiFiSSID());

  shellyManager = new ShellyManager();
  hardwareButtonManager = new HardwareButtonManager(GPIO_NUM_16, GPIO_NUM_17, GPIO_NUM_18, GPIO_NUM_19);
  stepperController = new StepperController(engine);
}

void handleButtonRose(int i) {
  if(stepperController->getCurrentPosition() == i) {
    shellyManager->toggle();
  } else {
    stepperController->goToPosition(i);
    shellyManager->turnOn();
  }
}


void loop() {
  shellyManager->update();
  hardwareButtonManager->tick();
  for (int i = 0; i < 4; i++) {
    if (hardwareButtonManager->posButtons[i]->rose()) {
      handleButtonRose(i);
    }
  }
}


