/*
 * @file    potentiometer.h
 * @author  Roman Khomenko
 * @date    01/05/2026
 * @brief   Header file for potentiometers
 */

#ifndef POTENTIOMETER_H
#define POTENTIOMETER_H

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
 * @brief Function that initializes the potentioemeter
 */

void adc_init(void);

/*
 * @brief Function that characterizes the ADC channel
 */
void adc_characterize(void);

/*
 * @brief Function that read and makes the average of 'no_samples' reads
 */
void read_adc_multisampling(uint16_t no_samples);

#endif