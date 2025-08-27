#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_timer.h"

#include "main.h"
#include "motor/BLDC6PWMmotor_controller.h"
#include "motor/drivers/BLDC6PWMDriver.h"
#include "PID.h"
#include "HallSensor.h"
#include "SpeedInput.h"

#define TAG "main"

static BLDC6PWMDriver driver(GPIO_MCPWM0A_OUT, GPIO_MCPWM0B_OUT, GPIO_MCPWM1A_OUT, GPIO_MCPWM1B_OUT,
                             GPIO_MCPWM2A_OUT, GPIO_MCPWM2B_OUT,
                             GPIO_MCPWM_CAP0_IN, GPIO_MCPWM_CAP1_IN, GPIO_MCPWM_CAP2_IN,
                             GPIO_DRIVER_ENABLE);
static SpeedInput throttle(GPIO_THROTTLE_IN);
static HallSensor hall(GPIO_MCPWM_CAP0_IN, GPIO_MCPWM_CAP1_IN, GPIO_MCPWM_CAP2_IN, MOTOR_POLE_PAIRS);
static PID speed_pid(SPEED_PID_KP, SPEED_PID_KI, SPEED_PID_KD, 0.0f, MAX_DRIVE_VOLTAGE, MAX_DRIVE_VOLTAGE);

static void shutdown_motor(bool &running) {
    if (running) {
        StopMotor();
        driver.disable();
        running = false;
    }
}

static void control_task(void *arg) {
    bool running = false;
    float target_velocity = 0.0f; // slew-limited electrical rad/s
    const float dt = CONTROL_PERIOD_MS / 1000.0f;
    TickType_t last_wake = xTaskGetTickCount();

    while (true) {
        float pedal = throttle.read();
        hall.update();

        // Start / stop with hysteresis so a noisy pot at rest doesn't chatter
        if (!running && pedal > THROTTLE_START) {
            ResetOpenLoop();
            speed_pid.reset();
            target_velocity = 0.0f;
            driver.enable();
            running = true;
            ESP_LOGI(TAG, "Motor started");
        } else if (running && pedal < THROTTLE_STOP) {
            shutdown_motor(running);
            ESP_LOGI(TAG, "Motor stopped");
        }

        if (running) {
            // Move the target toward the throttle at a limited acceleration
            float wanted = pedal * MAX_ELECTRICAL_VELOCITY;
            float step = MAX_ACCELERATION * dt;
            if (wanted > target_velocity + step) target_velocity += step;
            else if (wanted < target_velocity - step) target_velocity -= step;
            else target_velocity = wanted;

#if CONTROL_MODE == CONTROL_CLOSED_LOOP
            if (!hall.isValid()) {
                // Invalid hall code means a wiring fault, don't drive blind
                ESP_LOGE(TAG, "Invalid hall reading, shutting down");
                shutdown_motor(running);
            } else {
                float error = target_velocity - hall.getElectricalVelocity();
                float vq = speed_pid.update(error, dt);
                // Hold the field 90 degrees ahead of the rotor to produce torque
                UpdateBLDCMotor(vq, NormalizeAngle(hall.getElectricalAngle() + PI / 2.0));
            }
#else
            VelocityOpenLoop(target_velocity, OPEN_LOOP_VOLTAGE);
#endif
        }

        vTaskDelayUntil(&last_wake, pdMS_TO_TICKS(CONTROL_PERIOD_MS));
    }
}

extern "C" void app_main(void)
{
    // init() sets up the GPIOs and configures the MCPWM timers, outputs stay off until enable()
    driver.init();
    AttachDriver(&driver);
    modulation = Space_Vector;

    throttle.init();
    hall.init();

    xTaskCreatePinnedToCore(control_task, "control", 4096, NULL, 10, NULL, 1);
}
