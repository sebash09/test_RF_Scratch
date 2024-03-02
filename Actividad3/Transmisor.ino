#include <SPI.h>
#include <RF24.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

RF24 radio(9, 10); // CE, CSN
DHT dht(7, DHT11);

struct DataPacket {
  float temperature;
  float humidity;
};

void setup() {
  Serial.begin(115200);
  radio.begin();
  radio.openWritingPipe(0xF0F0F0F0E1LL); // Dirección del canal de comunicación
  dht.begin();
}

void loop() {
  DataPacket dataPacket;
  dataPacket.temperature = dht.readTemperature();
  dataPacket.humidity = dht.readHumidity();

  Serial.print("Temperatura: ");
  Serial.println(dataPacket.temperature);
  Serial.print("Humedad: ");
  Serial.println(dataPacket.humidity);

  radio.write(&dataPacket, sizeof(dataPacket)); // Envía el paquete de datos al receptor
  delay(1000); // Espera 1 segundo antes de enviar el siguiente paquete
}