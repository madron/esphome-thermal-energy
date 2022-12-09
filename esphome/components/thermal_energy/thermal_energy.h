#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace thermal_energy {

class ThermalEnergy : public sensor::Sensor, public PollingComponent {
  public:
    sensor::Sensor* counter_;
    sensor::Sensor* temp_in_;
    sensor::Sensor* temp_out_;
    sensor::Sensor* positive_energy_sensor_;
    sensor::Sensor* negative_energy_sensor_;
    float multiply_;
    float counter_value_;
    float temp_in_value_;
    float temp_out_value_;

    void set_config(sensor::Sensor *counter, sensor::Sensor *temp_in, sensor::Sensor *temp_out, float multiply) { counter_ = counter; temp_in_ = temp_in; temp_out_ = temp_out; multiply_ = multiply; }
    void dump_config() override;
    void setup() override;
    void update() override;
    void set_positive_energy_sensor(sensor::Sensor *positive_energy_sensor) { positive_energy_sensor_ = positive_energy_sensor; }
    void set_negative_energy_sensor(sensor::Sensor *negative_energy_sensor) { negative_energy_sensor_ = negative_energy_sensor; }
};

}  // namespace thermal_energy
}  // namespace esphome
