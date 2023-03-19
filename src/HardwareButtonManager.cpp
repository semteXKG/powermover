#include <HardwareButtonManager.h>



HardwareButtonManager::HardwareButtonManager(gpio_num_t pos1, gpio_num_t pos2, gpio_num_t pos3, gpio_num_t pos4, SharedData* sharedData, StepperController* stepperController, ShellyManager* shellyManager) {
    this->sharedData = sharedData;
    this->stepperController = stepperController;
    this->shellyManager = shellyManager;
    sharedData->posButtons[0] = createButton(pos1);
    sharedData->posButtons[1] = createButton(pos2);
    sharedData->posButtons[2] = createButton(pos3);
    sharedData->posButtons[3] = createButton(pos4);
}

HardwareButtonManager::~HardwareButtonManager() {
}

void HardwareButtonManager::tick() {
    for(int i = 0; i < 4; i++) {
        sharedData->posButtons[i]->update();
        if (sharedData->posButtons[i]->pressed()) {
            handleButtonPressedForPosition(i);
        }
    }
    
}

void HardwareButtonManager::handleButtonPressedForPosition(int position) {
    shellyManager->toggleShelly();
    stepperController->goToPosition(position);
}

Bounce2::Button* createButton(gpio_num_t pin) {
    Bounce2::Button* button = new Bounce2::Button();
    button->setPressedState(HIGH);
    button->interval(5);
    button->attach(pin, INPUT_PULLUP);
    return button;
}