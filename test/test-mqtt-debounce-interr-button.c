#include <unity.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "mqtt_button_example.h"

#define TIMEOUT_MS 5000

static const char *TAG = "mqtt_button_test";

// Test 1: Verify button press increments counter and triggers MQTT publish
void test_button_press(void) {
    ESP_LOGI(TAG, "Please press the button once within %d ms", TIMEOUT_MS);

    // Store initial press count
    uint32_t initial_count = press_count;

    // Wait for button press (button_event set by ISR)
    TickType_t start_time = xTaskGetTickCount();
    while (!button_event && 
           ((xTaskGetTickCount() - start_time) * portTICK_PERIOD_MS < TIMEOUT_MS)) {
        vTaskDelay(pdMS_TO_TICKS(10));
    }

    // Check if button press was detected
    TEST_ASSERT_TRUE_MESSAGE(button_event, "Button press not detected");

    // Wait for debouncing and MQTT publish
    vTaskDelay(pdMS_TO_TICKS(100));

    // Verify press count incremented
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(initial_count + 1, press_count, 
                                    "Button press count did not increment");

    // Reminder to check MQTTX
    ESP_LOGI(TAG, "Check MQTTX to confirm 'Button pressed! Count: %lu' was received on /topic/button", 
             press_count);
}



// Test 2: Verify debouncing (rapid presses count as one)
void test_button_debounce(void) {
    ESP_LOGI(TAG, "Please press the button rapidly 3-5 times within %d ms", TIMEOUT_MS);

    // Store initial press count
    uint32_t initial_count = press_count;

    // Wait for multiple presses
    vTaskDelay(pdMS_TO_TICKS(TIMEOUT_MS));

    // Verify only one increment due to debouncing
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(initial_count + 1, press_count,
                                    "Debouncing failed: multiple increments detected");

    // Reminder to check MQTTX
    ESP_LOGI(TAG, "Check MQTTX to confirm only one message was published");
}



// Main test function
void app_main(void) {
    // Run the original application
    mqtt_button_app_main(); // Renamed original app_main

    // Wait for MQTT connection
    ESP_LOGI(TAG, "Waiting for MQTT connection...");
    vTaskDelay(pdMS_TO_TICKS(5000));

    // Execute tests
    UNITY_BEGIN();
    RUN_TEST(test_button_press);
    RUN_TEST(test_button_debounce);
    UNITY_END();

    // Keep running for manual testing
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}