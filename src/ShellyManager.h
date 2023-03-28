#ifndef SHELLY_MANAGER
#define SHELLY_MANAGER
#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFiClient.h>

class ShellyManager {
private:
    long lastReconnectRetry = 0;
    WiFiClient* net;
    PubSubClient* mqtt;
    void connect();
    void callback(char* topic, byte* payload, unsigned int length);
public:
    void update();
    void toggleShelly();
    ShellyManager(/* args */);
    ~ShellyManager();
};

#endif