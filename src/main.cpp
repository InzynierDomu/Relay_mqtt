#include "Config.h"

#include <Arduino.h>
#include <PubSubClient.h>

#ifdef ESP8266
#include <ESP8266WiFi.h>
WiFiClient wifiClient;
#endif
#ifdef WIFININA
#include <SPI.h>
#include <WiFiNINA.h>
WiFiClient wifiClient;
#endif

PubSubClient client(wifiClient); ///< mqtt client

/**
 * @brief Setup wifi
 */
void setup_wifi()
{
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

/**
 * @brief reaction for recive mqtt message
 * @param topic: topic of message
 * @param payload: paylod of message
 * @param length: lenght pyload
 */
void callback(char* topic, byte* payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  Serial.print((char)*payload);
  Serial.println();

  if ((char)*payload == '1')
  {
    digitalWrite(DIGITAL_OUTPUT_PIN, HIGH);
    Serial.print("LOW");
  }
  else if ((char)*payload == '0')
  {
    digitalWrite(DIGITAL_OUTPUT_PIN, LOW);
    Serial.print("HIGH");
  }
}

/**
 * @brief setup
 */
void setup()
{
  pinMode(DIGITAL_OUTPUT_PIN, OUTPUT);
  pinMode(DIGITAL_INPUT_PIN, INPUT_PULLUP);
  digitalWrite(DIGITAL_OUTPUT_PIN, LOW);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(MQTT_SERV, 1883);
  client.setCallback(callback);
}

/**
 * @brief reconnect to MQTT server
 */
void reconnect()
{
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("client"))
    {
      Serial.println("connected");
      client.subscribe(SUB_TOPIC);
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

/**
 * @brief main loop
 */
void loop()
{
  static bool btn_las_state;
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();
  if ((digitalRead(DIGITAL_INPUT_PIN) != 1) && btn_las_state)
  {
    Serial.print("Publish message");
    char* msg = "on";
    client.publish(PUB_TOPIC, msg);
    btn_las_state = HIGH;
  }
  else if ((digitalRead(DIGITAL_INPUT_PIN) == 1) && btn_las_state)
  {
    Serial.print("Publish message");
    char* msg = "off";
    client.publish(PUB_TOPIC, msg);
    btn_las_state = LOW;
  }
  delay(10);
}