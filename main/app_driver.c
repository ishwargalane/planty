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
#include "csms_v2.h"

/* This is the button that is used for toggling the power */
#define BUTTON_GPIO          CONFIG_ESP32_BOARD_BUTTON_GPIO
#define BUTTON_ACTIVE_LEVEL  0
#define BUTTON_SWITCH_OFF_INTERVAL  CONFIG_ESP32_BOARD_BUTTON_GPIO_SWITCH_OFF_INTERVAL

/* This is the GPIO on which the power will be set */
#define OUTPUT_GPIO    CONFIG_ESP32_OUTPUT_GPIO
static bool g_power_state = DEFAULT_POWER;

/* These values correspoind to H,S,V = 120,100,10 */
#define DEFAULT_RED     0
#define DEFAULT_GREEN   25
#define DEFAULT_BLUE    0

#define WIFI_RESET_BUTTON_TIMEOUT       3
#define FACTORY_RESET_BUTTON_TIMEOUT    10

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

/* 
This is to switch off the water supply after a set interval. The interval is set to 10 seconds.
This is to avoid the water supply being on for a long time.
*/
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
            ESP_LOGI(TAG, "Switch turned off after set interval of %d seconds", BUTTON_SWITCH_OFF_INTERVAL);
        }
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}

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
    /* Create a task to set the average temperature and humidity */
    xTaskCreate(set_average_temperature_humidity, "set_average_temperature_humidity", configMINIMAL_STACK_SIZE * 3, NULL, 5, NULL);
    /* Create a task to turn off the switch after a set interval */
    xTaskCreate(turnOffSwitchAfterSetInterval, "turnOffSwitchAfterSetInterval", configMINIMAL_STACK_SIZE * 3, NULL, 5, NULL);
    /* Create a task to set the soil moisture data */
    xTaskCreate(set_average_soil_moisture_data, "set_average_soil_moisture_data", 4096, NULL, 5, NULL);
    /* Create a task to display the soil moisture data */
    xTaskCreate(display_average_soil_moisture_data, "display_average_soil_moisture_data", 4096, NULL, 5, NULL);
    /* Create a task to update the temperature sensor data */
    soil_moisture_sensor_init();
}
