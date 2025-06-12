#include <stdio.h>
#include "freertos_callback.h"


esp_err_t freertos_callback_error;

double cells_voltages[PCB_VOLTAGE_DEVIDERS];
double *cell_voltages_p = &cells_voltages[0];


// ADC_timer callback function
// This function is called when the timer expires
void voltage_measurement(TimerHandle_t timer_handler){
    freertos_callback_error = adc1_get_measurement();
    freertos_callback_error = adc2_get_measurement();
    get_avg_raw_voltage();
    cell_voltages_p = calculate_cell_voltages();
    char msg[256];
    int len = snprintf(msg, sizeof(msg), "Voltages:\n");
    for (int i = 0; i < PCB_VOLTAGE_DEVIDERS; i++) {
        len += snprintf(msg + len, sizeof(msg) - len, "%dS: %.2f\n", i + 1, cell_voltages_p[i]);
        if (len >= sizeof(msg) - 10) break;
    }
    if(esp_global_spp_handle != 0 && connection_established == true) {
        freertos_callback_error = esp_spp_write(esp_global_spp_handle, strlen(msg), (uint8_t *)msg);
    }
    if (freertos_callback_error != ESP_OK) {
    ESP_LOGE(SPP_TAG, "Write error: %s", esp_err_to_name(freertos_callback_error));
    }
}
