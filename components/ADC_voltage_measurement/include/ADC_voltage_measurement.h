#ifndef ADC_VOLTAGE_MEASUREMENT_H
#define ADC_VOLTAGE_MEASUREMENT_H

#include <stdio.h>
#include <esp_adc/adc_oneshot.h>
#include <esp_adc/adc_continuous.h>
#include <esp_adc/adc_cali_scheme.h>
#include <esp_adc/adc_cali.h>
#include <driver/gpio.h>
#include "esp_log.h"

// There are 10 voltage dividers on the PCB, each measuring a cell voltage
#define PCB_VOLTAGE_DEVIDERS 10
// The number of samples to average for each measurement
#define ADC_AVERAGE_SAMPLES 5

// ADC resolution and reference voltage
#define ADC_RESOLUTION 4095.0f
#define ADC_VREF 3.3f

#define ADC1_CHANNELS_USED 2
#define ADC2_CHANNELS_USED 9

// The pins used for enabling the mosfets on the PCB
// They enabling the cells voltages measurement
#define ADC_1_5S_ENABLE_PIN 16
#define ADC_6_10S_ENABLE_PIN 17


#define READ_LENGHT 256


extern esp_err_t ADC_error;

esp_err_t adc1_init();
esp_err_t adc2_init();
esp_err_t adc1_continous_start();
esp_err_t adc1_get_measurement();
esp_err_t adc2_get_measurement();
esp_err_t enable_adc();


typedef struct battery{
    uint32_t adc_raw_data[PCB_VOLTAGE_DEVIDERS][ADC_AVERAGE_SAMPLES];
    uint32_t adc_raw_sum[PCB_VOLTAGE_DEVIDERS];
    double adc_raw_avg[PCB_VOLTAGE_DEVIDERS];
    float cells_voltages[PCB_VOLTAGE_DEVIDERS];
}battery_t;

void get_avg_raw_voltage();
double* calculate_cell_voltages();

#endif // ADC_VOLTAGE_MEASUREMENT_H