#include "LEDHandler.h"

void RingComplete();

LEDHandler::LEDHandler(const uint8_t led_count, const uint8_t led_pin) : neoPatterns(led_count,led_pin,NEO_GRB + NEO_KHZ800, *this)
{
}

LEDHandler::~LEDHandler()
{
}

bool LEDHandler::initialize(){
   neoPatterns.begin();
   neoPatterns.Scanner(neoPatterns.Color(255,0,0), 50);
   //ws2812fx.init();
  //ws2812fx.setBrightness(50);
  //ws2812fx.setSpeed(10);
  //ws2812fx.setMode(FX_MODE_COLOR_SWEEP_RANDOM);
  //ws2812fx.start();
    return true;
}
void LEDHandler::loop(){
    neoPatterns.Update();
   //  ws2812fx.service();
}

void LEDHandler::OnRingComplete(){

//Serial.println("Ring COmplettteee!!!");
}
