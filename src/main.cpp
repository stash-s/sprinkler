#include <Arduino.h>
#include <Homie.h>

#include "ValveNode.h"
#include "WaterFlowNode.h"

ValveNode valveNode("valve", "Valve");
WaterFlowNode waterFlowNode("water", "Water Flow");

void setup()
{
    Serial.begin(9600);

    Homie_setBrand("sprinkler");
    Homie_setFirmware("sprinkler", "0.2.0");

    Homie.setup();
}

void loop()
{
    Homie.loop();
}