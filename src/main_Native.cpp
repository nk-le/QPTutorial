#include <Arduino.h>
#include <MathCore/Types/BasicTypes.h>
#include <MISC_HW/LEDCtrl.hpp>

Position _pos;
Position_ECEF _posECEF;
Velocity _vel;
Velocity_B _velB;

void setup(){
    blink(200,30);
    Serial.begin(115200);
    Serial.println("Hello World");
}

void loop(){

    
}
