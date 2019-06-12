#ifndef _ESP32_TOUCH_BAR_
#define _ESP32_TOUCH_BAR_

#define TOUCH_THRESH_NO_USE   (0)
#define TOUCH_THRESH_PERCENT  (80)
#define TOUCHPAD_FILTER_TOUCH_PERIOD (10)

class ESP32_touchBar {
public:
    static const uint8_t touchPads[];
    float staticValues[7];
    uint16_t touchThreshold = 7;
    int32_t count = 0;  // output encoder-equivalent variable

    void init();
    uint16_t read(uint8_t pin);
    void calibrate();
    int32_t getCount();
    int32_t getDiff();
};

#endif //_ESP32_TOUCH_BAR_