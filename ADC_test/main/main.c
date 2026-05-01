/*
 * @file    main.c
 * @author  Roman Khomenko
 * @date    01/05/2026
 * @brief   Testing how ADC works with a potentiometer
 */


#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "potentiometer.h"

#define NO_SAMPLES      64


void app_main(void)
{
    check_efuse();
    adc_init();
    adc_characterize();

    for(;;)
    {     
        read_adc_multisampling(NO_SAMPLES);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}