idf_component_register(SRCS "mqtt-debounce-interr-button.c"
                    INCLUDE_DIRS "."
                    REQUIRES mqtt esp_event nvs_flash protocol_examples_common driver esp_netif)
