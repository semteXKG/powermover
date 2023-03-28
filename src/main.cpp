#include <Arduino.h>
#include <WlanHandler.h>
#include <FastAccelStepper.h>
#include <ShellyManager.h>

#define enablePinStepper 25
#define dirPinStepper 26
#define stepPinStepper 27

FastAccelStepperEngine engine = FastAccelStepperEngine();
FastAccelStepper* stepper = NULL;

WiFiManager* wifiManager;
ShellyManager* shellyManager;

void setup() {
  Serial.begin(115200);
  wifiManager = new WiFiManager(Serial);
  if(!wifiManager->autoConnect("Ad-Hoc")) {
    wifiManager->startConfigPortal();
  }

  Serial.println("Connected to " + wifiManager->getWiFiSSID());

  shellyManager = new ShellyManager();
  /*
  engine.init();
  stepper = engine.stepperConnectToPin(stepPinStepper);
  if (stepper) {
    stepper->setDirectionPin(dirPinStepper);
    stepper->setEnablePin(enablePinStepper);
    stepper->setAutoEnable(true);

    stepper->setDelayToEnable(1000);
    stepper->setDelayToDisable(1000);

    stepper->setSpeedInHz(3200*5);  
    stepper->setAcceleration(10000);
    stepper->move(6000000);
  }*/
}

void loop() {
  shellyManager->update();
  delay(100);
}
