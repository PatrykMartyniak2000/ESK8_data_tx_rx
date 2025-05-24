idf_component_register(SRCS "freertos_callback.c"
                    INCLUDE_DIRS "include"
                    REQUIRES bluetooth_config ADC_voltage_measurement)
