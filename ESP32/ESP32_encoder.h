#ifndef _ESP32_ENCODER_
#define _ESP32_ENCODER_

#include "driver/pcnt.h"

static const int16_t PCNT_H_LIM_VAL = 32767;
static const int16_t PCNT_L_LIM_VAL = -32768;
static const int16_t PCNT_THRESH1_VAL = 32766;
static const int16_t PCNT_THRESH0_VAL = -32767;
static const int16_t ESP_INTR_FLAG_DEFAULT = 0;

static const uint8_t pcntPins[16] = {4, 5, 13, 14, 15, 16, 17, 18, 19, 21, 25, 26, 27, 32, 33, 35};

struct CounterTimeData {
    volatile int64_t counterPrevTime;   //prev time of pulse interrupt call
    volatile int32_t counterTimeDiff;   //time difference of pulse interrupt calls
    uint8_t aCounterIndex;
    int16_t debounceUs;
};

class ESP32_encoder {
    static uint8_t nextCounterIndex;
    uint8_t counterIndex;   //0-7

    uint8_t gpioA;
    uint8_t gpioB;
    bool risingSensitive;
    bool fallingSensitive;
    bool reverse;
    bool enableInterrupts;
    int16_t incrPerRevolution;
    int16_t debounceUs;
    int32_t maxPeriodUs;    //encoder period limit separating zero result [us]
    int32_t minPeriodUs;    //encoder period limit separating zero results [us]

    int16_t PCNT_internal_count;   //variable to store PCNT coming data, PCNT is only int16_t
    int32_t PCNT_count; //counter variable for external usage
    float frequency;
    static uint64_t gpioInputPinSel;
    struct CounterTimeData counterTimeData;
    static void pcnt_init(pcnt_unit_t pcntUnit, uint8_t GPIO_A, uint8_t GPIO_B);
    static void IRAM_ATTR gpio_isr_handler(void* arg);
public:
    ESP32_encoder(uint8_t aGpioA, uint8_t aGpioB);
    void init();
    int32_t getCount(); //return number of MotorEncoder increments with resolution 2 increments per revolution
    int16_t getDiff();
    void clearCount(){
        //clear encoder counter value to set zero position
        PCNT_count = 0;
    }
    float getFrequency(); //return motor axis frequency in [Hz]
};

#endif //_ESP32_ENCODER_