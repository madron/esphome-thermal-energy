# esphome-thermal-energy
Thermal energy sensor for ESPHome

The sensor calculate thermal energy using a flow meter counter and input/output temperature.
The values are published every update_interval and as soon as the value changes.

```yaml
# Example configuration entry
external_components:
  - source: github://madron/esphome-thermal-energy
    components:
      - thermal_energy


sensor:
  - platform: thermal_energy
    id: dhw_total_energy
    counter: dhw_counter_id
    temp_in: dhw_in_id
    temp_out: dhw_out_id
    positive_energy:
      name: dhw_energy
```


## Configuration variables:
  - **counter** (**Required**): Sensor id of an ever increasing flow meter counter
  - **temp_in** (**Required**): Fluid input temperature (sensor id)
  - **temp_out** (**Required**): Fluid output temperature (sensor id)
  - **multiply** (Optional): The energy contained in one unit of fluid for a delta temperature of 1. When using standard units (volume in litres and temperature in kelvin or celsius) this value is a [calorie](https://en.wikipedia.org/wiki/Calorie) which can be converted in 1.16222 Wh. Defaults to 1.16222.
  - **positive_energy** (Optional): A sensor providing only positive energy.
  - **negative_energy** (Optional): A sensor providing only negative energy.
  - **update_interval** (Optional, Time): The interval to check the sensor. Defaults to 60s.
