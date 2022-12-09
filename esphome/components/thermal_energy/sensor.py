import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import CONF_ID, CONF_MULTIPLY, CONF_VALUE
from esphome.const import DEVICE_CLASS_ENERGY, UNIT_WATT_HOURS
from esphome.const import STATE_CLASS_MEASUREMENT, STATE_CLASS_TOTAL_INCREASING

CONF_COUNTER = 'counter'
CONF_TEMP_IN = 'temp_in'
CONF_TEMP_OUT = 'temp_out'
CONF_POSITIVE_ENERGY = 'positive_energy'
CONF_NEGATIVE_ENERGY = 'negative_energy'


thermal_energy_ns = cg.esphome_ns.namespace("thermal_energy")
ThermalEnergy = thermal_energy_ns.class_("ThermalEnergy", sensor.Sensor, cg.Component)

ENERGY_SENSOR_SCHEMA = sensor.sensor_schema(
    unit_of_measurement=UNIT_WATT_HOURS,
    accuracy_decimals=0,
    device_class=DEVICE_CLASS_ENERGY,
    state_class=STATE_CLASS_MEASUREMENT,
)
INCREASING_ENERGY_SENSOR_SCHEMA = sensor.sensor_schema(
    unit_of_measurement=UNIT_WATT_HOURS,
    accuracy_decimals=0,
    device_class=DEVICE_CLASS_ENERGY,
    state_class=STATE_CLASS_TOTAL_INCREASING,
)


CONFIG_SCHEMA = ENERGY_SENSOR_SCHEMA.extend(
    {
        cv.GenerateID(): cv.declare_id(ThermalEnergy),
        cv.Required(CONF_COUNTER): cv.use_id(sensor.Sensor),
        cv.Required(CONF_TEMP_IN): cv.use_id(sensor.Sensor),
        cv.Required(CONF_TEMP_OUT): cv.use_id(sensor.Sensor),
        cv.Optional(CONF_MULTIPLY, default=1.16222): cv.float_,
        cv.Optional(CONF_POSITIVE_ENERGY): INCREASING_ENERGY_SENSOR_SCHEMA,
        cv.Optional(CONF_NEGATIVE_ENERGY): INCREASING_ENERGY_SENSOR_SCHEMA,
    }
).extend(cv.polling_component_schema('60s'))


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await sensor.register_sensor(var, config)
    counter = await cg.get_variable(config[CONF_COUNTER])
    temp1 = await cg.get_variable(config[CONF_TEMP_IN])
    temp2 = await cg.get_variable(config[CONF_TEMP_OUT])
    cg.add(var.set_config(counter, temp1, temp2, config[CONF_MULTIPLY]))
    if CONF_POSITIVE_ENERGY in config:
        conf = config[CONF_POSITIVE_ENERGY]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_positive_energy_sensor(sens))
    if CONF_NEGATIVE_ENERGY in config:
        conf = config[CONF_NEGATIVE_ENERGY]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_negative_energy_sensor(sens))
