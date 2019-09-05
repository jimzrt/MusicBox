#ifndef BUTTONHANDLER_H
#define BUTTONHANDLER_H

#include <Arduino.h>
#include <JC_Button.h>

class ButtonHandler
{
private:
    Button btn1;
    Button btn2;
    Button btn3;
    bool afterLongPress;
    bool detectLongPress(Button &button);
    bool detectShortPress(Button &button);

public:
    ButtonHandler(uint8_t btn1Pin, uint8_t btn2Pin, uint8_t btn3Pin);
    ~ButtonHandler();
    void initialize();
    void readButtons();
    bool pressedNext();
    bool pressedPrevious();
    bool pressedConfirm();
    bool pressedNextHold();
    bool pressedPreviousHold();
    bool pressedConfirmHold();
};

#endif