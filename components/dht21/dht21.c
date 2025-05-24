#include <stdio.h>
#include "dht21.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>

#define NUM_READINGS 12
static float g_temperature = DEFAULT_TEMPERATURE;
static float g_humidity = DEFAULT_HUMIDITY;

static const char *TAG = "DHT_SENSOR";
float get_average_temperature(void)
{
    return ((int)(g_temperature * 100 + 0.5)) / 100.0;
}
float get_average_humidity(void)
{
    return ((int)(g_temperature * 100 + 0.5)) / 100.0;
    //return (int)(g_humidity + 0.5); //to round off to the nearest integer
}

void set_average_temperature_humidity(void *pvParameters)
{
#ifdef CONFIG_EXAMPLE_INTERNAL_PULLUP
    gpio_set_pull_mode(dht_gpio, GPIO_PULLUP_ONLY);
#endif

    while (1)
    {
        float total_temperature = 0;
        float total_humidity = 0;
        float humidity = 0;
        float temperature = 0;
        for (int i = 0; i < NUM_READINGS; i++) {
            if (dht_read_float_data(SENSOR_TYPE, CONFIG_DHT_DATA_GPIO, &humidity, &temperature) == ESP_OK){
                total_temperature += temperature;
                total_humidity += humidity;
            }
            else{
                printf("Could not read data from sensor\n");
            }
            
        // If you read the sensor data too often, it will heat up
        // http://www.kandrsmith.org/RJS/Misc/Hygrometers/dht_sht_how_fast.html
        vTaskDelay(5000 / portTICK_PERIOD_MS);

        }
        g_temperature = total_temperature / NUM_READINGS;
        g_humidity = total_humidity / NUM_READINGS;
        ESP_LOGI(TAG, "Humidity: %.1f%% Temp: %.1fC\n", get_average_humidity(), get_average_temperature());
    }
}