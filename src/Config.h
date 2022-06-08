#include <Arduino.h>

const char* WIFI_SSID = "ssid"; ///< WiFi ssid
const char* WIFI_PASS = "pass"; ///< WiFi password
const char* MQTT_SERV = "192.168.0.46"; ///< broker ip
const char* PUB_TOPIC = "/home/button"; ///< input topic to publish
const char* SUB_TOPIC = "/home/lamp"; ///< subscribe topic to control output
const byte DIGITAL_OUTPUT_PIN = 5; ///< pin for output
const byte DIGITAL_INPUT_PIN = 4; ///< pin for input