#include "SpeedInput.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_log.h"
#include <stdlib.h>

#define TAG "SpeedInput"

#define ADC_MAX 4095.0f
#define FILTER_ALPHA 0.2f // low pass, higher = faster response

SpeedInput::SpeedInput(int pin, float deadband) : pin(pin), deadband(deadband) {}

void SpeedInput::init(void) {
    adc_unit_t unit;
    adc_channel_t ch;
    ESP_ERROR_CHECK(adc_oneshot_io_to_channel(pin, &unit, &ch));
    if (unit != ADC_UNIT_1) {
        ESP_LOGE(TAG, "GPIO %d is not on ADC1 (ADC2 conflicts with WiFi and is not supported)", pin);
        abort();
    }
    channel = ch;

    adc_oneshot_unit_handle_t h;
    adc_oneshot_unit_init_cfg_t unit_cfg = {};
    unit_cfg.unit_id = ADC_UNIT_1;
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&unit_cfg, &h));

    adc_oneshot_chan_cfg_t chan_cfg = {};
    chan_cfg.atten = ADC_ATTEN_DB_12;
    chan_cfg.bitwidth = ADC_BITWIDTH_12;
    ESP_ERROR_CHECK(adc_oneshot_config_channel(h, ch, &chan_cfg));
    handle = h;

    ESP_LOGI(TAG, "Speed input initialized on GPIO %d", pin);
}

float SpeedInput::read(void) {
    int raw = 0;
    if (adc_oneshot_read((adc_oneshot_unit_handle_t)handle, (adc_channel_t)channel, &raw) != ESP_OK) {
        return 0.0f;
    }
    float x = raw / ADC_MAX;
    filtered += FILTER_ALPHA * (x - filtered);

    // Remove the deadband and rescale so full travel still reaches 1.0
    if (filtered < deadband) return 0.0f;
    float out = (filtered - deadband) / (1.0f - deadband);
    return out > 1.0f ? 1.0f : out;
}
