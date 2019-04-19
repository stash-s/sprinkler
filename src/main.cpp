#include <Arduino.h>
#include <Homie.h>

#include "ValveNode.h"

ValveNode valveNode("valve", "Valve");
//HomieNode flowNode("flow", "Water Flow", "flow");

/**
 * Static variables
 * */
unsigned long timeout = 0;

/**
 * Use cases
 * 
 * 1. Switch on sprinkler and let it stay on for a time given in seconds.
 *    - all other sprinklers are switched off. Controller publishes new state of each valve: on/off state and timeout
 *    - controller periodically publishes on/off state and timeout of each valve
 * 
 * 2. Switch on sprinkler A while other sprinkler B is running
 *    - sprinkler B is closed and timeout is set to zero. Both values are published
 *    - Sprinkler A is open (on) and timeout is set
 * */

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