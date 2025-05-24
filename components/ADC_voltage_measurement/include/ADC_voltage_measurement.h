#ifndef ADC_VOLTAGE_MEASUREMENT_H
#define ADC_VOLTAGE_MEASUREMENT_H

#include <stdio.h>
#include <esp_adc/adc_oneshot.h>
#include <esp_adc/adc_continuous.h>
#include <esp_adc/adc_cali_scheme.h>
#include <esp_adc/adc_cali.h>
#include "esp_log.h"


#define ADC1_CHANNELS_USED 1
#define ADC2_CHANNELS_USED 9
#define ADC_AVERAGE_SAMPLES 5

#define READ_LENGHT 256

extern esp_err_t ADC_error;


adc_oneshot_unit_init_cfg_t* get_adc2_config_unit();
adc_oneshot_unit_handle_t* get_adc2_unit_handle();
esp_err_t adc2_channels_configure(uint8_t channel_id, uint8_t handle_id);
esp_err_t adc1_continous_start();
esp_err_t adc1_get_measurement();

esp_err_t adc1_handle_create();

adc_continuous_handle_t get_adc1_handle();
adc_continuous_config_t* get_adc1_config();

esp_err_t adc2_measurement();
int* get_avg_raw_voltage();

#endif // ADC_VOLTAGE_MEASUREMENT_H