#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h> // <-- NUEVO: Librería para el servo

// --- Pines ---
// NOTA: He movido trigPin al 11 porque el 10 lo usará el Servo
const int trigPin = 10; 
const int echoPin = 9;
const int buzzerPin = 8; 
const int servoPin = 11; // <-- NUEVO: Pin del servo (solicitado)

// --- Variables Globales ---
long duration;
int distance;
unsigned long lastBeepTime = 0; 

// --- Objetos ---
LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo myServo; // <-- NUEVO: Objeto servo

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  
  myServo.attach(servoPin); // <-- NUEVO: Iniciar servo en pin 10
  
  Serial.begin(9600);
  
  lcd.init(); 
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Radar Prendido");
  delay(5000);
  lcd.clear();
}

// --- Funciones Auxiliares ---

int calculateDistance(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH, 30000); 
  
  if(duration == 0) {
    return -1; 
  }
  
  int dist = duration * 0.034 / 2;
  return dist;
}

void handleBuzzer(int dist) {
  int maxBeepDistance = 50; 

  if (dist < 0 || dist > maxBeepDistance) {
    noTone(buzzerPin);
    return;
  }

  long pauseBetweenBeeps = map(dist, 0, maxBeepDistance, 50, 750);
  unsigned long currentTime = millis();

  if (currentTime - lastBeepTime > pauseBetweenBeeps) {
    lastBeepTime = currentTime;
    tone(buzzerPin, 1000, 50); 
  }
}

void showOnLCD(int angle, int dist){
  lcd.clear();
  lcd.setCursor(0,0);
  
  if(dist >= 0){
    lcd.print("Distancia: ");
    lcd.print(dist);
    lcd.print(" cm");
  } else {
    lcd.print("Distancia: ---");
  }

  lcd.setCursor(0,1);
  int barLen = 0;
  if(dist > 0){
    barLen = map(constrain(dist, 0, 200), 0, 200, 0, 16);
  }
  
  for(int i=0; i<barLen; i++) {
    lcd.write(byte(255)); 
  }
}

void loop() {
  // --- BARRIDO DE IDA (15 a 165 grados) ---
  for(int i = 15; i <= 165; i++){
    myServo.write(i); // <-- NUEVO: Mover servo a posición 'i'
    delay(30);        // <-- IMPORTANTE: Dar tiempo al servo para llegar
    
    distance = calculateDistance();
    handleBuzzer(distance);
    
    // Serial
    Serial.print(i);
    Serial.print(",");
    Serial.print(distance);
    Serial.print(".");
    
    // LCD
    showOnLCD(i, distance);
  }
  
  // --- BARRIDO DE VUELTA (165 a 15 grados) ---
  for(int i = 165; i > 15; i--){
    myServo.write(i); // <-- NUEVO: Mover servo
    delay(30);        // <-- IMPORTANTE: Pausa para movimiento
    
    distance = calculateDistance();
    handleBuzzer(distance);
    
    Serial.print(i);
    Serial.print(",");
    Serial.print(distance);
    Serial.print(".");
    
    showOnLCD(i, distance);
  }
}
