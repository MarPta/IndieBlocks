#ifndef _ESP32_TOUCH_
#define _ESP32_TOUCH_

#define TOUCH_THRESH_NO_USE   (0)
#define TOUCH_THRESH_PERCENT  (80)
#define TOUCHPAD_FILTER_TOUCH_PERIOD (10)

//static const uint8_t touchPins[] = {4, 15, 13, 12, 14, 27, 32};  //gpio 32 is labeled 23

class ESP32_Touch {
    static const uint8_t touchPads[];
    float staticValues[7];
    uint16_t touchThreshold = 7;
public:
    void init();
    uint16_t read(uint8_t pin);
    void calibrate();
    int32_t getCount();
};

#endif //_ESP32_TOUCH_