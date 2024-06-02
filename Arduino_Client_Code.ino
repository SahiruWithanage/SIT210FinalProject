#include <WiFiNINA.h>
#include <WiFiClient.h>
#include <I2S.h>

const char* ssid   = "";
const char* password = "";

const char* server = "192.168.1.6";
const int port = 5000;
const char* path = "/audio";

WiFiClient client;

void setup() {
 Serial.begin(9600);
 while (!Serial);

 if (WiFi.status() == WL_NO_MODULE) {
  Serial.println("Communication with WiFi module failed!");
  while (true);
 }

 while (WiFi.begin(ssid, password) != WL_CONNECTED) {
  delay(5000);
  Serial.print(".");
 }

 Serial.println("Connected to WiFi");

 if (!I2S.begin(I2S_PHILIPS_MODE, 44100, 16)) {
  Serial.println("Failed to initialize I2S!");
  while (1);
 }

 if (!client.connect(server, port)) {
  Serial.println("Connection to server failed!");
  while (1);
 }

 client.print(String("GET ") + path + " HTTP/1.1\r\n" +
       "Host: " + server + "\r\n" + 
       "Connection: close\r\n\r\n");

 while (client.connected()) {
  String line = client.readStringUntil('\n');
  if (line == "\r") {
   break;
  }
 }
}

void loop() {
 if (client.available()) {
  byte audioData[512];
  int bytesRead = client.read(audioData, sizeof(audioData));

  for (int i = 0; i < bytesRead; i += 2) {
   int16_t sample = (audioData[i + 1] << 8) | audioData[i];
   I2S.write(sample);
  }
 }

 if (!client.connected()) {
  Serial.println("Stream ended.");
  while (1);
 }
}
