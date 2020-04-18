#include <Arduino.h>
#include <Homie.h>

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
    Serial.begin(9600);

    for (auto i : valvePins)
    {
        pinMode(i, OUTPUT);
        digitalWrite(i, LOW);
    }

    Homie_setBrand("sprinkler");
    Homie_setFirmware("sprinkler", "0.0.2");
    valveBoxNode.advertise("valve").setName("Valve").setDatatype("boolean").settable(valveHandler);
    //flowNode.advertise("flow").setDatatype("float");
    Homie.setup();
}

void loop()
{
    Homie.loop();
}