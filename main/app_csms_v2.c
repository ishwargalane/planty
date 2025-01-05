#include "app_priv.h"
#include "csms_v2.h"

#include <freertos/FreeRTOS.h>
#include <freertos/timers.h>
#include <esp_rmaker_standard_types.h>
#include <string.h>

#include "esp_log.h"

static const char *TAG = "CSMC_V2_SENSOR";
static TimerHandle_t s_sensor_timer;

#define PARAM_NAME_PREFIX "Plant_"

// Sensor value range definitions
#define SENSOR_MAX_RAW 2500  // 0% moisture
#define SENSOR_MIN_RAW 500   // 100% moisture


static int soil_moisture_percent[NUM_CHANNELS] = {0};

// function to create/add a soil moisture sensor device parameter to the device
void add_soil_moisture_params(soil_monitor_t* soil_monitor)
{
    for(int i = 0; i < NUM_CHANNELS; i++) {
        char param_name[32];
        snprintf(param_name, sizeof(param_name), "%s%d", PARAM_NAME_PREFIX, i);
        esp_rmaker_param_t *moisture_param = esp_rmaker_param_create(param_name, ESP_RMAKER_PARAM_SOIL_MOISTURE, 
            esp_rmaker_int(soil_moisture_percent[i]), PROP_FLAG_READ | PROP_FLAG_TIME_SERIES);
        if (moisture_param) {
            esp_rmaker_param_add_ui_type(moisture_param, ESP_RMAKER_UI_SLIDER);
            esp_rmaker_param_add_bounds(moisture_param, esp_rmaker_int(0), esp_rmaker_int(100), esp_rmaker_int(1));
            esp_rmaker_device_add_param(soil_monitor->device, moisture_param);
            soil_monitor->moisture_params[i] = moisture_param;
        }
    }
}   

/* soil moisture sensor */
static void update_soil_moisture_sensor_data(TimerHandle_t handle)
{
    const int* adc_values = get_average_soil_moisture_data();
    soil_monitor_t soil_monitor = *get_soil_monitor();

    char mean_values[256] = {0};
    snprintf(mean_values, sizeof(mean_values), "Soil Moisture Values:");

    for (int channel = 0; channel < NUM_CHANNELS; channel++) {
        char buffer[NUM_CHANNELS];
        snprintf(buffer, sizeof(buffer), " %d:%d", channel, adc_values[channel]);
        strncat(mean_values, buffer, sizeof(mean_values) - strlen(mean_values) - 1);
        soil_moisture_percent[channel] = (SENSOR_MAX_RAW - adc_values[channel]) * 100 / (SENSOR_MAX_RAW - SENSOR_MIN_RAW);
        esp_rmaker_param_update_and_report(soil_monitor.moisture_params[channel], esp_rmaker_int(soil_moisture_percent[channel]));
    }

    ESP_LOGI(TAG, "%s", mean_values);
    
}


esp_err_t soil_moisture_sensor_init(void)
{
    s_sensor_timer = xTimerCreate("update_soil_moisture_sensor_data", (CSMSV2_REPORTING_PERIOD * 1000) / portTICK_PERIOD_MS,
                            pdTRUE, NULL, update_soil_moisture_sensor_data);
    if (s_sensor_timer) {
        xTimerStart(s_sensor_timer, 0);
        return ESP_OK;
    }
    return ESP_FAIL;
}
/* soil moisture sensor - END */
