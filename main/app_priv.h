/*
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <esp_rmaker_core.h>
#include <esp_err.h>
#include "csms_v2.h"

#define DEFAULT_POWER  false
extern esp_rmaker_device_t *switch_device;
extern esp_rmaker_device_t *temp_sensor_device;
typedef struct {
    esp_rmaker_device_t *device;
    esp_rmaker_param_t *moisture_params[NUM_CHANNELS];
} soil_monitor_t;
//extern soil_monitor_t soil_monitor;

#define ESP_RMAKER_PARAM_HUMIDITY  "esp.param.humidity"
#define ESP_RMAKER_DEVICE_SOIL_MOISTURE "esp.device.soil_moisture"
#define ESP_RMAKER_PARAM_SOIL_MOISTURE "esp.param.soil_moisture"

void app_driver_init(void);
int app_driver_set_state(bool state);
bool app_driver_get_state(void);

esp_err_t temperature_sensor_init(void);
esp_err_t soil_moisture_sensor_init(void);
void add_soil_moisture_params(soil_monitor_t *soil_monitor);
soil_monitor_t* get_soil_monitor();