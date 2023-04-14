#include <StepperController.h>

StepperController::StepperController(FastAccelStepperEngine& engine) {
    engine.init();
    stepper = engine.stepperConnectToPin(GPIO_NUM_27);
    stepper->setDirectionPin(GPIO_NUM_26);
    stepper->setSpeedInHz(OP_SPEED);
    stepper->setAcceleration(10 * OP_SPEED);
    stepper->setCurrentPosition(0);  
    currentPos = 0;
}

void StepperController::goToPosition(int position) {
    int posOffset = currentPos - position;
    if(posOffset == 0) {
        return;
    }
    
    stepper->enableOutputs();
    delay(100);

    int steps = (TICKS_PER_ROT / (double)MM_PER_ROT) * (posOffset * DIST_BETWEEN_POS_MM);
    Serial.print("moving steps: ");
    Serial.println(steps);
    stepper->move(steps);
    currentPos = position;
}

void StepperController::setEnabled(boolean enabled) {
    if (enabled) {
        stepper->enableOutputs();
    } else {
        stepper->disableOutputs();
    }
}

int StepperController::getCurrentPosition() {
    return currentPos;
}

StepperController::~StepperController() {
}
