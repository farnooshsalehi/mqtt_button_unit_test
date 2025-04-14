#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "protocol_examples_common.h"
#include "esp_log.h"
#include "mqtt_client.h"

#define BUTTON_PIN 0
#define DEBOUNCE_TIME_MS 50

static const char *TAG = "mqtt_button_example";
static volatile uint32_t last_interrupt_time = 0;
static volatile bool button_event = false;
static uint32_t press_count = 0;
static esp_mqtt_client_handle_t mqtt_client; // Global MQTT client handle

// ISR for button press
static void IRAM_ATTR button_isr_handler(void* arg) {
    last_interrupt_time = xTaskGetTickCountFromISR();
    button_event = true;
}

// MQTT event handler
static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
    esp_mqtt_event_handle_t event = event_data;
    switch (event->event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "Connected to MQTT broker!");
            break;
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "Disconnected from MQTT broker.");
            break;
        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(TAG, "Message published successfully!");
            break;
        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "Received data: Topic=%.*s, Data=%.*s",
                     event->topic_len, event->topic, event->data_len, event->data);
            break;
        default:
            break;
    }
}

// Button task with MQTT publish
static void button_task(void* arg) {
    bool last_state = true; // Button not pressed (HIGH with pull-up)

    while (1) {
        if (button_event) {
            uint32_t current_time = xTaskGetTickCount();
            if ((current_time - last_interrupt_time) * portTICK_PERIOD_MS >= DEBOUNCE_TIME_MS) {
                bool current_state = gpio_get_level(BUTTON_PIN);
                if (current_state == false && last_state == true) { // Button pressed
                    press_count++;
                    ESP_LOGI(TAG, "Button pressed - count: %lu", press_count);
                    // Publish MQTT message
                    char msg[50];
                    snprintf(msg, sizeof(msg), "Button pressed! Count: %lu", press_count);
                    esp_mqtt_client_publish(mqtt_client, "/topic/button", msg, 0, 1, 0);
                }
                last_state = current_state;
                button_event = false;
            }
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void app_main(void) {
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Initialize network
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    ESP_ERROR_CHECK(example_connect()); // Wi-Fi from menuconfig

    // Configure GPIO for button
    gpio_config_t button_config = {
        .pin_bit_mask = (1ULL << BUTTON_PIN),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_ANYEDGE
    };
    gpio_config(&button_config);
    gpio_install_isr_service(0);
    gpio_isr_handler_add(BUTTON_PIN, button_isr_handler, NULL);

    // Start MQTT
    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = CONFIG_BROKER_URL,
    };
    mqtt_client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(mqtt_client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(mqtt_client);

    // Create button task
    xTaskCreate(button_task, "button_task", 2048, NULL, 10, NULL);

    ESP_LOGI(TAG, "Setup complete. Waiting for button press...");
}