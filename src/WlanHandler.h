#ifndef WLAN_HANDLER_H
#define WLAN_HANDLER_H

#include <WiFiManager.h>

class WlanHandler {
private:
    WiFiManager* wifiManager;     
public:
    void startCapturePortal();
    void connectToWifi();
    WlanHandler();
    ~WlanHandler();
};

WlanHandler::WlanHandler() {
    wifiManager = new WiFiManager(Serial);
}

WlanHandler::~WlanHandler()
{
}

void WlanHandler::startCapturePortal() {
    Serial.println("Starting ad-hoc AP");
    wifiManager->startConfigPortal("klagra-adhoc");
}

void WlanHandler::connectToWifi() {

}

#endif