#include <IActuator.h>
#include <vector>

class Ws2812Actuator : public IActuator
{
public:
    Ws2812Actuator(int ledCount, int gpioPin = 18);
    ~Ws2812Actuator();

    int init() override;
    void onNoteOn(uint8_t note, uint8_t velocity) override;
    void onNoteOff(uint8_t note) override;

private:
    int ledCount;
    int gpioPin;
    ws2811_t ledStrip;
    std::vector<uint32_t> ledColors;

    uint32_t colorFromNoteVelocity(uint8_t note, uint8_t velocity);
};
