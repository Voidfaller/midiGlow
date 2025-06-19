#ifndef RE_H
#define RE_H

#include <cstdint>
class RotaryEncoder
{
private:
    uint8_t pinA, pinB, pinButton;
    int _lastState;
    int _rotation;
    bool _buttonPressed;
    unsigned long _lastDebounceTime;

public:
    RotaryEncoder(uint8_t pinA, uint8_t pinB, uint8_t pinButton);
    ~RotaryEncoder();
    void update();
    int getRotation();
    bool isButtonPressed();

};

#endif // RE_H