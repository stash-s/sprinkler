#pragma once

#include <Homie.hpp>

#define LOOP_PERIOD_MILLIS 1000

const unsigned int NUMBER_OF_VALVES = 4;
const uint8_t valvePins[NUMBER_OF_VALVES] = {D5, D6, D7, D8};
const uint8_t flowSensorPin = D2;

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