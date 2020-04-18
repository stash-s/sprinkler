#pragma once

#include <Homie.h>

const uint8_t FLOW_SENSOR_PIN = 4

;

class WaterFlowNode : public HomieNode {
   public:
    WaterFlowNode(const char *id, const char *name,
                  uint8_t sensorPin = FLOW_SENSOR_PIN);

    void setup() override;
    void loop() override;

    uint8_t getSensorPin();

   private:
    uint8_t sensorPin;

    const unsigned long FLOW_INTERVAL = 1000UL;
    const float beats_per_litre = 373.3962;

    unsigned long last_update_time = 0;
    unsigned long last_state_seen = 0;
};

inline uint8_t WaterFlowNode::getSensorPin() { return sensorPin; }