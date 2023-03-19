#ifndef shareddata_h
#define shareddata_h

#include <Bounce2.h>

class SharedData {
    private:
       int currentPos; 
    public:
        SharedData();
        ~SharedData();
        Bounce2::Button* posButtons[4];
        void setPosition(int newPosition);
        int getPosition();
};

#endif