#include <ESP8266WiFi.h>
#include <RF24.h>

const char* ssid = "dddd";
const char* password = "data";

RF24 radio(2, 4); // Pines D2 y D4 para CE y CSN respectivamente

struct SensorData {
  float temperature;
  float humidity;
};

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }
  Serial.println("Conectado a WiFi");

  if (!radio.begin()) {
    Serial.println("Error al iniciar la radio NRF24");
    while (1);
  }

  radio.openReadingPipe(1, 0xF0F0F0F0E1LL); // Dirección del canal de comunicación
  radio.startListening();
}

void loop() {
  if (radio.available()) {
    SensorData data;
    radio.read(&data, sizeof(data));

    Serial.print("Temperatura recibida: ");
    Serial.print(data.temperature);
    Serial.print("°C\t");

    Serial.print("Humedad recibida: ");
    Serial.print(data.humidity);
    Serial.println("%");
  }
}