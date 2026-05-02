/*
 * @file    potentiometer.c
 * @author  Roman Khomenko
 * @date    01/05/2026
 * @brief   Implementation file for potentiometers
 */

#include "driver/adc.h"         // Implementes the adc channels
#include "esp_adc_cal.h"        // Implements the calibration of the adc
#include "potentiometer.h"

#define DEFAULT_VREF 1100

static const adc_bits_width_t   bit_width   = ADC_WIDTH_BIT_12; // if adc2, the bid width has to be specified 
                                                                // each time adc2_get_raw() si called
static const adc1_channel_t     channel     = ADC_CHANNEL_0;    // adc1_channel_0 => gpio 36
static const adc_atten_t        attenuator  = ADC_ATTEN_DB_12;  // For potentiometers => atten. 150mV - 2450 mV
static const adc_unit_t         unit        = ADC_UNIT_1;

static esp_adc_cal_characteristics_t *adc_chars;                // Calibration characteristics

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

void print_char_val_type(esp_adc_cal_value_t val_type)
{
    if(val_type == ESP_ADC_CAL_VAL_EFUSE_TP)
    {
        printf("Characterized using Two Point Calibration\n");
    }
    else if(val_type == ESP_ADC_CAL_VAL_EFUSE_VREF)
    {
        printf("Characterized using Vref Calibration\n");
    }
    else
    {
        printf("Characterized using Default Vref Calibration\n");
    }
}

void adc_init(void)
{
     // Configure the ADC
    adc1_config_width(bit_width);
    adc1_config_channel_atten(channel, attenuator);
}

void adc_characterize(void)
{
    // Characterize the ADC
    adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(unit, attenuator, bit_width, DEFAULT_VREF, adc_chars);
    print_char_val_type(val_type);
}

void read_adc_multisampling(uint16_t no_samples, uint32_t *adc_reading_output, uint32_t *mV_output)
{
    uint32_t adc_reading_input = 0;
    static uint32_t mV_input = 0;
        
    // Multisampling
    // One read = 20us, 64 reads = 1.28 ms
    for(int i = 0; i < no_samples; i++)
    {
        adc_reading_input += adc1_get_raw(channel);
    }

    adc_reading_input /= no_samples;
    mV_input = esp_adc_cal_raw_to_voltage(adc_reading_input, adc_chars);

    *adc_reading_output = adc_reading_input;
    *mV_output = mV_input;
}
