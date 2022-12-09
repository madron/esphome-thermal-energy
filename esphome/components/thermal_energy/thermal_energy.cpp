#include "thermal_energy.h"
#include "esphome/core/log.h"

namespace esphome {
namespace thermal_energy {

static const char *const TAG = "thermal_energy";

void ThermalEnergy::dump_config() {
  ESP_LOGCONFIG(TAG, "ThermalEnergy");
  ESP_LOGCONFIG(TAG, "  multiply: %f", this->multiply_);
}

float pulse_value;
float pulse_energy;
float delta_t;

void ThermalEnergy::setup() {
  Component::setup();
  this->publish_state(0);
  if (this->positive_energy_sensor_ != nullptr) this->positive_energy_sensor_->publish_state(0);
  if (this->negative_energy_sensor_ != nullptr) this->negative_energy_sensor_->publish_state(0);

  this->counter_value_ = 0;
  this->temp_in_->add_on_raw_state_callback([this](float state) {
      this->temp_in_value_ = state;
  });
  this->temp_out_->add_on_raw_state_callback([this](float state) {
      this->temp_out_value_ = state;
  });
  this->counter_->add_on_raw_state_callback([this](float value) {
    pulse_value = value - this->counter_value_;
    this->counter_value_ = value;
    delta_t = this->temp_out_value_ - this->temp_in_value_;
    pulse_energy = delta_t * pulse_value * this->multiply_;
    ESP_LOGV(TAG, "delta_t = temp_out - temp_in = %f - %f = %f", this->temp_out_value_, this->temp_in_value_, delta_t);
    ESP_LOGV(TAG, "pulse_energy = delta_t * pulse_value * multiply = %f * %f * %f = %f", delta_t, pulse_value, this->multiply_, pulse_energy);
    this->publish_state(this->get_raw_state() + pulse_energy);
    if (pulse_energy > 0 and this->positive_energy_sensor_ != nullptr) {
      this->positive_energy_sensor_->publish_state(this->positive_energy_sensor_->get_raw_state() + pulse_energy);
    }
    if (pulse_energy < 0 and this->negative_energy_sensor_ != nullptr) {
      this->negative_energy_sensor_->publish_state(this->negative_energy_sensor_->get_raw_state() - pulse_energy);
    }
  });
}

void ThermalEnergy::update() {
  this->publish_state(this->get_raw_state());
  if (this->positive_energy_sensor_ != nullptr) {
    this->positive_energy_sensor_->publish_state(this->positive_energy_sensor_->get_raw_state());
  }
  if (this->negative_energy_sensor_ != nullptr) {
    this->negative_energy_sensor_->publish_state(this->negative_energy_sensor_->get_raw_state());
  }
}

}  // namespace thermal_energy
}  // namespace esphome
