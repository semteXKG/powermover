#ifndef hardware_button_manager_h
#define hardware_button_manager_h

#include <Arduino.h>
#include <SharedData.h>
#include <Bounce2.h>
#include <StepperController.h>
#include <ShellyManager.h>

class HardwareButtonManager {
private:
    SharedData* sharedData;
    StepperController* stepperController;
    ShellyManager* shellyManager;
    Bounce2::Button* createButton(gpio_num_t pin);
    void handleButtonPressedForPosition(int positionNumber);
public:
    HardwareButtonManager(gpio_num_t pos1, gpio_num_t pos2, gpio_num_t pos3, gpio_num_t pos4, SharedData* sharedData, StepperController* stepperController, ShellyManager* shellyManager);
    ~HardwareButtonManager();
    void tick();
};

#endif