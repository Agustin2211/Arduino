/*
  Proyecto: Vúmetro LED (10 LEDs) + Módulo RGB
  Hardware: Arduino Uno, Sensor (A0), 10 LEDs, RGB (12, 13, A1)
*/

// --- Configuración de Pines ---
const int pinSensor = A0; 
const int pinesLed[10] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

// Pines para el LED RGB
const int pinRojo = 12;
const int pinVerde = 13;
const int pinAzul = A5; // Usamos el analógico A1 como digital

// --- Configuración de Sensibilidad ---
const int umbralMaximo = 100; // AJUSTAR SEGÚN EL RUIDO
const int umbralMinimo = 3;   

int picoActual = 0; 

void setup() {
  // Configurar LEDs de la barra
  for (int i = 0; i < 10; i++) {
    pinMode(pinesLed[i], OUTPUT);
  }

  // Configurar RGB como salidas
  pinMode(pinRojo, OUTPUT);
  pinMode(pinVerde, OUTPUT);
  pinMode(pinAzul, OUTPUT); // A1 actuará como digital
  
  // Apagar RGB al inicio
  colorRGB(0, 0, 0);
  
  Serial.begin(9600);
}

void loop() {
  // 1. Muestreo de sonido (igual que antes)
  unsigned long startMillis = millis(); 
  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;
  unsigned int peakToPeak = 0;

  while (millis() - startMillis < 50) {
    int sample = analogRead(pinSensor);
    if (sample < 1024) { 
       if (sample > signalMax) signalMax = sample;
       else if (sample < signalMin) signalMin = sample;
    }
  }
  peakToPeak = signalMax - signalMin;  

  // 2. Mapeo a escala de 0 a 10
  int ledsEncendidos = map(peakToPeak, umbralMinimo, umbralMaximo, 0, 10);
  ledsEncendidos = constrain(ledsEncendidos, 0, 10);

  // 3. Efecto caída suave barra LEDs
  if (ledsEncendidos < picoActual) {
      picoActual--; 
      delay(15);    
  } else {
      picoActual = ledsEncendidos; 
  }

  // 4. Encender barra de LEDs
  for (int i = 0; i < 10; i++) {
    if (i < picoActual) digitalWrite(pinesLed[i], HIGH);
    else digitalWrite(pinesLed[i], LOW);
  }

// ... dentro del loop, después de calcular picoActual ...

  switch (picoActual) {
      case 0: 
        colorRGB(0, 0, 0); // Apagado
        break;
      case 1: 
      case 2: 
        colorRGB(0, 1, 0); // Verde (Inicio)
        break;
      case 3: 
      case 4: 
        colorRGB(0, 1, 1); // Cian (Mezcla Verde + Azul)
        break;
      case 5: 
        colorRGB(0, 0, 1); // Azul
        break;
      case 6: 
        colorRGB(1, 0, 1); // Magenta (Mezcla Azul + Rojo)
        break;
      case 7: 
      case 8: 
        colorRGB(1, 1, 0); // Amarillo (Mezcla Rojo + Verde)
        break;
      case 9: 
        colorRGB(1, 0, 0); // Rojo (Fuerte)
        break;
      case 10: 
        colorRGB(1, 1, 1); // Blanco (Pico máximo: todos prendidos)
        break;
    }
}

// Función auxiliar para facilitar el cambio de color
// Si tu LED es Ánodo común, cambia los HIGH por LOW y viceversa aquí.
void colorRGB(int rojo, int verde, int azul) {
  digitalWrite(pinRojo, rojo);
  digitalWrite(pinVerde, verde);
  digitalWrite(pinAzul, azul);
}
