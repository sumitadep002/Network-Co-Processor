#include "nvs_flash.h"
#include "esp_log.h"

// LOG-MACROS
#define LOG_WIFI "WIFI"

static char *wifi_ap_ssid = "Sumit";
static char *wifi_ap_pswd = "123456789";

void wifi_st_init(char *ssid, char *pswd)
{
    if (ssid != nullptr && pswd != nullptr)
    {
        wifi_ap_ssid = ssid;
        wifi_ap_pswd = pswd;
    }

    // Attempting to connect with
    ESP_LOGI
}