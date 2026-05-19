#include "RotarySystem.h"

void RotarySystem::begin(uint8_t clkPin, uint8_t dtPin, uint8_t swPin) 
{
    _clkPin = clkPin;
    _dtPin  = dtPin;
    _swPin  = swPin;

    _queue = xQueueCreate(QUEUE_SIZE, sizeof(Event));
    xTaskCreatePinnedToCore(taskRotary, "rotary", 4096, this, 1, nullptr, CORE_ID);
}

bool RotarySystem::popEvent(Event& ev) 
{
    return xQueueReceive(_queue, &ev, 0) == pdTRUE;
}

void RotarySystem::taskRotary(void* pvParams) 
{
    RotarySystem* self = static_cast<RotarySystem*>(pvParams);

    pinMode(self->_clkPin, INPUT_PULLUP);
    pinMode(self->_dtPin,  INPUT_PULLUP);
    pinMode(self->_swPin,  INPUT_PULLUP);

    int      lastClk  = digitalRead(self->_clkPin);
    bool     lastBtn  = HIGH;
    uint32_t pressAt  = 0;

    QueueHandle_t queue = self->_queue;
    uint8_t clkPin = self->_clkPin;
    uint8_t dtPin  = self->_dtPin;
    uint8_t swPin  = self->_swPin;

    for (;;) 
    {
        int clk = digitalRead(clkPin);
        if (clk != lastClk && clk == LOW) 
        {
            Event e = digitalRead(dtPin) ? RIGHT : LEFT;
            xQueueSend(queue, &e, 0);
        }
        lastClk = clk;

        bool btn = digitalRead(swPin);
        if (btn == LOW && lastBtn == HIGH) 
        {
            pressAt = millis();
        }

        if (btn == HIGH && lastBtn == LOW) 
        {
            uint32_t elapsed = millis() - pressAt;
            Event e = (elapsed > LONG_PRESS_MS) ? PRESS : CLICK;
            xQueueSend(queue, &e, 0);
        }

        lastBtn = btn;

        vTaskDelay(pdMS_TO_TICKS(TASK_DELAY_MS));
    }
}