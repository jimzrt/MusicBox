#include "ButtonHandler.h"

ButtonHandler::ButtonHandler(uint8_t btn1Pin, uint8_t btn2Pin, uint8_t btn3Pin) : btn1(btn1Pin), btn2(btn2Pin), btn3(btn3Pin), afterLongPress(false)
{
}

ButtonHandler::~ButtonHandler()
{
}

void ButtonHandler::initialize()
{
    btn1.begin();
    btn2.begin();
    btn3.begin();
}

void ButtonHandler::readButtons()
{
    btn1.read();
    btn2.read();
    btn3.read();
}

bool ButtonHandler::detectLongPress(Button &button)
{
    if (button.pressedFor(2000))
    {
        afterLongPress = true;
        return true;
    }
    return false;
}

bool ButtonHandler::detectShortPress(Button &button)
{
    if (button.wasReleased())
    {
        if (afterLongPress)
        {
            afterLongPress = false;
            return false;
        }
        return true;
    }
    return false;
}

bool ButtonHandler::pressedNext()
{
    return detectShortPress(btn2);
}

bool ButtonHandler::pressedNextHold()
{
    return detectLongPress(btn2);
}
bool ButtonHandler::pressedPrevious()
{
    return detectShortPress(btn3);
}
bool ButtonHandler::pressedPreviousHold()
{
    return detectLongPress(btn3);
}
bool ButtonHandler::pressedConfirm()
{
    return detectShortPress(btn1);
}
bool ButtonHandler::pressedConfirmHold()
{
    return detectLongPress(btn1);
}
