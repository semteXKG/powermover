#ifndef SHELLY_MANAGER
#define SHELLY_MANAGER
#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>

class ShellyManager {
private:
    long lastReconnectRetry = 0;
    WiFiClient* net;
    PubSubClient* mqtt;
    String shellyId;
    String commandTopic;
    bool isOn = false;
    void connect();
    void callback(char* topic, byte* payload, unsigned int length);
    void handleAnnounce(StaticJsonDocument<1000>& doc);
    void handleStatus(StaticJsonDocument<1000>& doc);
public:
    void update();
    void toggle();
    void turnOn();
    void turnOff();
    ShellyManager(/* args */);
    ~ShellyManager();
};

#endif