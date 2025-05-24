#include <stdio.h>
#include "ADC_voltage_measurement.h"

esp_err_t ADC_error;

static int adc_raw[ADC1_CHANNELS_USED + ADC2_CHANNELS_USED][ADC_AVERAGE_SAMPLES];
static int adc_raw_sum[ADC1_CHANNELS_USED + ADC2_CHANNELS_USED];
static int adc_raw_avg[ADC1_CHANNELS_USED + ADC2_CHANNELS_USED];
static int adc_voltages[ADC1_CHANNELS_USED + ADC2_CHANNELS_USED];


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

adc_oneshot_unit_init_cfg_t* get_adc2_config_unit(){
    return &adc2_config_unit;
}

adc_oneshot_unit_handle_t adc2_handle;

adc_oneshot_unit_handle_t* get_adc2_unit_handle(){
    return &adc2_handle;
}

adc_oneshot_chan_cfg_t adc2_channels_config = {
    .atten = ADC_ATTEN_DB_12,
    .bitwidth = ADC_BITWIDTH_12,
};

esp_err_t adc2_channels_configure(uint8_t channel_id, uint8_t handle_id){
    for(uint8_t channel = 0; channel < ADC2_CHANNELS_USED; channel++){
        ADC_error = adc_oneshot_config_channel(adc2_handle, adc2_channels[channel], &adc2_channels_config);
    }  
    return ADC_error;
}


esp_err_t adc2_measurement(){
    for (uint8_t channel = 0; channel < (ADC1_CHANNELS_USED + ADC2_CHANNELS_USED); channel++){
        for (int sample = 0; sample < ADC_AVERAGE_SAMPLES; sample++){
            if(channel == 2){
                channel++;
            }
            ADC_error = adc_oneshot_read(adc2_handle, adc2_channels[channel], &adc_raw[channel][sample]);
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

esp_err_t adc1_handle_create(){
    ADC_error = (adc_continuous_new_handle(&adc1_config, &adc1_handle));
    return ADC_error;
}


adc_continuous_handle_t get_adc1_handle(){
    return adc1_handle;
}

adc_digi_pattern_config_t adc1_pattern = {
    .atten = ADC_ATTEN_DB_12,
    .channel = ADC_CHANNEL_4,  // 3S
    .unit = ADC_UNIT_1, // ADC1
    .bit_width = 12
};

// ADC UNITS CONFIGURATIONS
adc_continuous_config_t adc_config_continuous = {
    .adc_pattern = &adc1_pattern,
    .sample_freq_hz = 20000,
    .conv_mode = ADC_CONV_SINGLE_UNIT_1,
    .format = ADC_DIGI_OUTPUT_FORMAT_TYPE1,
    .pattern_num = 1
};

adc_continuous_config_t* get_adc1_config(){
    return &adc_config_continuous;
}

esp_err_t adc1_continous_start(){
    ADC_error = adc_continuous_start(adc1_handle);
    return ADC_error;
}

uint32_t ret_num = 0;
uint8_t result[ADC_AVERAGE_SAMPLES * sizeof(adc_digi_output_data_t)] = {0};

esp_err_t adc1_get_measurement(){
    for (uint8_t sample = 0; sample < ADC_AVERAGE_SAMPLES; sample++){
        ADC_error = adc_continuous_read(adc1_handle, result, sizeof(result), &ret_num, 0);
    }
    return ADC_error;
}



int* get_avg_raw_voltage(){
    for (uint8_t channel = 0; channel < (ADC1_CHANNELS_USED + ADC2_CHANNELS_USED); channel++){
        adc_raw_sum[channel] = 0;
        for (uint8_t sample = 0; sample < ADC_AVERAGE_SAMPLES; sample++){
            adc_raw_sum[channel] += adc_raw[channel][sample];
        }
        adc_raw_avg[channel] = adc_raw_sum[channel] / ADC_AVERAGE_SAMPLES;
        ESP_LOGI("ADC", "Channel %d: Raw Value: %d", channel, adc_raw_avg[channel]);
    }
    return adc_raw_avg;
}

