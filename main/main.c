#include <stdio.h>
#include <inttypes.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "time.h"
#include "sys/time.h"

#include "bluetooth_config.h"
#include "ADC_voltage_measurement.h"
#include "freertos_callback.h"

#define SENDING_INTERVAL 500

bool connection_established = false;

void app_main(){

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



    // Initialize ADC1
    ADC_error = adc1_init();
    if(ADC_error != ESP_OK){
        ESP_LOGE(SPP_TAG, "ADC1 initialization failed: %s", esp_err_to_name(ADC_error));
    } else {
        ESP_LOGI(SPP_TAG, "ADC1 initialized successfully");
    }
    // Start ADC1 continuous measurement      
    ADC_error = adc1_continous_start();
    if(ADC_error != ESP_OK){
        ESP_LOGE(SPP_TAG, "ADC continuous start failed: %s", esp_err_to_name(ADC_error));
    } else {
        ESP_LOGI(SPP_TAG, "ADC continuous started successfully");
    }

    // Initialize ADC2
    ADC_error = adc2_init();
    if(ADC_error != ESP_OK){
        ESP_LOGE(SPP_TAG, "ADC2 initialization failed: %s", esp_err_to_name(ADC_error));
    } else {
        ESP_LOGI(SPP_TAG, "ADC2 initialized successfully");
    }

    ADC_error = enable_adc();
    if(ADC_error != ESP_OK){
        ESP_LOGE(SPP_TAG, "ADC enable failed: %s", esp_err_to_name(ADC_error));
    } else {
        ESP_LOGI(SPP_TAG, "ADC enabled successfully");
    }

    TimerHandle_t voltage_measurement_timer = xTimerCreate("voltage_measurement_timer", pdMS_TO_TICKS(SENDING_INTERVAL), pdTRUE, NULL, voltage_measurement);
    xTimerStart(voltage_measurement_timer, 0);

}