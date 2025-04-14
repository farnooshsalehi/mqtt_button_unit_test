## What the Tests Do? 
The unit tests are in test/test_mqtt_button.c . They verify the main code by checking:
1. **Button Press Test ("test_button_press" function):**
   * Checks if pressing the button once:
     * Increases the press count (press_count) by 1.
     * Triggers a message to the MQTT topic.

   * You press the button, and the test confirms the count changed. You also check the message in MQTTX (a tool like an MQTT message viewer).

2. **Debounce Test ("test_button_debounce" function):**
   * Checks if pressing the button rapidly (3–5 times quickly) only counts as one press, thanks to a 50ms delay (debouncing) in the main code.
   * You press the button fast, and the test ensures the count only increases once and only one MQTT message is sent.


## How the Tests Work?
* Accessing Main Code:
  * The main code has variables like press_count (tracks button presses) and button_event (signals a button press). These are shared with the tests via a header file (mqtt_button_example.h).

  * The tests read these variables to check if the main code behaves correctly when you press the button.

* Running the Main Code:
  * The tests start the main code to set up the button, Wi-Fi, and MQTT connection before testing.
  * The tests then wait for you to press the button and check the results (e.g., did press_count increase?).

* Checking MQTT:
  * The tests don’t directly read MQTT messages (to keep things simple). Instead, they ask you to use MQTTX to confirm messages like "Button pressed! Count: 1" appear on /topic/button.


## Why Rename app_main?
* The test code needs its own app_main to start tests.
