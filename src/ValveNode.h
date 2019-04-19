#pragma once

#include <Homie.hpp>

#define LOOP_PERIOD_MILLIS 1000

const unsigned int NUMBER_OF_VALVES = 4;
const uint8_t valvePins[NUMBER_OF_VALVES] = {4, 14, 12, 13};
const uint8_t flowSensorPin = 15;

class ValveNode : public HomieNode
{

public:
    ValveNode(const char *id, const char *name);

    void setup() override;
    void loop() override;

    bool valveTimeOutHandler(const HomieRange &range, const String &value);

    static bool isValidRange(const HomieRange &range);

private:
    unsigned long timeout;
    unsigned long timeOfLastUpdate;
};

/**
 * Validate sprinkler range
 * */
inline bool ValveNode::isValidRange(const HomieRange &range)
{
    if (!range.isRange)
        return false;

    if (range.index < 1 || range.index > NUMBER_OF_VALVES)
        return false;

    return true;
}