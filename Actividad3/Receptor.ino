#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <RF24.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

const char* ssid = "TIGO-FC99";
const char* password = "2NB144203115";
const char* server = "http://telemsebas-0a07559143b3.herokuapp.com/subirdatos.php";

RF24 radio(2, 4);  // Utiliza los pines D2 y D4 para CE y CSN respectivamente
DHT dht(5, DHT11);

struct DataPacket {
  float temperature;
  float humidity;
};

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
  dht.begin();
}

void loop() {
  if (radio.available()) {
    DataPacket dataPacket;
    radio.read(&dataPacket, sizeof(dataPacket));

    Serial.print("Temperatura recibida: ");
    Serial.println(dataPacket.temperature);
    Serial.print("Humedad recibida: ");
    Serial.println(dataPacket.humidity);

    // Realizar la solicitud HTTP POST al servidor
    HTTPClient http;

    String url = String(server);
    http.begin(url);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    // Ajusta el nombre del campo y la asignación según tu base de datos
    String postBody = "&temperature=" + String(dataPacket.temperature) + "&humidity=" + String(dataPacket.humidity); 

    int httpCode = http.POST(postBody);

    if (httpCode > 0) {
      String payload = http.getString();
      Serial.println("Server Answer:");
      Serial.println(payload);
    } else {
      Serial.printf("Error en la solicitud HTTP %d\n", httpCode);
    }

    http.end();
  }
}