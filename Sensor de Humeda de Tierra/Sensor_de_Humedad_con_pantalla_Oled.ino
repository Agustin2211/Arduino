#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Configuración de la pantalla OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Definición de pines
const int sensorPin = A0;
const int ledRojo = 6;
const int ledVerde = 7;

int umbral = 500; 

void setup() {
  Serial.begin(9600);

  pinMode(ledRojo, OUTPUT);
  pinMode(ledVerde, OUTPUT);

  // Inicializar pantalla OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("Fallo al iniciar SSD1306"));
    for(;;);
  }

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0,0);
  display.println("Iniciando...");
  display.display();
  delay(2000);
}

void loop() {
  // Leer el valor del sensor
  int lecturaHumedad = analogRead(sensorPin);
  
  // Imprimir valor en monitor serie para depuración
  Serial.print("Humedad: ");
  Serial.println(lecturaHumedad);

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Humedad: ");
  display.println(lecturaHumedad);
  
  display.setTextSize(1);
  display.setCursor(0, 30);

  if (lecturaHumedad > umbral) {

    digitalWrite(ledRojo, HIGH);
    digitalWrite(ledVerde, LOW);
    
    display.println("Hace Falta");
    display.setTextSize(2);
    display.setCursor(0, 45);
    display.println("REGAR!");
    
  } else {
    
    digitalWrite(ledRojo, LOW);
    digitalWrite(ledVerde, HIGH);
    
    display.println("No hace falta");
    display.setTextSize(2);
    display.setCursor(0, 45);
    display.println("REGAR");
  }

  display.display();
  delay(1000);
}
