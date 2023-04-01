#include <HardwareButtonManager.h>


Bounce2::Button* HardwareButtonManager::createButton(gpio_num_t pin) {
    Bounce2::Button* button = new Bounce2::Button();
    button->setPressedState(HIGH);
    button->interval(5);
    button->attach(pin);
    return button;
}

HardwareButtonManager::HardwareButtonManager(gpio_num_t pos1, gpio_num_t pos2, gpio_num_t pos3, gpio_num_t pos4) {
    posButtons[0] = createButton(pos1);
    posButtons[1] = createButton(pos2);
    posButtons[2] = createButton(pos3);
    posButtons[3] = createButton(pos4);
}

HardwareButtonManager::~HardwareButtonManager() {
}

void HardwareButtonManager::tick() {
    for(int i = 0; i < 4; i++) {
        posButtons[i]->update();
    }    
}