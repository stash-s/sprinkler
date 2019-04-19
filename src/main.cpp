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

bool valveOnOffHandler(const HomieRange &range, const String &value)
{

    Homie.getLogger() << "valve " << range.index << " set to " << value << endl;

    if (!ValveNode::isValidRange(range))
        return false;

    if (value != "on" && value != "off")
        return false;

    const bool on = (value == "on");

    digitalWrite(valvePins[range.index - 1], on ? HIGH : LOW);

    valveNode.setProperty("state").setRange(range).send(value);

    return true;
}

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