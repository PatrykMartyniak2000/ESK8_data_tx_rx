#include <stdio.h>
#include "freertos_callback.h"


esp_err_t freertos_callback_error;

int avg_voltage[ADC1_CHANNELS_USED + ADC2_CHANNELS_USED];
int *avg_voltage_ptr = &avg_voltage[0];


// ADC_timer callback function
// This function is called when the timer expires
void voltage_measurement(TimerHandle_t timer_handler){
    //freertos_callback_error = adc1_get_measurement();
    freertos_callback_error = adc2_measurement();
    avg_voltage_ptr = get_avg_raw_voltage();
    char msg[256];
    int len = snprintf(msg, sizeof(msg), "Voltages:\n");
    for (int i = 0; i < ADC1_CHANNELS_USED + ADC2_CHANNELS_USED; i++) {
        len += snprintf(msg + len, sizeof(msg) - len, "%dS: %d\n", i + 1, avg_voltage_ptr[i]);
        if (len >= sizeof(msg) - 10) break;
    }
    if(esp_global_spp_handle != 0){
        esp_spp_write(esp_global_spp_handle, strlen(msg), (uint8_t *)msg);
    }
}
