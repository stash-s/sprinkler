#include "ValveNode.h"

#define ON_VALUE "true"
#define OFF_VALUE "false"

#define STATE_ON LOW
#define STATE_OFF HIGH

ValveNode::ValveNode(const char *id, const char *name)
    : HomieNode(id, name, "switch", true, 1, NUMBER_OF_VALVES),
      timeout(0),
      timeOfLastUpdate(0) {
        for (auto i : valvePins) {
            pinMode(i, OUTPUT);
            digitalWrite(i, STATE_OFF);
        }
      }

void ValveNode::setup() {
    for (auto i : valvePins) {
        pinMode(i, OUTPUT);
        digitalWrite(i, STATE_OFF);
    }

    advertise("on")
        .setName("Valve")
        .setDatatype("boolean")
        .setFormat("true,false")
        .setRetained(true).settable(
            [&](const HomieRange &range, const String &value) {
                return valveToggleHandler(range, value);
            }
        );
    advertise("timeout").setName("Timeout").setDatatype("integer").settable(
        [&](const HomieRange &range, const String &value) {
            return valveTimeOutHandler(range, value);
        });
}

void ValveNode::loop(void) {
    auto timeNow = millis();
    auto timeElapsed = timeNow - timeOfLastUpdate;

    if ((timeElapsed >= LOOP_PERIOD_MILLIS) ||
        (timeout && (timeNow >= timeout))) {
        timeOfLastUpdate = timeNow;

        if (timeout && (timeout <= timeNow)) {
            timeout = 0;

            for (auto i : valvePins) {
                digitalWrite(i, STATE_OFF);
            }

            for (unsigned int i = 1; i <= NUMBER_OF_VALVES; ++i) {
                HomieRange range;
                range.index = i;
                range.isRange = true;

                setProperty("on").setRange(range).send(OFF_VALUE);
                setProperty("timeout").setRange(range).send("0");
            }
        } else {
            for (unsigned int i = 1; i <= NUMBER_OF_VALVES; ++i) {
                HomieRange range;
                range.index = i;
                range.isRange = true;

                int state = digitalRead(valvePins[i - 1]);
                if (state == STATE_ON) {
                    setProperty("on").setRange(range).send(ON_VALUE);
                    setProperty("timeout").setRange(range).send(
                        String((timeout - timeNow) / 1000));
                } else {
                    setProperty("on").setRange(range).send(OFF_VALUE);
                    setProperty("timeout").setRange(range).send("0");
                }
            }
        }
    }
}

bool ValveNode::valveToggleHandler(const HomieRange &range,
                                    const String &value) {

    Homie.getLogger() << "toggle valve " << range.index << " to " << value << endl;

    if (0 == value.compareTo ("true")) {
        valveTimeOutHandler (range, "3600");
    } else {
        valveTimeOutHandler (range, "0");
    }

    Serial.println ("");

    return true;
}

bool ValveNode::valveTimeOutHandler(const HomieRange &range,
                                    const String &value) {
    Homie.getLogger() << "setting valve " << range.index << " timeout to "
                      << value << endl;

    if (!isValidRange(range)) return false;

    const long timeoutSeconds = value.toInt();

    auto timeNow = millis();
    timeout = timeNow + timeoutSeconds * 1000UL;

    for (unsigned int i = 1; i <= NUMBER_OF_VALVES; ++i) {
        HomieRange targetRange;
        targetRange.index = i;
        targetRange.isRange = true;

        if (targetRange.index == range.index) {
            digitalWrite(valvePins[i - 1], STATE_ON);
            setProperty("on").setRange(targetRange).send(ON_VALUE);
            setProperty("timeout")
                .setRange(targetRange)
                .send(String((timeout - timeNow) / 1000));
        } else {
            digitalWrite(valvePins[i - 1], STATE_OFF);
            setProperty("on").setRange(targetRange).send(OFF_VALUE);
            setProperty("timeout").setRange(targetRange).send(String("0"));
        }
    }

    return true;
}
