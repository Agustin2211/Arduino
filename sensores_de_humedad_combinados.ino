.#include "DHT.h" // Importa la librería para el DHT

// --- Configuración Sensor Humedad de Tierra (Analógico) ---
const int sensorPin = A0;    // Pin analógico A0 para el sensor de tierra
const int valorSeco = 870;   // VALOR A CALIBRAR: Lectura del sensor en aire seco
const int valorHumedo = 425; // VALOR A CALIBRAR: Lectura del sensor en agua

// --- Configuración Sensor DHT11 (Digital) ---
#define DHTPIN 2       // Pin digital D2 para el sensor DHT11
#define DHTTYPE DHT11  // Define el tipo de sensor como DHT11

// --- Inicialización de objetos ---
DHT dht(DHTPIN, DHTTYPE); // Crea el objeto 'dht' para el sensor

void setup() {
  Serial.begin(9600); // Inicia la comunicación serial
  dht.begin();        // Inicia el sensor DHT11
}

void loop() {
  // --- 1. Lectura del Sensor de Humedad de Tierra ---
  int valorHumedad = analogRead(sensorPin);

  // Mapea el valor analógico a un porcentaje (0-100%)
  int porcentajeHumedadTierra = map(valorHumedad, valorSeco, valorHumedo, 0, 100);

  // Limita el valor para que esté siempre entre 0 y 100
  porcentajeHumedadTierra = constrain(porcentajeHumedadTierra, 0, 100);

  // --- 2. Lectura del Sensor DHT11 ---
  float h = dht.readHumidity();    // Lee la humedad ambiente
  float t = dht.readTemperature(); // Lee la temperatura ambiente

  // --- 3. Verificación de errores del DHT11 ---
  // Comprueba si alguna lectura falló (retorna NaN - Not a Number)
  if (isnan(h) || isnan(t)) {
    Serial.println("Error leyendo el sensor DHT11");
    // Espera un segundo antes de reintentar
    delay(1000); 
    return; // Sale de esta vuelta del loop y empieza de nuevo
  }

  // --- 4. Envío de datos por el Puerto Serie ---
  // Se envían los datos en formato CSV (Valores Separados por Comas)
  
  Serial.print(porcentajeHumedadTierra); // Dato 1
  Serial.print(",");                     // Separador
  Serial.print(t);                       // Dato 2
  Serial.print(",");                     // Separador
  Serial.println(h);                     // Dato 3 (y salto de línea)

  // --- 5. Delay (Espera) ---
  delay(1000);
}
