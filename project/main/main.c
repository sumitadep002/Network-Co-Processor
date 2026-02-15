#include <stdio.h>
#include "freertos/FreeRTOS.h"

#include "wifi.h"

void app_main(void)
{
    wifi_st_init("Sumit", "123456789");
    while (1)
    {
        printf("Hello-World\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}