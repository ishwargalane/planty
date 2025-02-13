#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"

// Define GPIO pins for multiplexer control
#define MUX_S0_PIN  GPIO_NUM_12
#define MUX_S1_PIN  GPIO_NUM_13
#define MUX_S2_PIN  GPIO_NUM_14
#define MUX_S3_PIN  GPIO_NUM_15

/*---------------------------------------------------------------
    ADC General Macros
---------------------------------------------------------------*/
//ADC1 Channels
#define EXAMPLE_ADC1_CHAN0          ADC_CHANNEL_0
#define EXAMPLE_ADC_ATTEN           ADC_ATTEN_DB_12

#define SAMPLE_COUNT            20      // Number of samples for smoothing
#define NUM_CHANNELS            5       // Number of channels on the multiplexer
#define OUTLIER_THRESHOLD       500     // Define the acceptable deviation threshold
#define CSMSV2_REPORTING_PERIOD 300     // Reporting period in seconds

void set_average_soil_moisture_data(void *pvParameters);
void display_average_soil_moisture_data(void *pvParameters);
// Function to retrive the average soil moisture data stored in the global variable adc_data
const int* get_average_soil_moisture_data();
