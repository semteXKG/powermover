#include <ShellyManager.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include <ArduinoJson.h>

ShellyManager::ShellyManager() {
    this->net = new WiFiClient();  
    this->mqtt = new PubSubClient(IPAddress(10, 0, 0, 8), 1883, *this->net);  
}

ShellyManager::~ShellyManager() {

}

void ShellyManager::callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    String topStr = String(topic);
    StaticJsonDocument<1000> doc;
    deserializeJson(doc, payload, length);
    serializeJsonPretty(doc, Serial);
    if (topStr.equals("shellies/announce")) {
        handleAnnounce(doc);
    } else if (topStr.equals(this->shellyId + "/status/switch:0")) {
        handleStatus(doc);
    }
}

void ShellyManager::handleAnnounce(StaticJsonDocument<1000>& doc) {
    shellyId = doc["id"].as<String>();
    
    String sub = this->shellyId + "/status/switch:0";
    commandTopic = this->shellyId + "/command/switch:0";
    
    Serial.println(sub);
    Serial.println(commandTopic);

    mqtt->subscribe(sub.c_str());
    mqtt->publish(commandTopic.c_str(), "status_update");
}

void ShellyManager::handleStatus(StaticJsonDocument<1000>& doc) {
    this->isOn = doc["output"];
    Serial.println();
    Serial.print("Current status is ");
    Serial.println(this->isOn ? "ON": "OFF");
}

void ShellyManager::toggle() {
    String newTargetState = isOn ? "off" : "on";
    Serial.print("new target state: ");
    Serial.println(newTargetState);
    mqtt->publish(commandTopic.c_str(), newTargetState.c_str());
}


void ShellyManager::turnOn() {
    mqtt->publish(commandTopic.c_str(), "on");
}

void ShellyManager::turnOff() {
    mqtt->publish(commandTopic.c_str(), "off");
}

void ShellyManager::update() {
    if (WiFi.status() != WL_CONNECTED) {
        return;
    }    

    if (!mqtt->connected()) {
        connect();
        return;
    }
    mqtt->loop();
}

void ShellyManager::connect() {
    if (!mqtt->connected() && millis() - lastReconnectRetry > 5000) {
        if (mqtt->connect("ESP32-powermover", "esp32", "esp32")) {
            static auto callback_static = [this](char* topic, byte* payload, unsigned int length) {
                callback(topic, payload, length);
            };
                      
            mqtt->setCallback([](char* topic, byte* payload, unsigned int length) {
                callback_static(topic, payload, length);
            });
            
            mqtt->subscribe("shellies/announce");
            mqtt->publish("shellies/command", "announce");
        }
    }
}