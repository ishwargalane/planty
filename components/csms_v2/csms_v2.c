#include <stdio.h>
#include "csms_v2.h"

#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"

static int adc_raw[NUM_CHANNELS];
static int adc_data[NUM_CHANNELS];
static int voltage[NUM_CHANNELS];

const static char *TAG = "CSMC_V2";

static bool example_adc_calibration_init(adc_unit_t unit, adc_channel_t channel, adc_atten_t atten, adc_cali_handle_t *out_handle);
static void example_adc_calibration_deinit(adc_cali_handle_t handle);

// Function to retrive the average soil moisture data stored in the global variable adc_data
const int* get_average_soil_moisture_data() {
    return adc_data; // Return the array's pointer
}

// Function to select the multiplexer channel
void select_mux_channel(uint8_t channel) {
    gpio_set_level(MUX_S0_PIN, channel & 0x01);
    gpio_set_level(MUX_S1_PIN, (channel >> 1) & 0x01);
    gpio_set_level(MUX_S2_PIN, (channel >> 2) & 0x01);
    gpio_set_level(MUX_S3_PIN, (channel >> 3) & 0x01);
}

// Function to set the average soil moisture data
void set_average_soil_moisture_data(void *pvParameters)
{
    // Initialize GPIO pins for multiplexer control
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << MUX_S0_PIN) | (1ULL << MUX_S1_PIN) | (1ULL << MUX_S2_PIN) | (1ULL << MUX_S3_PIN),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&io_conf);

    //-------------ADC1 Init---------------//
    adc_oneshot_unit_handle_t adc1_handle;
    adc_oneshot_unit_init_cfg_t init_config1 = {
        .unit_id = ADC_UNIT_1,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config1, &adc1_handle));

    //-------------ADC1 Config---------------//
    adc_oneshot_chan_cfg_t config = {
        .bitwidth = ADC_BITWIDTH_DEFAULT,
        .atten = EXAMPLE_ADC_ATTEN,
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, EXAMPLE_ADC1_CHAN0, &config));

    //-------------ADC1 Calibration Init---------------//
    adc_cali_handle_t adc1_cali_chan0_handle = NULL;
    bool do_calibration1_chan0 = example_adc_calibration_init(ADC_UNIT_1, EXAMPLE_ADC1_CHAN0, EXAMPLE_ADC_ATTEN, &adc1_cali_chan0_handle);

    while (1) {
        for (uint8_t channel = 0; channel < NUM_CHANNELS; channel++) {
            int adc_sum = 0;
            int voltage_sum = 0;
            for (int i = 0; i < SAMPLE_COUNT; i++) {
                select_mux_channel(channel);
                vTaskDelay(50 / portTICK_PERIOD_MS); // wait for the mux to settle
                ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, EXAMPLE_ADC1_CHAN0, &adc_raw[channel]));
                adc_sum += adc_raw[channel];
                if (do_calibration1_chan0) {
                    ESP_ERROR_CHECK(adc_cali_raw_to_voltage(adc1_cali_chan0_handle, adc_raw[channel], &voltage[channel]));
                    voltage_sum += voltage[channel];
                }
                vTaskDelay(100 / portTICK_PERIOD_MS); // interval between readings
            }
            adc_data[channel] = adc_sum / SAMPLE_COUNT;
            if (do_calibration1_chan0) {
                voltage[channel] = voltage_sum / SAMPLE_COUNT;
            }
            //ESP_LOGI(TAG, "Updated Channel [%d]: %d mV ", channel, voltage[channel]);
            vTaskDelay(100 / portTICK_PERIOD_MS); // interval between readings
        }

        vTaskDelay(1000 / portTICK_PERIOD_MS); // interval between cycles
    }

    //Tear Down
    ESP_ERROR_CHECK(adc_oneshot_del_unit(adc1_handle));
    if (do_calibration1_chan0) {
        example_adc_calibration_deinit(adc1_cali_chan0_handle);
    }
    vTaskDelete(NULL);
}

/*---------------------------------------------------------------
    ADC Calibration
---------------------------------------------------------------*/
static bool example_adc_calibration_init(adc_unit_t unit, adc_channel_t channel, adc_atten_t atten, adc_cali_handle_t *out_handle)
{
    adc_cali_handle_t handle = NULL;
    esp_err_t ret = ESP_FAIL;
    bool calibrated = false;

    // Only line fitting calibration scheme is supported for ESP32.
    if (!calibrated) {
    ESP_LOGI(TAG, "calibration scheme version is %s", "Line Fitting");
    adc_cali_line_fitting_config_t cali_config = {
        .unit_id = unit,
        .atten = atten,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };
    ret = adc_cali_create_scheme_line_fitting(&cali_config, &handle);
    if (ret == ESP_OK) {
        calibrated = true;
    }
    }

    *out_handle = handle;
    if (ret == ESP_OK) {
    ESP_LOGI(TAG, "Calibration Success");
    } else if (ret == ESP_ERR_NOT_SUPPORTED || !calibrated) {
    ESP_LOGW(TAG, "eFuse not burnt, skip software calibration");
    } else {
    ESP_LOGE(TAG, "Invalid arg or no memory");
    }

    return calibrated;
}

static void example_adc_calibration_deinit(adc_cali_handle_t handle)
{
    // Only line fitting calibration scheme is supported for ESP32.
    ESP_LOGI(TAG, "deregister %s calibration scheme", "Line Fitting");
    ESP_ERROR_CHECK(adc_cali_delete_scheme_line_fitting(handle));
}

void display_average_soil_moisture_data(void *pvParameters) {
    while (true) {
        char mean_values[256] = {0};
        snprintf(mean_values, sizeof(mean_values), "Mean Values:");

        for (int channel = 0; channel < NUM_CHANNELS; channel++) {
            char buffer[16];
            snprintf(buffer, sizeof(buffer), " %d:%d", channel, adc_data[channel]);
            strncat(mean_values, buffer, sizeof(mean_values) - strlen(mean_values) - 1);
        }

        ESP_LOGI(TAG, "%s", mean_values);
        vTaskDelay(pdMS_TO_TICKS(30000)); // Display every 30 seconds
    }
}
