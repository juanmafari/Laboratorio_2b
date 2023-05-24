/*  WiFi softAP Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <string.h>
#include "esp_wifi.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include <esp_http_server.h>


/* SOFTAP SETEO COSO
*/
#define EXAMPLE_ESP_WIFI_SSID      "esp32-s2"
#define EXAMPLE_ESP_WIFI_PASS      "unodostres"


static const char *TAG = "wifi softAP";







void wifi_init_softap(void)
{
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_ap();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    wifi_config_t wifi_config = {
        .ap = {
            .ssid = EXAMPLE_ESP_WIFI_SSID,
            .ssid_len = strlen(EXAMPLE_ESP_WIFI_SSID),
            .channel = 3,
            .password = EXAMPLE_ESP_WIFI_PASS,
            .max_connection = 2,
            .authmode = WIFI_AUTH_WPA_WPA2_PSK,
            },
        };

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    //ESP_LOGI(TAG, "wifi_init_softap finished. SSID:%s password:%s channel:%d",
             //EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS, EXAMPLE_ESP_WIFI_CHANNEL);
}



esp_err_t get_handler(httpd_req_t *req)
{
    extern const uint8_t index_html_start[] asm("_binary_index_html_start");
    extern const uint8_t index_html_end[] asm("_binary_index_html_end");
    
    size_t index_len = index_html_end - index_html_start;
    char indexHtml[index_len];
    memcpy(indexHtml,index_html_start,index_len);
    httpd_resp_send(req, (const char *)index_html_start, index_len);
    return ESP_OK;
}

httpd_uri_t uri_get = {
    .uri      = "/",
    .method   = HTTP_GET,
    .handler  = get_handler,
};

/* URI handler structure for POST /uri */


httpd_handle_t start_web_server() {
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.server_port = 80;
    esp_err_t ret = httpd_start(&server, &config);
    httpd_register_uri_handler(server, &uri_get);
    
    return server;
}

void app_main(void)
{
    esp_err_t ret = nvs_flash_init();
    ESP_LOGI(TAG, "ESP_WIFI_MODE_AP");
    wifi_init_softap();
    start_web_server();

    /*esp_err_t ret = nvs_flash_init();
    ESP_LOGI(TAG, "ESP_WIFI_MODE_STA");
    wifi_init_sta();*/
}