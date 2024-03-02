#include <ESP8266WiFi.h>
#include <RF24.h>

const char* ssid = "TIGO-FC99";
const char* password = "2NB144203115";

RF24 radio(2, 4); // Utiliza los pines D2 y D4 para CE y CSN respectivamente

void setup() {
  Serial.begin(115200);
  
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
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
    char mensaje[32] = "";
    radio.read(mensaje, sizeof(mensaje));
    Serial.println("Mensaje recibido: " + String(mensaje));
  }
}
