#include <RotaryEncoder.h>
#include <wiringPi.h>
#include <chrono>

RotaryEncoder::RotaryEncoder(uint8_t pinA, uint8_t pinB, uint8_t pinButton)
    : _pinA(pinA), _pinB(pinB), _pinButton(pinButton), _lastState(0), _rotation(0),
      _buttonPressed(false), _lastButtonPressTime(0)
{
    wiringPiSetupGpio(); // Use BCM numbering

    pinMode(_pinA, INPUT);
    pinMode(_pinB, INPUT);
    pinMode(_pinButton, INPUT);

    pullUpDnControl(_pinA, PUD_UP);
    pullUpDnControl(_pinB, PUD_UP);
    pullUpDnControl(_pinButton, PUD_UP);

    _lastState = (digitalRead(_pinA) << 1) | digitalRead(_pinB);
}

void RotaryEncoder::update()
{
    // --- Rotary movement ---
    int a = digitalRead(_pinA);
    int b = digitalRead(_pinB);
    int state = (a << 1) | b;

    // Simple state change detection
    if (state != _lastState)
    {
        if ((_lastState == 0b00 && state == 0b01) ||
            (_lastState == 0b01 && state == 0b11) ||
            (_lastState == 0b11 && state == 0b10) ||
            (_lastState == 0b10 && state == 0b00))
        {
            _rotation = 1; // Clockwise
        }
        else if ((_lastState == 0b00 && state == 0b10) ||
                 (_lastState == 0b10 && state == 0b11) ||
                 (_lastState == 0b11 && state == 0b01) ||
                 (_lastState == 0b01 && state == 0b00))
        {
            _rotation = -1; // Counter-clockwise
        }
        _lastState = state;
    }

    // --- Button press (with debounce) ---
    if (digitalRead(_pinButton) == LOW)
    {
        auto now = millis();
        if (now - _lastButtonPressTime > 200)
        {
            _buttonPressed = true;
            _lastButtonPressTime = now;
        }
    }
}

int RotaryEncoder::getRotation()
{
    int result = _rotation;
    _rotation = 0;
    return result;
}

bool RotaryEncoder::isButtonPressed()
{
    bool pressed = _buttonPressed;
    _buttonPressed = false;
    return pressed;
}
