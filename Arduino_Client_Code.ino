#include <WiFi.h>
#include <ArduinoHttpClient.h>
#include <I2S.h>

const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";
const char* streamUrl = "http://your_raspberry_pi_ip:8000/stream";

const int bufferSize = 8192;  // Buffer size for audio data
int16_t audioBuffer[bufferSize / 2];  // Buffer for audio samples

WiFiClient wifi;
HttpClient http(wifi, "your_raspberry_pi_ip", 8000);

// Ultrasonic sensor pins
const int trigPinL = 12;
const int echoPinL = 14;
const int trigPinR = 27;
const int echoPinR = 26;

// Speaker volume levels
float volumeL = 1.0;
float volumeR = 1.0;

void setup() {
  Serial.begin(115200);
  pinMode(trigPinL, OUTPUT);
  pinMode(echoPinL, INPUT);
  pinMode(trigPinR, OUTPUT);
  pinMode(echoPinR, INPUT);
  connectToWiFi();
  initializeI2S();
  connectToStream();
}

void loop() {
  readDistances();
  adjustVolumes();
  if (http.connected() && http.available()) {
    size_t bytesRead = http.readBytes(reinterpret_cast<char*>(audioBuffer), bufferSize);
    playAudioBuffer(bytesRead);
  }
}

void connectToWiFi() {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
}

void initializeI2S() {
  if (!I2S.begin(I2S_PHILIPS_MODE, 44100, 16)) {
    Serial.println("Failed to initialize I2S!");
    while (1); // Halt if I2S initialization fails
  }
  Serial.println("I2S initialized");
}

void connectToStream() {
  http.get("/stream");
  int statusCode = http.responseStatusCode();

  if (statusCode != 200) {
    Serial.print("Failed to connect to stream. Status code: ");
    Serial.println(statusCode);
    http.stop();
    return;
  }

  Serial.println("Connected to stream");
}

void playAudioBuffer(size_t bytesRead) {
  if (bytesRead > 0) {
    Serial.print("Fetched bytes: ");
    Serial.println(bytesRead);

    for (size_t i = 0; i < bytesRead / 2; i++) {
      int16_t sample = audioBuffer[i];
      int16_t sampleL = sample * volumeL;
      int16_t sampleR = sample * volumeR;
      I2S.write(sampleL);
      I2S.write(sampleR);
    }
  }
}

void readDistances() {
  long durationL, durationR;
  digitalWrite(trigPinL, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinL, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinL, LOW);
  durationL = pulseIn(echoPinL, HIGH);

  digitalWrite(trigPinR, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinR, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinR, LOW);
  durationR = pulseIn(echoPinR, HIGH);

  float distanceL = durationL * 0.034 / 2;
  float distanceR = durationR * 0.034 / 2;

  Serial.print("Distance L: ");
  Serial.print(distanceL);
  Serial.print(" cm, Distance R: ");
  Serial.print(distanceR);
  Serial.println(" cm");

  // Adjust volume levels based on distance
  volumeL = mapDistanceToVolume(distanceL);
  volumeR = mapDistanceToVolume(distanceR);
}

float mapDistanceToVolume(float distance) {
  float volume = 1.0 - (distance / 100.0); // Example mapping: decrease volume as distance decreases
  return constrain(volume, 0.0, 1.0); // Ensure volume is within 0 to 1 range
}

void adjustVolumes() {
  // Ensure volume values are within range
  volumeL = constrain(volumeL, 0.0, 1.0);
  volumeR = constrain(volumeR, 0.0, 1.0);

  // Inverse volume adjustment for opposite speaker
  volumeL = 1.0 - volumeR;
  volumeR = 1.0 - volumeL;
}
