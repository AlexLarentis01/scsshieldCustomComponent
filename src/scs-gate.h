#include "esphome.h"
#include "Interfaccia.h"
#include <Arduino.h>
#include <vector>
#include <map>

class SCSGate : public Component
{
public:
    Interfaccia scs_interface;
    std::vector<Component*> devices;

    MyCustomSensor() : PollingComponent(10) {}

    SCSCover* addCover(int scs_0, int scs_1, int sec_down, int sec_up) 
    {
        SCSCover* = new SCSCover(scs_interface);
        devices.push_back();
    }

    void setup() override
    {
        bmp.begin();
        scs_interface = Interfaccia(D7, D6);
        scs_interface.begin();
    }

    void update() override
    {
        scs_interface.Loop_Seriale();
    }
};

class SCSCover : public PollingComponent, public Cover
{

    Serranda *serranda;

public:
    SCSCover(Interfaccia *i) : PollingComponent(1000)
    {
        serranda = new Serranda(i);
        float current_poss = 1;
        float time_per_percent_down = 60;
        float time_per_percent_up = 60;
    }

    void setup(int scs_0, int scs_1, int sec_down, int sec_up)
    {
        serranda->address(scs_0, scs_1);
        serranda->set_Timer(sec_down, sec_up);
        serranda->Reset_Change_Stato();
    }

    void update() override
    {
    }

    CoverTraits get_traits() override
    {
        auto traits = CoverTraits();
        traits.set_is_assumed_state(false);
        traits.set_supports_position(true);
        traits.set_supports_tilt(true);
        return traits;
    }
    void control(const CoverCall &call) override
    {

        // This will be called every time the user requests a state change.
        if (call.get_position().has_value())
        {
            float pos = *call.get_position();

            ESP_LOGD("UFFA", "%i", pos * 100);

            if (pos > 0)
            {
                serranda->Alza(1);
                this->position = pos;
            }
            else
            {
                serranda->Abbassa(1);
                this->position = pos;
            }

            // Publish new state

            this->publish_state();
        }
        if (call.get_tilt().has_value())
        {
            float pos = *call.get_tilt();

            ESP_LOGD("UFFA", "%f", pos);

            this->publish_state();
        }
        if (call.get_stop())
        {
            serranda->Stop();
        }
    }
};
