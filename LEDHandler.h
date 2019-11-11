#ifndef LEDANDLER_H
#define LEDANDLER_H

#include <Arduino.h>
#define REDUCED_MODES
//#include <WS2812FX.h>


#include "NeoPatterns.h"


class LEDHandler
{
private:
    NeoPatterns<LEDHandler> neoPatterns;
   // WS2812FX ws2812fx;
    void OnRingComplete();
    friend class NeoPatterns<LEDHandler>;
public:
    LEDHandler(const uint8_t led_count, const uint8_t led_pin);
    ~LEDHandler();
    bool initialize();
    void loop();
};
#endif