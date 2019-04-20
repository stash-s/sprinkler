#include "ValveNode.h"

ValveNode::ValveNode(const char *id, const char *name)
    : HomieNode(id, name, "valve", true, 1, NUMBER_OF_VALVES),
      timeout(0), timeOfLastUpdate(0)
{
}

void ValveNode::setup()
{
    for (auto i : valvePins)
    {
        pinMode(i, OUTPUT);
        digitalWrite(i, LOW);
    }

    advertise("state").setName("Valve").setDatatype("string").setFormat("on,off").setRetained(true);
    advertise("timeout").setName("Timeout").setDatatype("integer").settable([&](const HomieRange &range, const String &value) {
        return valveTimeOutHandler(range, value);
    });
}

void ValveNode::loop(void)
{
    auto timeNow = millis();
    auto timeElapsed = timeNow - timeOfLastUpdate;

    if ((timeElapsed >= LOOP_PERIOD_MILLIS) || (timeout && (timeNow >= timeout)))
    {
        timeOfLastUpdate = timeNow;

        if (timeout && (timeout <= timeNow))
        {
            timeout = 0;

            for (auto i : valvePins)
            {
                digitalWrite(i, LOW);
            }

            for (unsigned int i = 1; i <= NUMBER_OF_VALVES; ++i)
            {
                HomieRange range;
                range.index = i;
                range.isRange = true;

                setProperty("state").setRange(range).send("off");
                setProperty("timeout").setRange(range).send("0");
            }
        }
        else
        {
            for (unsigned int i = 1; i <= NUMBER_OF_VALVES; ++i)
            {
                HomieRange range;
                range.index = i;
                range.isRange = true;

                int state = digitalRead(valvePins[i - 1]);
                if (state)
                {
                    setProperty("state").setRange(range).send("on");
                    setProperty("timeout").setRange(range).send(String((timeout - timeNow) / 1000));
                }
                else
                {
                    setProperty("state").setRange(range).send("off");
                    setProperty("timeout").setRange(range).send("0");
                }
            }
        }
    }
}

bool ValveNode::valveTimeOutHandler(const HomieRange &range, const String &value)
{
    Homie.getLogger() << "setting valve " << range.index << " timeout to " << value << endl;

    if (!isValidRange(range))
        return false;

    const long timeoutSeconds = value.toInt();

    auto timeNow = millis();
    timeout = timeNow + timeoutSeconds * 1000UL;

    for (unsigned int i = 1; i <= NUMBER_OF_VALVES; ++i)
    {
        HomieRange targetRange;
        targetRange.index = i;
        targetRange.isRange = true;

        if (targetRange.index == range.index)
        {
            digitalWrite(valvePins[i - 1], HIGH);
            setProperty("state").setRange(targetRange).send("on");
            setProperty("timeout").setRange(targetRange).send(String((timeout - timeNow) / 1000));
        }
        else
        {
            digitalWrite(valvePins[i - 1], LOW);
            setProperty("state").setRange(targetRange).send("off");
            setProperty("timeout").setRange(targetRange).send(String("0"));
        }
    }

    return true;
}