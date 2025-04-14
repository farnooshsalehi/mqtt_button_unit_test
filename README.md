# MQTT Button Example with Hardware Unit Tests
This project demonstrates an ESP32 application that uses a built-in button to publish messages to an MQTT broker when pressed. The button press is debounced, and a counter increments with each press, publishing a message to the topic /topic/button (e.g., "Button pressed! Count: 1"). 


The project includes unit tests that verify : 
* The button's functionality.
* Debouncing.
* Interrupt eny edge while button triggered.
* MQTT publishing using the actual hardware.

## Key important points for this project: 
* **Unit Tests on ESP32** : Designed to run directly on the ESP32, using its built-in button and a real MQTT broker for integration testing.

* **Unity Framework** : Utilizes the Unity test framework within ESP-IDF for reliable and structured test execution.

* **README Guidance** : Provides a detailed, step-by-step guide in README.md for cloning, setting up, and running the unit tests.

# Prerequisites
To set up and run the project and its unit tests, ensure you have the following:

*  ## Hardware
   * **ESP32 Development Board:** Any ESP32 module with a built-in button connected to GPIO 0 (e.g., ESP32-DevKitC). The button should be active-low with an internal pull-up resistor enabled by the code.

   * **USB Cable**
   * **Wi-Fi Network**

*  ## Software
   * **ESP-IDF:** Version 5.0 or later.
   * **Python:** Version 3.6 or later
   * **Git**
   * **MQTTX**

 


# Project Structure

---

- mqtt_button_example/

  - main/
    - mqtt_button_example.c 
    - mqtt_button_example.h 
    - CMakeLists.txt 

  - test/
    - test_mqtt_button.c
    - CMakeLists.txt

  - CMakeLists.txt
  - sdkconfig.defaults 
  - README.md 

---

*  **Main Application:** Connects to Wi-Fi and an MQTT broker, monitors the built-in button on GPIO 0, debounces presses, and publishes messages to /topic/button.
*  **Unit Tests:** Verify bellow using the Unity framework : 
   *  button detection
   *  debouncing
   *  counter increment
   *  MQTT publishing
  

# Cloning the Project
1. **Clone the Repository:** 

2. **Verify Files:** Ensure the project structure matches the one above. Key files include :
   * **main/mqtt_button_example.c:** The application logic.
   * **test/test_mqtt_button.c:** The unit tests.
   * **sdkconfig.defaults:** Default settings for Wi-Fi and MQTT.
  




# Get start: 
1. ## Setup MQTTX : 
* Install MQTTX from mqttx.app.
* Open MQTTX and create a new connection:
  * Host: broker.hivemq.com
  * Port: 1883
  * Protocol: mqtt
  * Leave username/password blank unless your broker requires them.
* Connect to the broker.
* Subscribe to the topic /topic/button to monitor messages.

2. ## Connect the ESP32 : 
3. ## Configuring the Project : 
* Set Wi-Fi and MQTT Settings:
  * Run the configuration tool.
  * Navigate to Example Connection Configuration:
    * WiFi SSID: Enter your Wi-Fi network name (e.g., MyWiFi).
    * WiFi Password: Enter your Wi-Fi password.
    * Broker URL: Set to your MQTT broker : mqtt://broker.hivemq.com 
  
4. flash and monitor. 

    