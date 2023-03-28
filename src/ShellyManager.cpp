#include <ShellyManager.h>
#include <PubSubClient.h>
#include <WiFi.h>

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
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
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
            mqtt->publish("shellies/command", "announce");
        }
    }
}