#include "HallSensor.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include "esp_log.h"
#include <math.h>

#define TAG "HallSensor"

#define HALL_TIMEOUT_US 200000 // no edge for 200 ms -> treat as stopped

// Hall code (A<<2 | B<<1 | C) to sector 0..5 for a standard 120 degree sensor layout, -1 = invalid
static const int hall_to_sector[8] = {-1, 0, 2, 1, 4, 5, 3, -1};

HallSensor::HallSensor(int pin_a, int pin_b, int pin_c, int pole_pairs)
    : pin_a(pin_a), pin_b(pin_b), pin_c(pin_c), pole_pairs(pole_pairs < 1 ? 1 : pole_pairs) {}

void HallSensor::init(void) {
    const int pins[3] = {pin_a, pin_b, pin_c};
    for (int pin : pins) {
        gpio_config_t cfg = {};
        cfg.pin_bit_mask = 1ULL << pin;
        cfg.mode = GPIO_MODE_INPUT;
        cfg.pull_up_en = GPIO_PULLUP_ENABLE;
        cfg.pull_down_en = GPIO_PULLDOWN_DISABLE;
        cfg.intr_type = GPIO_INTR_DISABLE;
        ESP_ERROR_CHECK(gpio_config(&cfg));
    }
    ESP_LOGI(TAG, "Hall sensor initialized on pins %d, %d, %d", pin_a, pin_b, pin_c);
}

void HallSensor::update(void) {
    int code = (gpio_get_level((gpio_num_t)pin_a) << 2) |
               (gpio_get_level((gpio_num_t)pin_b) << 1) |
                gpio_get_level((gpio_num_t)pin_c);
    int sector = hall_to_sector[code];
    int64_t now = esp_timer_get_time();

    if (sector < 0) {
        valid = false; // wiring fault or sensor not powered
        return;
    }
    if (reversed) {
        sector = 5 - sector;
    }
    valid = true;
    electrical_angle = (float)((sector + 0.5) * (M_PI / 3.0));

    if (last_sector < 0) {
        last_sector = sector;
        last_edge_us = now;
        return;
    }

    if (sector != last_sector) {
        // Step is +1 or -1 sector (mod 6); anything else is a glitch, ignore it
        int step = (sector - last_sector + 6) % 6;
        if (step == 1 || step == 5) {
            float dt = (now - last_edge_us) * 1e-6f;
            if (dt > 0.0f) {
                float v = (float)(M_PI / 3.0) / dt;
                electrical_velocity = (step == 1) ? v : -v;
            }
        }
        last_sector = sector;
        last_edge_us = now;
    } else if (now - last_edge_us > HALL_TIMEOUT_US) {
        electrical_velocity = 0.0f;
    }
}
