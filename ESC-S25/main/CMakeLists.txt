set(SOURCES
    src/main.cpp
    src/PWM/BLDC6PWMmcpwm.cpp
    src/motor/BLDC6PWMmotor_controller.cpp
    src/motor/drivers/BLDC6PWMDriver.cpp
    src/control/PID.cpp
    src/sensors/HallSensor.cpp
    src/input/SpeedInput.cpp)

idf_component_register(SRCS ${SOURCES}
                       INCLUDE_DIRS inc inc/PWM inc/misc inc/motor inc/motor/drivers inc/control inc/sensors inc/input
                       REQUIRES driver esp_timer esp_adc)
