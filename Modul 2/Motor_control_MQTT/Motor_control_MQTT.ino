#include <WiFi.h>
#include <PubSubClient.h>

#define MOTOR_PIN1 27
#define MOTOR_PIN2 26
#define ENABLE_PIN 12

// ==== WIFI ====
const char* ssid = "vivo";
const char* password = "12341234";

// ==== MQTT ====
const char* mqtt_server = "broker.emqx.io";
const int   mqtt_port   = 1883;
const char* topic_sub   = "esp32/motor/cmd";
const char* topic_pub   = "esp32/motor/status";

WiFiClient espClient;
PubSubClient client(espClient);

int speedValue = 0;

// =========== MQTT CALLBACK ===========
void callback(char* topic, byte* payload, unsigned int length) {
  String cmd;
  for (unsigned int i = 0; i < length; i++) cmd += (char)payload[i];
  cmd.trim();
  Serial.println("CMD: " + cmd);

  if (cmd == "ON") {
    digitalWrite(MOTOR_PIN1, HIGH);
    digitalWrite(MOTOR_PIN2, LOW);
    analogWrite(ENABLE_PIN, speedValue);
    client.publish(topic_pub, "Motor Maju");
  }
  else if (cmd == "OFF") {
    analogWrite(ENABLE_PIN, 0);
    client.publish(topic_pub, "Motor OFF");
  }
  else if (cmd == "REV") {
    digitalWrite(MOTOR_PIN1, LOW);
    digitalWrite(MOTOR_PIN2, HIGH);
    analogWrite(ENABLE_PIN, speedValue);
    client.publish(topic_pub, "Motor Mundur");
  }
  else if (cmd.startsWith("SPEED")) {
    int val = cmd.substring(6).toInt();
    speedValue = constrain(val, 0, 255);
    analogWrite(ENABLE_PIN, speedValue);
    String msg = "Speed=" + String(speedValue);
    client.publish(topic_pub, msg.c_str());
  }
  else {
    client.publish(topic_pub, "Unknown CMD");
  }
}

// =========== MQTT RECONNECT ===========
void reconnect() {
  while (!client.connected()) {
    Serial.print("MQTT...");
    if (client.connect("ESP32MotorClient")) {
      Serial.println("Connected!");
      client.subscribe(topic_sub);
    } else {
      Serial.print("Failed rc=");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(MOTOR_PIN1, OUTPUT);
  pinMode(MOTOR_PIN2, OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);
  analogWrite(ENABLE_PIN, 0);

  // ==== WIFI CONNECT ====
  WiFi.begin(ssid, password);
  Serial.print("WiFi...");
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
  Serial.println("\nWiFi Connected");

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();
}
