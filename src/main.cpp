#include <Arduino.h>
#include <FastAccelStepper.h>
#include <ShellyManager.h>
#include <HardwareButtonManager.h>
#include <Bounce2.h>
#include <StepperController.h>
#include <WiFiManager.h>

#define RETRY_COUNTER 10;
#define DELAY_MS 10000

FastAccelStepperEngine engine = FastAccelStepperEngine();
FastAccelStepper* stepper = NULL;

HardwareButtonManager* hardwareButtonManager;
WiFiManager* wifiManager;
ShellyManager* shellyManager;
StepperController* stepperController;

void setup() {
  Serial.begin(115200);

  u_int retry = RETRY_COUNTER;

  wifiManager = new WiFiManager(Serial);
  while (retry > 0) {
    if(!wifiManager->autoConnect("Suction-Ad-Hoc")) {
      delay(DELAY_MS);
      retry--;
      Serial.println("Could not connect, back-off for 10s")
    }  
  }
  
  if(retry == 0) {
    wifiManager->startConfigPortal();
  }

  Serial.println("Connected to " + wifiManager->getWiFiSSID());

  hardwareButtonManager = new HardwareButtonManager(GPIO_NUM_16, GPIO_NUM_17, GPIO_NUM_18, GPIO_NUM_19);
  stepperController = new StepperController(engine);
  shellyManager = new ShellyManager(stepperController);
}

void handleButtonRose(int i) {
  if(stepperController->getCurrentPosition() == i) {
    shellyManager->toggle();
  } else {
    stepperController->goToPosition(i);
    shellyManager->turnOn();
  }
  Serial.print("Position of Stepper is: ");  
  Serial.println(i);
}

void loop() {
  shellyManager->update();
  hardwareButtonManager->tick();
  for (int i = 0; i < 4; i++) {
    if (hardwareButtonManager->posButtons[i]->rose()) {
      handleButtonRose(i);
    }
  }

  int shellyButton = shellyManager->wasButtonPressed();
  if (shellyButton != -1) {
    handleButtonRose(shellyButton);
  }
  delay(10);
}


