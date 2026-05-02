/*
 * @file    main.c
 * @author  Roman Khomenko
 * @date    01/05/2026
 * @brief   Testing how ADC works with a potentiometer
 */


#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "potentiometer.h"

#define NO_SAMPLES      64

static gpio_num_t pot_gpio = GPIO_NUM_36;
static uint32_t adc_reading;
static uint32_t mV;

void app_main(void)
{
    check_efuse();
    bool init_correct = adc_init(pot_gpio);

    if(init_correct == false)
    {
        vTaskDelete(NULL);
    }

    adc_characterize();

    for(;;)
    {     
        read_adc_multisampling(NO_SAMPLES, &adc_reading, &mV);
        printf("Raw: %lu\tVoltage: %lu mV\n", adc_reading, mV);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}