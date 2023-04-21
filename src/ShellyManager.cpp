#include <ShellyManager.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include <ArduinoJson.h>

ShellyManager::ShellyManager(StepperController* stepperController) {
    this->net = new WiFiClient();  
    this->mqtt = new PubSubClient(IPAddress(10, 0, 0, 8), 1883, *this->net);  
    this->stepperController = stepperController;
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
    Serial.println();
    if (topStr.equals(shellyRelais + "/status/switch:0")) {
        handleStatus(doc);
    } else if (topStr.equals(shellyButtonSaw + "/input_event/0")) {
        handleEvent(doc, 1);
    } else if (topStr.equals(shellyButtonPlaner + "/input_event/0")) {
        handleEvent(doc, 3);
    }
}

void ShellyManager::handleEvent(StaticJsonDocument<1000>& doc, int position) {
    if (doc.containsKey("event")) {
        String evnt = doc["event"];
        if(evnt.equals("S")) {
            buttonForPosPressed = position;
        }
    }
}

int ShellyManager::wasButtonPressed() {
    int tmp = buttonForPosPressed;
    buttonForPosPressed = -1;
    return tmp;
}

void ShellyManager::handleStatus(StaticJsonDocument<1000>& doc) {
    this->isOn = doc["output"];
    Serial.println();
    Serial.print("Current status is ");
    Serial.println(this->isOn ? "ON": "OFF");
    this->stepperController->setEnabled(isOn);
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
            mqtt->subscribe((shellyRelais + String("/status/switch:0")).c_str());
            mqtt->subscribe((shellyButtonPlaner + String("/input_event/0")).c_str());
            mqtt->subscribe((shellyButtonSaw + String("/input_event/0")).c_str());
            
            this->commandTopic = shellyRelais + "/command/switch:0";
            mqtt->publish(commandTopic.c_str(), "status_update");
            mqtt->publish("shellies/command", "announce");
        }
    }
}