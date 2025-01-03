#include <dht.h>

#if defined(CONFIG_DHT_TYPE_DHT11)
#define SENSOR_TYPE DHT_TYPE_DHT11
#endif
#if defined(CONFIG_DHT_TYPE_AM2301)
#define SENSOR_TYPE DHT_TYPE_AM2301
#endif
#if defined(CONFIG_DHT_TYPE_SI7021)
#define SENSOR_TYPE DHT_TYPE_SI7021
#endif

#define DEFAULT_TEMPERATURE         25.0
#define DEFAULT_HUMIDITY            50.0


void set_average_temperature_humidity(void *pvParameters);