/*
 * @file    potentiometer.c
 * @author  Roman Khomenko
 * @date    01/05/2026
 * @brief   Implementation file for potentiometers
 */

#include "driver/gpio.h" 
#include "driver/adc.h"         // Implementes the adc channels
#include "esp_adc_cal.h"        // Implements the calibration of the adc
#include "potentiometer.h"

#define DEFAULT_VREF 1100

static const adc_bits_width_t   bit_width   = ADC_WIDTH_BIT_12; // if adc2, the bid width has to be specified 
                                                                // each time adc2_get_raw() si called
static const adc_atten_t        attenuator  = ADC_ATTEN_DB_12;  // For potentiometers => atten. 150mV - 2450 mV
static const adc_unit_t         unit        = ADC_UNIT_1;

static adc1_channel_t                   channel;
static esp_adc_cal_characteristics_t    *adc_chars;             // Calibration characteristics

void adc_check_efuse(void)
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

void adc_print_char_val_type(esp_adc_cal_value_t val_type)
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

bool adc_init(gpio_num_t num_gpio)
{
    static bool b_correct_gpio = true;

    switch(num_gpio)
    {
        case GPIO_NUM_32:
            printf("Using ADC1_CHANNEL_4\n");
            channel = ADC1_CHANNEL_4;
            break;

        case GPIO_NUM_33:
            printf("Using ADC1_CHANNEL_5\n");
            channel = ADC1_CHANNEL_5;
            break;

        case GPIO_NUM_34:
            printf("Using ADC1_CHANNEL_6\n");
            channel = ADC1_CHANNEL_6;
            break;

        case GPIO_NUM_35:
            printf("Using ADC1_CHANNEL_7\n");
            channel = ADC1_CHANNEL_7;
            break;

        case GPIO_NUM_36:
            printf("Using ADC1_CHANNEL_0\n");
            channel = ADC1_CHANNEL_0;
            break;

        case GPIO_NUM_37:
            printf("Using ADC1_CHANNEL_1\n");
            channel = ADC1_CHANNEL_1;
            break;

        case GPIO_NUM_38:
            printf("Using ADC1_CHANNEL_2\n");
            channel = ADC1_CHANNEL_2;
            break;

        case GPIO_NUM_39:
            printf("Using ADC1_CHANNEL_3\n");
            channel = ADC1_CHANNEL_3;
            break;

        default:
            printf("Please, introduce an adc1 gpio (32-39)\n");
            b_correct_gpio = false;
            break;
    }

    if (b_correct_gpio == true)
    {
        // Configure the ADC
        adc1_config_width(bit_width);
        adc1_config_channel_atten(channel, attenuator);
        return true;
    }

    return false;
}

void adc_characterize(void)
{
    // Characterize the ADC
    adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(unit, attenuator, bit_width, DEFAULT_VREF, adc_chars);
    print_char_val_type(val_type);
}

void adc_read_multisampling(uint16_t no_samples, uint32_t *adc_reading_output, uint32_t *mV_output)
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
