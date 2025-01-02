#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include "FreeRTOS.h"
#include "task.h"

// Handler in case our application overflows the stack
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
    (void) xTask;
    (void) pcTaskName;

    for (;;);
}

// Task that toggles PC13, which is the LED
static void taskLed(void *args) {
    (void) args;

    for (;;) {
        gpio_toggle(GPIOC, GPIO13);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

int main(void) {
    //setup main clock, using external 8MHz crystal
    rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ]);

    //enable clock for GPIO port C
    rcc_periph_clock_enable(RCC_GPIOC);

    //set pinmode for PC13
    gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);

    //tell FreeRTOS about our toggle task
    xTaskCreate(taskLed, "LED", 100, NULL, 2, NULL);

    //start task scheduler
    vTaskStartScheduler();

    //task scheduler is blocking, we should never come here
    for (;;);
}
