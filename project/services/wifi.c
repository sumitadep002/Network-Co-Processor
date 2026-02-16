#include <stdbool.h>
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "esp_wifi.h"
#include "lwip/dns.h"

// LOG-MACROS
#define LOG_WIFI "WIFI"

static char *wifi_ap_ssid = "Sumit";
static char *wifi_ap_pswd = "123456789";

static bool gf_wifi_status = 0;

static void event_handler(void *arg, esp_event_base_t event_base,
                          int32_t event_id, void *event_data);
static void print_network_info(void);

void wifi_st_init(char *ssid, char *pswd)
{
    wifi_config_t wifi_config = {0};

    ESP_ERROR_CHECK(nvs_flash_init());

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL));

    if (ssid != NULL)
    {
        strncpy((char *)wifi_config.sta.ssid, ssid, sizeof(wifi_config.sta.ssid));
    }

    if (pswd != NULL)
    {
        strncpy((char *)wifi_config.sta.password, pswd, sizeof(wifi_config.sta.password));
    }

    wifi_config.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(LOG_WIFI, "...DONE");
}

void event_handler(void *arg, esp_event_base_t event_base,
                   int32_t event_id, void *event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
    {
        esp_wifi_connect();
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        gf_wifi_status = false;
        ESP_LOGI(LOG_WIFI, "Disconnected. Reconnecting...");
        esp_wifi_connect();
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_CONNECTED)
    {
        gf_wifi_status = true;
        ESP_LOGI(LOG_WIFI, "Connected: ");
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        print_network_info();
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_ASSIGNED_IP_TO_CLIENT)
    {
        ip_event_assigned_ip_to_client_t *ev = (ip_event_assigned_ip_to_client_t *)event_data;
        ESP_LOGI(LOG_WIFI, "Client got IP: " IPSTR, IP2STR(&ev->ip));
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_AP_STADISCONNECTED)
    {
        wifi_event_ap_stadisconnected_t *event = (wifi_event_ap_stadisconnected_t *)event_data;

        ESP_LOGI(LOG_WIFI, "Client disconnected: MAC=%02x:%02x:%02x:%02x:%02x:%02x, AID=%d",
                 event->mac[0], event->mac[1], event->mac[2],
                 event->mac[3], event->mac[4], event->mac[5],
                 event->aid);
    }
}

void print_network_info(void)
{
    esp_netif_ip_info_t ip_info;
    esp_netif_get_ip_info(esp_netif_get_handle_from_ifkey("WIFI_STA_DEF"), &ip_info);

    ESP_LOGI(LOG_WIFI, "================== Network Info ==================");
    ESP_LOGI(LOG_WIFI, "IP Address:    " IPSTR, IP2STR(&ip_info.ip));
    ESP_LOGI(LOG_WIFI, "Subnet Mask:   " IPSTR, IP2STR(&ip_info.netmask));
    ESP_LOGI(LOG_WIFI, "Gateway:       " IPSTR, IP2STR(&ip_info.gw));

    const ip_addr_t *dns_server = dns_getserver(0);
    ESP_LOGI(LOG_WIFI, "Primary DNS:   " IPSTR, IP2STR(&dns_server->u_addr.ip4));

    dns_server = dns_getserver(1);
    ESP_LOGI(LOG_WIFI, "Secondary DNS: " IPSTR, IP2STR(&dns_server->u_addr.ip4));
    ESP_LOGI(LOG_WIFI, "=================================================");
}

bool wifi_get_status()
{
    return gf_wifi_status;
}