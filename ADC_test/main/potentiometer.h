/*
 * @file    potentiometer.h
 * @author  Roman Khomenko
 * @date    01/05/2026
 * @brief   Header file for potentiometers
 */

#ifndef POTENTIOMETER_H
#define POTENTIOMETER_H

#include "driver/gpio.h"
#include "esp_adc_cal.h"

/*
 * @brief Function that prints wheter the esp32 has eFuse Two Point and Vref burned in 
 */
void check_efuse(void);

/*
 * @brief Function that prints the Calibration system used for the potentiometer
 * @param[in] Calibration type 
 */
void print_char_val_type(esp_adc_cal_value_t val_type);

/*
 * @brief Function that initializes the potentiometer
 * @param[in] num_gpio => gpio_number to be used
 */

bool adc_init(gpio_num_t num_gpio);

/*
 * @brief Function that characterizes the ADC channel
 */
void adc_characterize(void);

/*
 * @brief Function that read and makes the average of 'no_samples' reads
 * @param[in] no_samples => number of samples wanted, *adc_reading_output and *mV pointers to potentiometer info
 */
void read_adc_multisampling(uint16_t no_samples, uint32_t *adc_reading_output, uint32_t *mV);

#endif