#include "WaterFlowNode.h"

WaterFlowNode::WaterFlowNode(const char *id, const char *name,
                             uint8_t pinNumber)
    : HomieNode(id, name, "water-flow", false), sensorPin(pinNumber) {}

static unsigned int flow_count = 0;

ICACHE_FLASH_ATTR void flow_meter_isr() { ++flow_count; }

void WaterFlowNode::setup() {
    pinMode(getSensorPin(), INPUT);
    attachInterrupt(getSensorPin(), flow_meter_isr, RISING);

    advertise("flow")
        .setName("Incremental flow")
        .setDatatype("float")
        .setUnit("litres")
        .setRetained(false);
    advertise("beats")
        .setName("Incremental beats")
        .setDatatype("integer")
        .setUnit("beats")
        .setRetained(false);
    advertise("cumulative-beats")
        .setName("Cumulative beats")
        .setDatatype("integer")
        .setUnit("beats")
        .setRetained(true);
}

inline String to_string(float value) { return String(value, 5); }

void WaterFlowNode::loop() {
    unsigned long time_now = millis();
    unsigned long time_elapsed = time_now - last_update_time;

    if (time_elapsed >= FLOW_INTERVAL || last_update_time == 0) {
        unsigned long incremental_beats = flow_count - last_state_seen;

        
        if (incremental_beats) {
            float incremental_flow = (float)incremental_beats / beats_per_litre;

            last_state_seen = flow_count;
            last_update_time = time_now;

            setProperty("flow").send(to_string(incremental_flow));
            setProperty("beats").send(String(flow_count));
            setProperty("cumulative-beats").send(String(flow_count));
        }
    }
}