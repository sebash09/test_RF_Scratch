// Definir el pin al que está conectado el sensor LM35
const int lm35Pin = A0;

void setup() {
  // Inicializar comunicación serial a 9600 baudios
  Serial.begin(9600);
}

void loop() {
  // Leer el valor analógico del sensor LM35
  int sensorValue = analogRead(lm35Pin);

  // Convertir el valor analógico a temperatura en grados Celsius
  float temperatureC = (sensorValue * 5.0) / 1024.0 * 100.0;

  // Mostrar la temperatura por consola
  Serial.print("Temperatura: ");
  Serial.print(temperatureC);
  Serial.println(" °C");

  // Esperar un breve periodo de tiempo antes de volver a leer el sensor
  delay(1000);
