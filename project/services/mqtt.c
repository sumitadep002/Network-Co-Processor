#include <stdbool.h>

#include "mqtt_client.h"
#include "esp_tls.h"
#include "esp_log.h"
#include "esp_crt_bundle.h"

#define MQTT "MQTT"

static esp_mqtt_client_handle_t client = NULL;

static void mqtt_event_handler(void *event_handler_arg,
                               esp_event_base_t event_base,
                               int32_t event_id,
                               void *event_data);

bool mqtt_init()
{
    // Configure Broker
    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = "mqtts://broker.hivemq.com",
        .broker.verification.crt_bundle_attach = esp_crt_bundle_attach,
    };

    client = esp_mqtt_client_init(&mqtt_cfg);

    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);

    esp_mqtt_client_start(client);

    return true;
}

void mqtt_event_handler(void *event_handler_arg,
                        esp_event_base_t event_base,
                        int32_t event_id,
                        void *event_data)
{
    switch (event_id)
    {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(MQTT, "MQTT Connected");
        esp_mqtt_event_handle_t event = (esp_mqtt_event_handle_t)event_data;
        esp_mqtt_client_handle_t client = event->client;
        esp_mqtt_client_publish(client, "sumit/esp/test", "Hello-World", 0, 1, 0);
        break;

    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(MQTT, "MQTT Disconnected");
        break;

    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI(MQTT, "MQTT Subscribed");
        break;

    case MQTT_EVENT_PUBLISHED:
        ESP_LOGI(MQTT, "MQTT Published");
        break;

    case MQTT_EVENT_DATA:
        ESP_LOGI(MQTT, "MQTT Data Received");
        break;

    case MQTT_EVENT_ERROR:
        ESP_LOGI(MQTT, "MQTT Error");
        break;

    default:
        ESP_LOGI(MQTT, "Other MQTT Event: %ld", event_id);
        break;
    }
}