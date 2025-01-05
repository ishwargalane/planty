#include "app_priv.h"
#include "csms_v2.h"

#include <freertos/FreeRTOS.h>
#include <freertos/timers.h>
#include <esp_rmaker_standard_types.h>
#include <string.h>

#include "esp_log.h"

static const char *TAG = "CSMC_V2_SENSOR";
static TimerHandle_t s_sensor_timer;

/* soil moisture sensor */
static void update_soil_moisture_sensor_data(TimerHandle_t handle)
{
    const int* adc_values = get_average_soil_moisture_data();
    char mean_values[256] = {0};
    snprintf(mean_values, sizeof(mean_values), "Soil Moisture Values:");

    for (int channel = 0; channel < NUM_CHANNELS; channel++) {
        char buffer[NUM_CHANNELS];
        snprintf(buffer, sizeof(buffer), " %d:%d", channel, adc_values[channel]);
        strncat(mean_values, buffer, sizeof(mean_values) - strlen(mean_values) - 1);
    }

    ESP_LOGI(TAG, "%s", mean_values);
    
    /*
    esp_rmaker_param_update_and_report(
                esp_rmaker_device_get_param_by_type(temp_sensor_device, ESP_RMAKER_PARAM_TEMPERATURE),
                esp_rmaker_float(get_average_temperature()));

    */  
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
