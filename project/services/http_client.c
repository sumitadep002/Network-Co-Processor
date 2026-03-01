#include <stdbool.h>
#include <stddef.h>

#include "esp_log.h"
#include "esp_http_client.h"
#include "http_client.h"

#include "wifi.h"
#include "util.h"

#define PING_INTERVAL 5000
#define PING_URL "http://www.google.com"
#define WIFI_PING "PING"

static bool gf_internet_status = false;

static void http_client_ping_task(void *);

void http_ping_init()
{
    xTaskCreate(http_client_ping_task, "http_ping_task", 2048 + 1024, NULL, 5, NULL);
}

void http_client_ping_task(void *pvParameters)
{

    while (1)
    {
        if (wifi_get_status() == true)
        {
            esp_http_client_config_t config = {
                .url = PING_URL,
            };

            esp_http_client_handle_t client = esp_http_client_init(&config);

            esp_err_t err = esp_http_client_perform(client);

            if (err != ESP_OK)
            {
                ESP_LOGE(WIFI_PING, "NO-INTERNET");
                gf_internet_status = false;
            }
            else
            {
                gf_internet_status = true;
                ESP_LOGI(WIFI_PING, "OK");
            }

            esp_http_client_cleanup(client);
        }
        DELAY_MS(PING_INTERVAL);
    }
}

bool http_get_ping_status()
{
    return gf_internet_status;
}