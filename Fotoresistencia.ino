const int pinLDR = A0;
const int pinLedVerde = 7;
const int pinLedRojo = 6;

int umbral = 500;

void setup() {
  pinMode(pinLedVerde, OUTPUT);
  pinMode(pinLedRojo, OUTPUT);
  pinMode(pinLDR, INPUT);

  Serial.begin(9600);
}

void loop() {
  int lecturaLuz = analogRead(pinLDR);
  Serial.print("Nivel de Luz: ");
  Serial.print(lecturaLuz);
  Serial.print(" ");

  if(lecturaLuz < umbral) {
    digitalWrite(pinLedRojo,HIGH);
    digitalWrite(pinLedVerde, LOW); 
   }else{
    digitalWrite(pinLedRojo, LOW);
    digitalWrite(pinLedVerde, HIGH);
    }

  delay(1000);
}
