#include <Arduino.h>
#include <FastAccelStepper.h>

#define enablePinStepper 25
#define dirPinStepper 26
#define stepPinStepper 27

FastAccelStepperEngine engine = FastAccelStepperEngine();
FastAccelStepper *stepper = NULL;

void setup() {
  engine.init();
  stepper = engine.stepperConnectToPin(stepPinStepper);
  if (stepper) {
    stepper->setDirectionPin(dirPinStepper);
    stepper->setEnablePin(enablePinStepper);
    stepper->setAutoEnable(true);

    // If auto enable/disable need delays, just add (one or both):
    stepper->setDelayToEnable(1000);
    stepper->setDelayToDisable(1000);

    stepper->setSpeedInHz(3200*5);  // the parameter is us/step !!!
    stepper->setAcceleration(10000);
    stepper->move(6000000);
  }
}

void loop() {}
