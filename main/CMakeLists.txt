idf_component_register(SRCS "main.c"
                    INCLUDE_DIRS "."
                    REQUIRES bluetooth_config mpu6050 bt ADC_voltage_measurement freertos_callback)