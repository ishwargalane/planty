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

#define DEFAULT_POWER  false
extern esp_rmaker_device_t *switch_device;
extern esp_rmaker_device_t *temp_sensor_device;
extern esp_rmaker_device_t *humidity_sensor_device;


void app_driver_init(void);
int app_driver_set_state(bool state);
bool app_driver_get_state(void);

esp_err_t temperature_sensor_init(void);
esp_err_t humidity_sensor_init(void);
