#ifndef ROTARY_SYSTEM_H
#define ROTARY_SYSTEM_H

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

class RotarySystem {
public:
    enum Event { EV_CW, EV_CCW, EV_PRESS, EV_LONG };

    void begin(uint8_t clkPin, uint8_t dtPin, uint8_t swPin);
    bool popEvent(Event& ev);

private:
    uint8_t _clkPin;
    uint8_t _dtPin;
    uint8_t _swPin;
    QueueHandle_t _queue = nullptr;

    static constexpr uint32_t LONG_PRESS_MS = 700;
    static constexpr uint8_t  TASK_DELAY_MS = 2;
    static constexpr uint8_t  QUEUE_SIZE    = 16;
    static constexpr uint8_t  CORE_ID       = 1;

    static void taskRotary(void* pvParams);
};

#endif
