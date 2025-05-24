#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_spp_api.h"

#include "bluetooth_config.h"
#include "ADC_voltage_measurement.h"

#include "esp_log.h"
#include "esp_err.h"


void voltage_measurement(TimerHandle_t timer_handler);