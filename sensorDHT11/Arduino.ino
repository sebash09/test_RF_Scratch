#include <DHT.h>
#include <RF24.h>

#define DHTPIN 7      // Pin al que está conectado el sensor DHT11
#define DHTTYPE DHT11 // Tipo de sensor DHT (DHT11 en este caso)

DHT dht(DHTPIN, DHTTYPE);
RF24 radio(9, 10); // Pines CE, CSN

struct SensorData {
  float temperature;
  float humidity;
};

void setup() {
  Serial.begin(115200);
  Serial.println("Iniciando sensor DHT11 y radio NRF24...");

  dht.begin();

  if (!radio.begin()) {
    Serial.println("Error al iniciar la radio NRF24");
    while (1);
  }

  radio.openWritingPipe(0xF0F0F0F0E1LL); // Dirección del canal de comunicación
}

void loop() {
  delay(2000); // Espera 2 segundos entre lecturas

  float humidity = dht.readHumidity();    // Lee la humedad
  float temperature = dht.readTemperature(); // Lee la temperatura en grados Celsius

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Error al leer el sensor DHT11");
    return;
  }

  Serial.print("Humedad: ");
  Serial.print(humidity);
  Serial.print("%\t");
  Serial.print("Temperatura: ");
  Serial.print(temperature);
  Serial.println("°C");

  SensorData data;
  data.temperature = temperature;
  data.humidity = humidity;

  radio.write(&data, sizeof(data)); // Envía los datos al receptor
}
