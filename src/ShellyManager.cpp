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
    StaticJsonDocument<1000> doc;
    deserializeJson(doc, payload, length);
    serializeJsonPretty(doc, Serial);
    if (strcmp(topic, "shellies/announce") == 0) {
        handleAnnounce(doc);
    }
}

void ShellyManager::handleAnnounce(StaticJsonDocument<1000>& doc) {
    this->shellyId = doc["id"].as<String>();
    Serial.println(this->shellyId);
    String sub = this->shellyId + "/status/switch:0";
    String command = this->shellyId + "/command";
    Serial.println(sub);
    Serial.println(command);
    mqtt->subscribe(sub.c_str());
    mqtt->publish(command.c_str(), "status_update");
}

void ShellyManager::toggleShelly() {
    
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
            //mqtt->subscribe("#");
            mqtt->subscribe("shellyplus1-7c87ce63c878/status/switch:0");
            mqtt->publish("shellies/command", "announce");
        }
    }
}