#ifndef __HA_ENTITY_WEIGHT_H__
#define __HA_ENTITY_WEIGHT_H__

#include "HaDeviceClasses.h"
#include "HaEntitySensor.h"
#include <HaBridge.h>
#include <HaEntity.h>
#include <cstdint>
#include <optional>
#include <string>

/**
 * @brief Represent a Weight sensor (see homeassistantentities::Sensor::Weight:Unit in HaDeviceClasses.h).
 */
class HaEntityWeight : public HaEntity {
public:
  using Unit = homeassistantentities::Sensor::Weight::Unit;

  struct Configuration {
    /**
     * @brief the unit of measurement reported for this sensor. Make sure that the value you publish is of this unit.
     */
    Unit unit = Unit::kg;

    /**
     * In Home Assistant, trigger events even if the sensor's state hasn't changed. Useful if you want
     * to have meaningful value graphs in history or want to create an automation that triggers on every incoming state
     * message (not only when the sensor’s new state is different to the current one).
     */
    bool force_update = false;
  };

  inline static Configuration _default = {.unit = Unit::kg, .force_update = false};

  /**
   * @brief Construct a new Ha Entity Weight object
   *
   * @param name this is the human readable name that will be used for the entity in Home Assistant. If a device is set
   * when creating the [HaBridge], the friendly named displayed in the UI will be the device name plus this name.
   * Example: if device name is "Bathroom" and entity name "weight", friendly name will be "Bathroom weight". If no
   * device, friendly name will be just "weight". If a device is set, this name can be left empty if this entity is the
   * one main entity (or only) entity of this device. See
   * https://developers.home-assistant.io/docs/core/entity/#entity-naming for more
   * information.
   * @param child_object_id optional child identifier for this entity in case there are several sensors of the same
   * entity type for the same node ID. Example: If you have a lock for the node ID "door", the home asisstant
   * configuration path will be "homeassistant/binary_sensor/door/lock/config". This works if you only have one lock on
   * your door, but if you have two locks, you want to add a child object ID to them. By setting the child_object_id to
   * say "upper", the configuration will be "homeassistant/binary_sensor/door/lock/upper/config". This also apply for
   * all state/command topics and so on. Leave as empty string for no child object ID. Valid characters
   * are [a-zA-Z0-9_-] (machine readable, not human readable)
   * @param configuration the configuration for this entity.
   */
  HaEntityWeight(HaBridge &ha_bridge, std::string name, std::optional<std::string> child_object_id = std::nullopt,
                 Configuration configuration = _default)
      : _ha_entity_sensor(HaEntitySensor(ha_bridge, name, child_object_id,
                                         HaEntitySensor::Configuration{
                                             .device_class = _weight,
                                             .unit_of_measurement = configuration.unit,
                                             .force_update = configuration.force_update,
                                         })) {}

public:
  void publishConfiguration() override { _ha_entity_sensor.publishConfiguration(); }
  void republishState() override { _ha_entity_sensor.republishState(); }

  /**
   * @brief Publish the weight. This will publish to MQTT regardless if the value has changed. Also see
   * updateWeight().
   *
   * @param weight weight in the unit specified in the configuration.
   */
  void publishWeight(double weight) { _ha_entity_sensor.publishValue(weight); }

  /**
   * @brief Publish the weight, but only if the value has changed. Also see publishWeight().
   *
   * @param weight weight in the unit specified in the configuration.
   */
  void updateWeight(double weight) { _ha_entity_sensor.updateValue(weight); }

private:
  const homeassistantentities::Sensor::Weight _weight;
  HaEntitySensor _ha_entity_sensor;
};

#endif // __HA_ENTITY_WEIGHT_H__