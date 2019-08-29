#include "ButtonHandler.h"

ButtonHandler::ButtonHandler(uint8_t btn1Pin, uint8_t btn2Pin, uint8_t btn3Pin) : btn1(btn1Pin), btn2(btn2Pin), btn3(btn3Pin)
{
}

ButtonHandler::~ButtonHandler()
{
}

void ButtonHandler::initialize(){
    btn1.begin();
    btn2.begin();
    btn3.begin();
}