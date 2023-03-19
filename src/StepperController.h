#ifndef stepper_controller
#define stepper_controller

#include <Arduino.h>
#include <SharedData.h>
#include <FastAccelStepper.h>
#include <Constants.h>

class StepperController {
private:
    FastAccelStepper* stepper;
    SharedData* sharedData;
public:
    StepperController(SharedData* sharedData, FastAccelStepperEngine* engine, gpio_num_t pulsePin, gpio_num_t directionPin);
    void goToPosition(int position);
    void tick();
    ~StepperController();
};

#endif