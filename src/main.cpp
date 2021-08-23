#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "credentials.h"
#include <PubSubClient.h>

#define LED 2            // Led in NodeMCU at pin GPIO16 (D0). gpio2 ESP8266 led

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length) {

  Serial.print("Message arrived in topic: ");
  Serial.println(topic);

  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }

  Serial.println();
  Serial.println("-----------------------");

}

void setup() {
pinMode(LED, OUTPUT);    // LED pin as output.

Serial.begin(115200);
Serial.println();

WiFi.begin(mySSID, myPASSWORD);

Serial.print("Connecting");
while (WiFi.status() != WL_CONNECTED)
{
  delay(500);
  Serial.print(".");
}
Serial.println();

Serial.print("Connected, IP address: ");
Serial.println(WiFi.localIP());

client.setServer(mqttServer, mqttPort);
client.setCallback(callback);

while (!client.connected()) {
    Serial.println("Connecting to MQTT...");

    if (client.connect("ESP8266Client")) {

      Serial.println("connected");

    } else {

      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);

    }
  }

  client.publish("esp/test", "Hello from ESP8266");
  client.subscribe("esp/test");
}

void loop() {
digitalWrite(LED, HIGH);// turn the LED off.(Note that LOW is the voltage level but actually
                        //the LED is on; this is because it is acive low on the ESP8266.
delay(1000);            // wait for 1 second.
digitalWrite(LED, LOW); // turn the LED on.
delay(1000); // wait for 1 second.
client.publish("esp/test", "Hello from ESP8266");
}
