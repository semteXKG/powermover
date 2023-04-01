#include <StepperController.h>

StepperController::StepperController(FastAccelStepperEngine& engine) {
    stepper = engine.stepperConnectToPin(GPIO_NUM_27);
    stepper->setDirectionPin(GPIO_NUM_26);
    stepper->setSpeedInHz(OP_SPEED);
    stepper->setAcceleration(10 * OP_SPEED);
    stepper->setCurrentPosition(0);  
}

void StepperController::goToPosition(int position) {
    int posOffset = currentPos - position;
    if(posOffset == 0) {
        return;
    }
    int steps = posOffset * STEPS_PER_MOVE;
    stepper->move(steps);
    currentPos = position;
}

int StepperController::getCurrentPosition() {
    return currentPos;
}

StepperController::~StepperController() {
}
