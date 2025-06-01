#include <SoftwareSerial.h>

SoftwareSerial espSerial(2, 3); // RX, TX

const char* ssid = "WIFISSID"; // Replace with your WiFi SSID
const char* password = "WIFIPassword"; // Replace with your WiFi credentials
const char* mqttServer = "io.adafruit.com";
const char* mqttUser = "ADAFRUIT_IO_USERNAME"; // Replace with your Adafruit IO username
const char* mqttKey = "AdafruitIOKey";
const char* tempTopic = "ADAFRUIT_IO_USERNAME/feeds/temperature";
const char* humTopic = "ADAFRUIT_IO_USERNAME/feeds/humidity";

void sendAT(String cmd, const char* ack, int timeout = 2000) {
  espSerial.println(cmd);
  long int time = millis();
  while ((millis() - time) < timeout) {
    if (espSerial.find((char*)ack)) {
      break;
    }
  }
}

void subscribeToTopic(const char* topic) {
  int topicLen = strlen(topic);
  String packet = "";

  packet += (char)0x82; // SUBSCRIBE packet type
  packet += (char)(topicLen + 5); // Remaining length
  packet += (char)0x00; packet += (char)0x01; // Message ID
  packet += (char)0x00; packet += (char)topicLen;
  packet += topic;
  packet += (char)0x00; // QoS

  sendAT("AT+CIPSEND=" + String(packet.length()), ">");
  espSerial.print(packet);
}

void setup() {
  Serial.begin(9600);
  espSerial.begin(9600);
  delay(2000);

  sendAT("AT", "OK");
  sendAT("AT+CWMODE=1", "OK");
  sendAT("AT+CWJAP=\"" + String(ssid) + "\",\"" + String(password) + "\"", "WIFI CONNECTED", 8000);
  sendAT("AT+CIPMUX=0", "OK");
  sendAT("AT+CIPSTART=\"TCP\",\"" + String(mqttServer) + "\",1883", "CONNECT", 5000);

  // MQTT CONNECT packet
  String packet = "";
  packet += (char)0x10; // CONNECT
  int rl = 14 + strlen(mqttUser) + strlen(mqttKey);
  packet += (char)rl;
  packet += (char)0x00; packet += (char)0x04; packet += "MQTT";
  packet += (char)0x04;
  packet += (char)0xC2;
  packet += (char)0x00; packet += (char)0x3C;
  packet += (char)0x00; packet += (char)0x0B; packet += "arduino_sub";
  packet += (char)0x00; packet += (char)strlen(mqttUser); packet += mqttUser;
  packet += (char)0x00; packet += (char)strlen(mqttKey); packet += mqttKey;

  sendAT("AT+CIPSEND=" + String(packet.length()), ">");
  espSerial.print(packet);

  delay(2000);
  subscribeToTopic(tempTopic);
  subscribeToTopic(humTopic);
  Serial.println("Subscribed to topics");
}

void loop() {
  if (espSerial.available()) {
    String line = espSerial.readStringUntil('\n');
    line.trim();

    if (line.startsWith("+IPD,")) {
      int colonIndex = line.indexOf(':');
      if (colonIndex != -1) {
        int payloadLength = line.substring(5, colonIndex).toInt();

        while (espSerial.available() < payloadLength) {
          delay(10);
        }

        String payload = "";
        for (int i = 0; i < payloadLength; i++) {
          payload += (char)espSerial.read();
        }

        Serial.print("MQTT Payload: ");
        Serial.println(payload);
      }
    }
  }
}
