#include <stdio.h>
#include <inttypes.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "time.h"
#include "sys/time.h"

#include "mpu6050.h"
#include "bluetooth_config.h"


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
    
}