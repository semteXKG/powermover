#ifndef SHELLY_MANAGER
#define SHELLY_MANAGER
#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include <StepperController.h>

class ShellyManager {
private:
    long lastReconnectRetry = 0;
    WiFiClient* net;
    PubSubClient* mqtt;
    StepperController* stepperController;
    String shellyRelais = String("shellyplus1-7c87ce63c878");
    String shellyButtonPlaner = String("shellies/shellybutton1-planer");
    String shellyButtonSaw = String("shellies/shellybutton1-saw");
    String commandTopic;
    bool isOn = false;
    int buttonForPosPressed = -1;
    void connect();
    void callback(char* topic, byte* payload, unsigned int length);
    void handleAnnounce(StaticJsonDocument<1000>& doc);
    void handleStatus(StaticJsonDocument<1000>& doc);
    void handleEvent(StaticJsonDocument<1000>& doc, int position);
public:
    void update();
    void toggle();
    void turnOn();
    void turnOff();
    int wasButtonPressed();
    ShellyManager(StepperController* stepperController);
    ~ShellyManager();
};

#endif