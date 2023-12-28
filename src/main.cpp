#include <Arduino.h>
#include <FastAccelStepper.h>
#include <ShellyManager.h>
#include <HardwareButtonManager.h>
#include <Bounce2.h>
#include <StepperController.h>
#include <WiFiManager.h>

FastAccelStepperEngine engine = FastAccelStepperEngine();
FastAccelStepper* stepper = NULL;

HardwareButtonManager* hardwareButtonManager;
WiFiManager* wifiManager;
ShellyManager* shellyManager;
StepperController* stepperController;
int previousCheck = 0;


void setup() {
  Serial.begin(115200);
  wifiManager = new WiFiManager(Serial);
  wifiManager->setWiFiAutoReconnect(true);
  wifiManager->setConnectTimeout(180);
  Serial.println("Connecting...");
  if(!wifiManager->autoConnect("Suction-Ad-Hoc")) {
    Serial.println("Could not connect, starting portal");
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

  unsigned long currentMillis = millis();
  // if WiFi is down, try reconnecting
  if ((WiFi.status() != WL_CONNECTED) && (currentMillis - previousCheck >=5000)) {
    Serial.println("Reconnecting to WiFi...");
    WiFi.disconnect();
    WiFi.reconnect();
    previousCheck = currentMillis;
  }

  delay(10);
}


