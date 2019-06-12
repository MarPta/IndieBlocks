#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "driver/touch_pad.h"
#include "soc/rtc_periph.h"
#include "ESP32_touchBar.h"

const uint8_t ESP32_touchBar::touchPads[] = {0, 3, 4, 5, 6, 7, 8};

void ESP32_touchBar::init() {
    // Initialize touch pad peripheral, it will start a timer to run a filter
    ESP_LOGI("Touch pad", "Initializing touch pad");
    touch_pad_init();

    // Set reference voltage for charging/discharging
    // For most usage scenarios, we recommend using the following combination:
    // the high reference valtage will be 2.7V - 1V = 1.7V, The low reference voltage will be 0.5V.
    touch_pad_set_voltage(TOUCH_HVOLT_2V7, TOUCH_LVOLT_0V5, TOUCH_HVOLT_ATTEN_1V);
    // Init touch pad IO
    for (int i = 0; i < 7; ++i) {
        //init RTC IO and mode for touch pad.
        touch_pad_config((touch_pad_t)touchPads[i], TOUCH_THRESH_NO_USE);
    }
    // Initialize and start a software filter to detect slight change of capacitance.
    touch_pad_filter_start(TOUCHPAD_FILTER_TOUCH_PERIOD);

    // Set initial static cap. values
    uint16_t touch_value;
    for(uint8_t i = 0; i < 7; ++i) {
        touch_pad_read_filtered((touch_pad_t)touchPads[i], &touch_value);
        staticValues[i] = float(touch_value);
    }
}

uint16_t ESP32_touchBar::read(uint8_t pin) {
    uint16_t touch_value;
    touch_pad_read_filtered((touch_pad_t)touchPads[pin], &touch_value);
    if(touch_value < staticValues[pin])
        return uint16_t(staticValues[pin]) - touch_value;
    else
        return 0;
}

void ESP32_touchBar::calibrate() {
    uint16_t touch_value = 0;
    for(uint8_t i = 0; i < 7; ++i) {
        touch_pad_read_filtered((touch_pad_t)touchPads[i], &touch_value);
        staticValues[i] = float(staticValues[i])*0.99 + float(touch_value)*0.01;
    }
}

int32_t ESP32_touchBar::getCount() {
    static int8_t touchStartPad = -1;   // -1 no last touch detected, 0-6 last touch pad
    static int32_t touchStartCount = 0;
    uint16_t touch_value = 0;
    uint16_t maxValue = 0;
    uint8_t maxPad = 0;

    ESP32_touchBar::calibrate();

    // Read capacity values and store highest
    for(uint8_t i = 0; i < 7; ++i) {
        touch_value = ESP32_touchBar::read(i);
        if(touch_value > maxValue) {
            maxValue = touch_value;
            maxPad = i;
        }
    }

    // When any touch detected
    if(maxValue > touchThreshold) {
        // When it is the first touch of a move
        if(touchStartPad == -1) {
            touchStartPad = maxPad;
            touchStartCount = count;
        }
        else {
            count = touchStartCount + maxPad - touchStartPad;
        }
    }
    else
        touchStartPad = -1;

    return count;
}

int32_t ESP32_touchBar::getDiff() {
    int32_t prevCount = count;
    count = getCount();

    return count - prevCount;
}