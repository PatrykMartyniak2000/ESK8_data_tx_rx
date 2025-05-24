#include <stdio.h>
#include <inttypes.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "time.h"
#include "sys/time.h"

#include "mpu6050.h"
#include "bluetooth_config.h"
#include "ADC_voltage_measurement.h"
#include "freertos_callback.h"



void app_main(){

    ADC_error = adc1_handle_create();
    ADC_error = adc_continuous_config(get_adc1_handle(), get_adc1_config());
    ADC_error = adc1_continous_start();
    ADC_error = adc_oneshot_new_unit(get_adc2_config_unit(), get_adc2_unit_handle());

    ADC_error = esp_bt_sleep_disable();


    TimerHandle_t voltage_measurement_timer = xTimerCreate("voltage_measurement_timer", pdMS_TO_TICKS(5000), pdTRUE, NULL, voltage_measurement);
    xTimerStart(voltage_measurement_timer, 0);

    // INITIALIZE NVS
    ESP_ERROR_CHECK(nvs_init());
    // INITIALIZE BLUETOOTH CONTROLLER
    ESP_ERROR_CHECK(bt_controller_init());
    // INITIALIZE BLUEDROID
    ESP_ERROR_CHECK(bt_bluedroid_init());
    // INITIALIZE GAP
    ESP_ERROR_CHECK(bt_gap_init());
    // INITIALIZE SPP
    ESP_ERROR_CHECK(bt_spp_init());

}