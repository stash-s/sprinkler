#include <Arduino.h>
#include <Homie.h>

HomieNode valveBoxNode("valveBox", "Valve", "switch", true);

uint8_t valvePins[] = {4, 5, 10, 11};

bool valveHandler(const HomieRange &range, const String &value)
{
  if (value != "true" && value != "false")
    return false;

  const bool on = (value == "true");

  Homie.getLogger() << "valve " << range.index << " set to " << value << endl;

  digitalWrite(valvePins[range.index] - 1, on);

  valveBoxNode.setProperty("state").setRange(range).send(value);

  return true;
}

void setup()
{
  Homie_setFirmware("sprinkler", "0.0.1");
  valveBoxNode.advertise("valve").setName("On").setDatatype("boolean").settable(valveHandler);
  Homie.setup();
}

void loop()
{
  Homie.loop();
}