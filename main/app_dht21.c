#include "app_priv.h"
#include "dht21.h"

#include <freertos/FreeRTOS.h>
#include <freertos/timers.h>
#include <esp_rmaker_standard_types.h>

#include "esp_log.h"

static const char *TAG = "DHT_SENSOR";
static TimerHandle_t t_sensor_timer;

/* temperature and humidity sensor */
static void update_temperature_sensor_data(TimerHandle_t handle)
{
    ESP_LOGI(TAG, "Updating Temp: %.1fC\n", get_average_temperature());
    esp_rmaker_param_update_and_report(
            // _by_type returns the first parameter of the given type. If you have multiple parameters of the same type, you should use _by_name. 'temperature' is the name of the parameter, in case you want to use _by_name
            esp_rmaker_device_get_param_by_type(temp_sensor_device, ESP_RMAKER_PARAM_TEMPERATURE), 
            esp_rmaker_float(get_average_temperature()));
    ESP_LOGI(TAG, "Updating Humidity: %.1f%%\n", get_average_humidity());
    esp_rmaker_param_update_and_report(
            esp_rmaker_device_get_param_by_name(temp_sensor_device, "Humidity"),    // 'Humidity' is the name of the parameter
            esp_rmaker_float(get_average_humidity()));
}

esp_err_t temperature_sensor_init(void)
{
    t_sensor_timer = xTimerCreate("update_temperature_sensor_data", (REPORTING_PERIOD * 1000) / portTICK_PERIOD_MS,
                            pdTRUE, NULL, update_temperature_sensor_data);
    if (t_sensor_timer) {
        xTimerStart(t_sensor_timer, 0);
        return ESP_OK;
    }
    return ESP_FAIL;
}

/* temperature and humidity sensor - END */