/* Switch demo implementation using button and RGB LED
   
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <sdkconfig.h>

#include <iot_button.h>
#include <esp_rmaker_core.h>
#include <esp_rmaker_standard_types.h>
#include <esp_rmaker_standard_params.h> 
#include "esp_log.h"

#include <app_reset.h>
#include <ws2812_led.h>
#include "app_priv.h"

#include "dht21.h"

/* This is the button that is used for toggling the power */
#define BUTTON_GPIO          CONFIG_ESP32_BOARD_BUTTON_GPIO
#define BUTTON_ACTIVE_LEVEL  0

/* This is the GPIO on which the power will be set */
#define OUTPUT_GPIO    CONFIG_ESP32_OUTPUT_GPIO
static bool g_power_state = DEFAULT_POWER;

/* These values correspoind to H,S,V = 120,100,10 */
#define DEFAULT_RED     0
#define DEFAULT_GREEN   25
#define DEFAULT_BLUE    0

#define WIFI_RESET_BUTTON_TIMEOUT       3
#define FACTORY_RESET_BUTTON_TIMEOUT    10

static TimerHandle_t t_sensor_timer;
static TimerHandle_t h_sensor_timer;
static const char *TAG = "ESP-RMAKER-SENSOR";

static void app_indicator_set(bool state)
{
    if (state) {
        ws2812_led_set_rgb(DEFAULT_RED, DEFAULT_GREEN, DEFAULT_BLUE);
    } else {
        ws2812_led_clear();
    }
}

static void app_indicator_init(void)
{
    ws2812_led_init();
    app_indicator_set(g_power_state);
}
static void push_btn_cb(void *arg)
{
    bool new_state = !g_power_state;
    app_driver_set_state(new_state);
    esp_rmaker_param_update_and_report(
            esp_rmaker_device_get_param_by_name(switch_device, ESP_RMAKER_DEF_POWER_NAME),
            esp_rmaker_bool(new_state));
}

static void set_power_state(bool target)
{
    gpio_set_level(OUTPUT_GPIO, target);
    app_indicator_set(target);
}

void turnOffSwitchAfterSetInterval(void *pvParameters)
{
    while (1)
    {
        if (app_driver_get_state())
        {   
            vTaskDelay(10000 / portTICK_PERIOD_MS);    
            app_driver_set_state(false);
            esp_rmaker_param_update_and_report(
                esp_rmaker_device_get_param_by_name(switch_device, ESP_RMAKER_DEF_POWER_NAME),
                esp_rmaker_bool(false));
        }
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}


/* temperature and humidity sensor */
static void update_temperature_sensor_data(TimerHandle_t handle)
{
    ESP_LOGI(TAG, "Updating Temp: %.1fC\n", get_average_temperature());
    esp_rmaker_param_update_and_report(
                esp_rmaker_device_get_param_by_type(temp_sensor_device, ESP_RMAKER_PARAM_TEMPERATURE),
                esp_rmaker_float(get_average_temperature()));
}

static void update_humidity_sensor_data(TimerHandle_t handle)
{
    ESP_LOGI(TAG, "Updating Humidity: %.1f%%\n", get_average_humidity());
    esp_rmaker_param_update_and_report(
                esp_rmaker_device_get_param_by_type(humidity_sensor_device, ESP_RMAKER_PARAM_TEMPERATURE),
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

esp_err_t humidity_sensor_init(void)
{
    h_sensor_timer = xTimerCreate("update_humidity_sensor_data", (REPORTING_PERIOD * 1000) / portTICK_PERIOD_MS,
                            pdTRUE, NULL, update_humidity_sensor_data);
    if (h_sensor_timer) {
        xTimerStart(h_sensor_timer, 0);
        return ESP_OK;
    }
    return ESP_FAIL;
}
/* temperature and humidity sensor - END */

void app_driver_init()
{
    button_handle_t btn_handle = iot_button_create(BUTTON_GPIO, BUTTON_ACTIVE_LEVEL);
    if (btn_handle) {
        /* Register a callback for a button tap (short press) event */
        iot_button_set_evt_cb(btn_handle, BUTTON_CB_TAP, push_btn_cb, NULL);
        /* Register Wi-Fi reset and factory reset functionality on same button */
        app_reset_button_register(btn_handle, WIFI_RESET_BUTTON_TIMEOUT, FACTORY_RESET_BUTTON_TIMEOUT);
    }

    /* Configure power */
    gpio_config_t io_conf = {
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = 1,
    };
    io_conf.pin_bit_mask = ((uint64_t)1 << OUTPUT_GPIO);
    /* Configure the GPIO */
    gpio_config(&io_conf);
    app_indicator_init();
    
    /* Init the temperature and humidity sensor */
    temperature_sensor_init();
    humidity_sensor_init();
    xTaskCreate(set_average_temperature_humidity, "set_average_temperature_humidity", configMINIMAL_STACK_SIZE * 3, NULL, 5, NULL);
    xTaskCreate(turnOffSwitchAfterSetInterval, "turnOffSwitchAfterSetInterval", configMINIMAL_STACK_SIZE * 3, NULL, 5, NULL);
    
}


int IRAM_ATTR app_driver_set_state(bool state)
{
    if(g_power_state != state) {
        g_power_state = state;
        set_power_state(g_power_state);
    }
    return ESP_OK;
}

bool app_driver_get_state(void)
{
    return g_power_state;
}
