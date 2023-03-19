#include <SharedData.h>
#include <Arduino.h>

SharedData::SharedData() {

}

SharedData::~SharedData() {

}

void SharedData::setPosition(int newPosition) {
    this->currentPos = newPosition;
}

int SharedData::getPosition() {
    return this->currentPos;
}
