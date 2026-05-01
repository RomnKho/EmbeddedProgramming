/*
 * @file    main.c
 * @author  Roman Khomenko
 * @date    01/05/2026
 * @brief   Testing how ADC works with a potentiometer
 */


#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/adc.h"         // Implementes the adc channels
#include "esp_adc_cal.h"        // Implements the calibration of the adc

void check_efuse(void)
{
    if(esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_TP) == ESP_OK)
    {
        printf("eFuse Two Point: Supported\n");
    }
    else
    {
        printf("eFuse Two Point: Unsupported\n");   // My esp32 doesn't support Two Point Calibration ...
    }

    if(esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_VREF) == ESP_OK)
    {
        printf("eFuse Vref: Supported\n");          // ... but supports Vref
    }
    else
    {
        printf("eFuse Vref: Unsupported\n");
    }
}

void app_main(void)
{
    check_efuse();
}