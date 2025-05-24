idf_component_register(SRCS "bluetooth_config.c"
                    INCLUDE_DIRS "include"
                    REQUIRES nvs_flash bt vfs ADC_voltage_measurement freertos_callback)
