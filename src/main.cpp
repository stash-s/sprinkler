#include <Arduino.h>
#include <Homie.h>

#include "ValveNode.h"

ValveNode valveNode("valve", "Valve");
//HomieNode flowNode("flow", "Water Flow", "flow");

void setup()
{
    Serial.begin(9600);

    Homie_setBrand("sprinkler");
    Homie_setFirmware("sprinkler", "0.0.1");

    Homie.setup();
}

void loop()
{
    Homie.loop();
}