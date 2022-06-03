#include "esphome.h"
#include "Interfaccia.h"
#include <Arduino.h>
#include <stdlib.h>


#define TAG "blindControl"
using std::vector;

typedef enum actorType {
  ACTOR_TYPE_BLIND,
  ACTOR_TYPE_LIGHT,
  ACTOR_TYPE_SWITCH,
  ACTOR_TYPE_SENSOR,
  ACTOR_TYPE_UNKNOWN
} ActorType;


typedef struct
{
    int address0;
    int address1;
    ActorType type;
} ActorControl;


class MyCustomComponent : public Component, public CustomAPIDevice
{
private:
    Interfaccia *interfaccia = new Interfaccia(5, 6);
    vector<ActorControl> actors;

public:
    void setup() override
    {

        Serial.println("Setup");
        this->interfaccia->begin();

        register_service(&MyCustomComponent::on_hello_world, "hello_world");

        register_service(&MyCustomComponent::on_start_washer_cycle, "start_washer_cycle",
                         {"cycle_duration", "silent", "string_argument"});

        subscribe_homeassistant_state(&MyCustomComponent::on_state_changed, "sensor.temperature");
    }

    void on_hello_world()
    {
        ESP_LOGD("custom", "Hello World!");

        if (is_connected())
        {
            // Example check to see if a client is connected
        }
    }

    void on_start_washer_cycle(int cycle_duration, bool silent, std::string string_argument)
    {
        ESP_LOGD("custom", "Starting washer cycle!");
        digitalWrite(8, HIGH);
        // do something with arguments

        // Call a homeassistant service
        call_homeassistant_service("homeassistant.service");
    }
    void on_state_changed(std::string state)
    {
        ESP_LOGD(TAG, "Temperature has changed to %s", state.c_str());
    }
};