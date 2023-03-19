#include <StepperController.h>

StepperController::StepperController(SharedData* sharedData, FastAccelStepperEngine* engine, gpio_num_t pulsePin, gpio_num_t directionPin) {
    this->sharedData = sharedData;
    stepper = engine->stepperConnectToPin(pulsePin);
    stepper->setDirectionPin(directionPin);
    stepper->setSpeedInHz(OP_SPEED);
    stepper->setAcceleration(10 * OP_SPEED);
    stepper->setCurrentPosition(0);  
}

void StepperController::goToPosition(int position) {
    int posOffset = sharedData->getPosition() - position;
    if(posOffset == 0) {
        return;
    }
    int steps = posOffset * STEPS_PER_MOVE;
    stepper->move(steps);
}

void StepperController::tick() {
   
}

StepperController::~StepperController() {
}
