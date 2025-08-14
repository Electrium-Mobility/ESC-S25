set(SOURCES
    src/main.cpp
    src/PWM/BLDC6PWMmcpwm.cpp
    src/motor/BLDC6PWMmotor_controller.cpp
    src/motor/drivers/BLDC6PWMDriver.cpp)

idf_component_register(SRCS ${SOURCES}
                       INCLUDE_DIRS inc inc/PWM inc/misc inc/motor inc/motor/drivers
                       REQUIRES driver esp_timer)
