#include "DHT.h"

//Configuración Sensor Humedad de Tierra (Analógico)
const int sensorPin = A0;
const int valorSeco = 870;
const int valorHumedo = 425;

//Configuración Sensor DHT11 (Digital)
#define DHTPIN 2
#define DHTTYPE DHT11

//Inicialización de objetos
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  int valorHumedad = analogRead(sensorPin);

  // Mapea el valor analógico a un porcentaje (0-100%)
  int porcentajeHumedadTierra = map(valorHumedad, valorSeco, valorHumedo, 0, 100);

  // Limita el valor para que esté siempre entre 0 y 100
  porcentajeHumedadTierra = constrain(porcentajeHumedadTierra, 0, 100);

  float h = dht.readHumidity();    // Lee la humedad ambiente
  float t = dht.readTemperature(); // Lee la temperatura ambiente

  if (isnan(h) || isnan(t)) {
    Serial.println("Error leyendo el sensor DHT11");
    // Espera un segundo antes de reintentar
    delay(1000); 
    return; // Sale de esta vuelta del loop y empieza de nuevo
  }

  // Se envían los datos en formato CSV (Valores Separados por Comas)
  Serial.print(porcentajeHumedadTierra);
  Serial.print(",");
  Serial.print(t);
  Serial.print(",");
  Serial.println(h);

  // --- 5. Delay (Espera) ---
  delay(1000);
}
