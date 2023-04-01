#ifndef hardware_button_manager_h
#define hardware_button_manager_h

#include <Arduino.h>
#include <Bounce2.h>

class HardwareButtonManager {
private:
    Bounce2::Button* createButton(gpio_num_t pin);
public:
    Bounce2::Button* posButtons[4];
    HardwareButtonManager(gpio_num_t pos1, gpio_num_t pos2, gpio_num_t pos3, gpio_num_t pos4);
    ~HardwareButtonManager();
    void tick();
};

#endif