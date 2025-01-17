#include <ESP8266MQTTClient.h>
#include <ArduinoLog.h>

#define DEBUG_SERIAL  Serial

MQTTClient mqtt;

void setup() {
  DEBUG_SERIAL.begin(115200);
  //  WiFi.begin("ssid", "pass");
  Log.begin(LOG_LEVEL_VERBOSE, &DEBUG_SERIAL);


  //topic, data, data is continuing
  mqtt.onData([](String topic, String data, bool cont) {
    Log.notice("Data received, topic: %s, data: %s\r\n", topic.c_str(), data.c_str());
    mqtt.unSubscribe("/qos0");
  });

  mqtt.onSubscribe([](int sub_id) {
    Log.notice("Subscribe topic id: %d ok\r\n", sub_id);
    mqtt.publish("/qos0", "qos0", 0, 0);
  });
  mqtt.onConnect([]() {
    Log.notice("MQTT: Connected\r\n");
    Log.notice("Subscribe id: %d\r\n", mqtt.subscribe("/qos0", 0));
    mqtt.subscribe("/qos1", 1);
    mqtt.subscribe("/qos2", 2);
  });

  mqtt.begin("ws://broker.mqttdashboard.com:8000/mqtt");
  //mqtt.begin("ws://test.mosquitto.org:8080", {.lwtTopic = "hello", .lwtMsg = "offline", .lwtQos = 0, .lwtRetain = 0});
  //mqtt.begin("ws://user:pass@mosquito.org:8080");
  //mqtt.begin("ws://user:pass@mosquito.org:8080#clientId");

}

void loop() {
  mqtt.handle();
}
