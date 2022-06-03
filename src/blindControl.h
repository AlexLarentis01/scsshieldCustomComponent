#include "esphome.h"
#include "Interfaccia.h"
#include <Arduino.h>
class MyCustomComponent : public Component
{
private:
    Interfaccia interfaccia = new Interfaccia(D7, D6);

public:
    void setup() override
    {
        Serial.println("Setup");
        this->interfaccia.begin();
    }
    void loop() override
    {
        // This is called every loop
    }
};