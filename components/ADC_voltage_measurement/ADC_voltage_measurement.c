#include <stdio.h>
#include "ADC_voltage_measurement.h"

esp_err_t ADC_error;

static int adc_raw[PCB_VOLTAGE_DEVIDERS][ADC_AVERAGE_SAMPLES];
static int adc_raw_sum[PCB_VOLTAGE_DEVIDERS];
static double adc_raw_avg[PCB_VOLTAGE_DEVIDERS];
static double adc_voltages[PCB_VOLTAGE_DEVIDERS];
static double cell_voltages[PCB_VOLTAGE_DEVIDERS];
static int remote_joystick_position;

uint32_t ret_num = 0;
adc_digi_output_data_t adc1_raw[ADC1_CHANNELS_USED] = {0};

/* 
Voltage dividers for each cell on the PCB
{275/375, 220/625, 100/430, 100/540, 75/505, 
100/780, 75/735, 100/1040, 100/1100, 100/1430};
*/
const double voltage_deviders[PCB_VOLTAGE_DEVIDERS] = {0.733, 0.352, 0.233, 0.185, 0.149, 
                                                        0.128, 0.102, 0.096, 0.091, 0.070};


// ADC2 CONFIGURATION

const adc_channel_t adc2_channels[ADC2_CHANNELS_USED] = {
    ADC_CHANNEL_3, // 1S
    ADC_CHANNEL_2, // 2S
    ADC_CHANNEL_0, // 4S
    ADC_CHANNEL_8, // 5S
    ADC_CHANNEL_9, // 6S
    ADC_CHANNEL_7, // 7S
    ADC_CHANNEL_6, // 8S
    ADC_CHANNEL_5, // 9S
    ADC_CHANNEL_4  // 10S
};

adc_oneshot_unit_init_cfg_t adc2_config_unit = {
    .unit_id = ADC_UNIT_2,
    .clk_src = ADC_RTC_CLK_SRC_DEFAULT,
    .ulp_mode = ADC_ULP_MODE_DISABLE,
};

adc_oneshot_unit_handle_t adc2_handle;


adc_oneshot_chan_cfg_t adc2_channels_config = {
    .atten = ADC_ATTEN_DB_12,
    .bitwidth = ADC_BITWIDTH_12,
};

esp_err_t adc2_channels_configure(){
    for(uint8_t channel = 0; channel < ADC2_CHANNELS_USED; channel++){
        ADC_error = adc_oneshot_config_channel(adc2_handle, adc2_channels[channel], &adc2_channels_config);
    }  
    return ADC_error;
}

esp_err_t adc2_init(){
    ADC_error = adc_oneshot_new_unit(&adc2_config_unit, &adc2_handle);
    if (ADC_error != ESP_OK) {
        return ADC_error;
    }
    ADC_error = adc2_channels_configure();
    return ADC_error;
}



esp_err_t adc2_get_measurement(){
    for (uint8_t channel = 0; channel < (ADC2_CHANNELS_USED); channel++){
        for (int sample = 0; sample < ADC_AVERAGE_SAMPLES; sample++){
            if(channel < 2){
                ADC_error = adc_oneshot_read(adc2_handle, adc2_channels[channel], &adc_raw[channel][sample]);
            }
            else{
                ADC_error = adc_oneshot_read(adc2_handle, adc2_channels[channel], &adc_raw[channel + 1][sample]);
            }

            if (ADC_error != ESP_OK) {
                return ADC_error;
            }
        }
    }
    return ADC_error;
}


// ADC1 CONFIGURATION

adc_continuous_handle_t adc1_handle = NULL;

adc_continuous_handle_cfg_t adc1_config = {
    .max_store_buf_size = 1024,
    .conv_frame_size = 256,
};


adc_digi_pattern_config_t adc1_patterns[2] = {
    {
        .atten = ADC_ATTEN_DB_12,
        .channel = ADC_CHANNEL_4,  // 3S
        .unit = ADC_UNIT_1,        // ADC1
        .bit_width = 12
    },
    {
        .atten = ADC_ATTEN_DB_12,
        .channel = ADC_CHANNEL_6,  // REMOTE_JOYSTICK_READING
        .unit = ADC_UNIT_1,        // ADC1
        .bit_width = 12
    }
};

// ADC UNITS CONFIGURATIONS
adc_continuous_config_t adc_config_continuous = {
    .adc_pattern = adc1_patterns,
    .sample_freq_hz = 20000,
    .conv_mode = ADC_CONV_SINGLE_UNIT_1,
    .format = ADC_DIGI_OUTPUT_FORMAT_TYPE1,
    .pattern_num = 2
};

esp_err_t adc1_init(){
    ADC_error = adc_continuous_new_handle(&adc1_config, &adc1_handle);
    if(ADC_error != ESP_OK) {
        return ADC_error;
    }
    ADC_error = adc_continuous_config(adc1_handle, &adc_config_continuous);
    if(ADC_error != ESP_OK) {
        return ADC_error;
    }
    return ADC_error;
}

esp_err_t adc1_continous_start(){
    ADC_error = adc_continuous_start(adc1_handle);
    return ADC_error;
}

esp_err_t adc1_get_measurement(){
    for (uint8_t sample = 0; sample < ADC_AVERAGE_SAMPLES; sample++){
        ADC_error = adc_continuous_read(adc1_handle, (uint8_t*)adc1_raw, sizeof(adc1_raw), &ret_num, 0);
        if (ADC_error != ESP_OK) {
            return ADC_error;
        }

        for (uint32_t i = 0; i < ret_num / sizeof(adc_digi_output_data_t); i++) {
            if (adc1_raw[i].type1.channel == ADC_CHANNEL_4) {
                adc_raw[2][sample] = adc1_raw[i].type1.data;
            } else if (adc1_raw[i].type1.channel == ADC_CHANNEL_6) {
                remote_joystick_position = adc1_raw[i].type1.data;
            }
        }
    }
    return ADC_error;
}

esp_err_t enable_adc(){
    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_DISABLE, // No interrupt
        .mode = GPIO_MODE_OUTPUT,       // Output mode
        .pin_bit_mask = (1ULL << ADC_1_5S_ENABLE_PIN) | (1ULL << ADC_6_10S_ENABLE_PIN), // Enable pins
        .pull_down_en = GPIO_PULLDOWN_DISABLE, // No pull-down
        .pull_up_en = GPIO_PULLUP_DISABLE    // No pull-up
    };
    ADC_error = gpio_config(&io_conf);
    if (ADC_error != ESP_OK) {
        return ADC_error;
    }
    ADC_error = gpio_set_level(ADC_1_5S_ENABLE_PIN, 1); // Enable 1S to 5S measurement
    if (ADC_error != ESP_OK) {
        return ADC_error;
    }
    ADC_error = gpio_set_level(ADC_6_10S_ENABLE_PIN, 1); // Enable 6S to 10S measurement
    if (ADC_error != ESP_OK) {
        return ADC_error;
    }
    return ESP_OK;
}

void get_avg_raw_voltage(){
    for (uint8_t channel = 0; channel < (PCB_VOLTAGE_DEVIDERS); channel++){
        adc_raw_sum[channel] = 0;
        for (uint8_t sample = 0; sample < ADC_AVERAGE_SAMPLES; sample++){
            adc_raw_sum[channel] += adc_raw[channel][sample];
        }
        adc_raw_avg[channel] = adc_raw_sum[channel] / ADC_AVERAGE_SAMPLES;
    }
}


double* calculate_cell_voltages() {
    for (int i = 0; i < PCB_VOLTAGE_DEVIDERS; i++) {
        adc_voltages[i] = (adc_raw_avg[i] / ADC_RESOLUTION) * ADC_VREF;
        cell_voltages[i] = adc_voltages[i] / voltage_deviders[i];
        ESP_LOGI("ADC", "Cell %d: Voltage: %.2f V", i + 1, cell_voltages[i]);
    }
    return cell_voltages;
}

