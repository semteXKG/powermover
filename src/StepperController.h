#ifndef stepper_controller
#define stepper_controller

#include <Arduino.h>
#include <FastAccelStepper.h>
#include <Constants.h>

class StepperController {
private:
    FastAccelStepper* stepper;
    int currentPos;
public:
    StepperController(FastAccelStepperEngine& engine);
    void goToPosition(int position);
    int getCurrentPosition();
    void tick();
    ~StepperController();
};

#endif