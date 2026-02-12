const int pinSensor = A0;
const int pinLedRojo = 3;
const int pinLedVerde = 7;

float umbralCalor = 30.0;

void setup (){
  pinMode(pinLedRojo, OUTPUT);
  pinMode(pinLedVerde, OUTPUT);
  Serial.begin(9600);
}

void loop(){
  int lectura = analogRead(pinSensor);

  float voltaje = lectura * (5.0/1023.0);

  float temperaturaC = (voltaje - 0.5) * 100;

  Serial.print("Temperatura: ");
  Serial.print(temperaturaC);
  Serial.print(" C°");

  if (temperaturaC >= umbralCalor) {
    digitalWrite(pinLedVerde, HIGH);
    digitalWrite(pinLedRojo, LOW);
  } else {
    digitalWrite(pinLedVerde, LOW);
    digitalWrite(pinLedRojo, HIGH);
  }

  delay(500);
}
