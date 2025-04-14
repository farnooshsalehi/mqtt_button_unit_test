#ifndef MQTT_BUTTON_EXAMPLE_H
#define MQTT_BUTTON_EXAMPLE_H

extern volatile bool button_event;
extern volatile uint32_t last_interrupt_time;
extern uint32_t press_count;
extern esp_mqtt_client_handle_t mqtt_client;

void mqtt_button_app_main(void);

#endif