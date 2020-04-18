#include <Arduino.h>
#include <Homie.h>

<<<<<<< HEAD
const unsigned int NUMBER_OF_VALVES = 5;
const uint8_t valvePins[NUMBER_OF_VALVES] = {13, 12, 14, 16, 15};
//const uint8_t flowSensorPin = 15;

HomieNode valveBoxNode("valveBox", "Valve", "switch", true, 1, NUMBER_OF_VALVES);
//HomieNode flowNode("flow", "Water Flow", "flow");

bool valveHandler(const HomieRange &range, const String &value)
{
    if (value != "true" && value != "false")
        return false;

    if (!range.isRange)
        return false;

    if (range.index < 1 || range.index > NUMBER_OF_VALVES)
        return false;

    if (value != "on" && value != "off")
        return false;

    const bool on = (value == "on");

    Homie.getLogger() << "valve " << range.index << " set to " << value << endl;
    
    digitalWrite(valvePins[range.index-1], on ? HIGH : LOW);

    valveBoxNode.setProperty("state").setRange(range).send(value);

    return true;
}

void setup()
{
=======
#include "ValveNode.h"
#include "WaterFlowNode.h"

ValveNode valveNode("valve", "Valve");
WaterFlowNode waterFlowNode("water", "Water Flow");

void setup() {
>>>>>>> 31b29a993a7a723a9a8249437b511ee533a04f88
    Serial.begin(9600);

    Homie_setBrand("sprinkler");
<<<<<<< HEAD
    Homie_setFirmware("sprinkler", "0.0.2");
    valveBoxNode.advertise("valve").setName("Valve").setDatatype("boolean").settable(valveHandler);
    //flowNode.advertise("flow").setDatatype("float");
=======
    Homie_setFirmware("sprinkler", "0.2.0");

>>>>>>> 31b29a993a7a723a9a8249437b511ee533a04f88
    Homie.setup();
}

void loop() { Homie.loop(); }