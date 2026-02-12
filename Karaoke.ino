#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "pitches.h"

// ---------- OLED ----------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ---------- BUZZER ----------
#define BUZZER_PIN 9

// ---------- MELODÍA VOCAL: BLACKOUT ----------
// Esta secuencia imita el fraseo: "Y yo me le pegué / y él se me pegó..."
const uint16_t melody[] PROGMEM = {
  NOTE_G3, NOTE_G3, NOTE_G3, NOTE_G3, NOTE_AS3, // Y yo me le pe-gué
  0, NOTE_G3, NOTE_G3, NOTE_G3, NOTE_AS3,       // y él se me pe-gó
  0, NOTE_G3, NOTE_AS3, NOTE_C4,                // (Woh-woh)
  NOTE_C4, NOTE_C4, NOTE_C4, NOTE_C4, NOTE_C4, NOTE_AS3, NOTE_G3, // To-da la no-che pe-gao
  
  0, NOTE_G3, NOTE_G3, NOTE_G3, NOTE_G3, NOTE_AS3, // Y yo me le pe-gué
  0, NOTE_G3, NOTE_G3, NOTE_G3, NOTE_AS3,       // y él se me pe-gó
  0, NOTE_G3, NOTE_AS3, NOTE_C4,                // (Woh-woh)
  NOTE_C4, NOTE_C4, NOTE_C4, NOTE_C4, NOTE_C4, NOTE_AS3, NOTE_G3  // To-da la no-che pe-gao
};

// Duraciones ajustadas al fraseo real de la letra
const uint8_t durations[] PROGMEM = {
  16, 16, 16, 16, 8,   // Y yo me le pe-gué
  16, 16, 16, 16, 8,   // y él se me pe-gó
  16, 8, 8, 4,         // Woh-woh
  16, 16, 16, 16, 16, 8, 4, // To-da la no-che pe-gao
  
  16, 16, 16, 16, 16, 8, 
  16, 16, 16, 16, 8, 
  16, 8, 8, 4,
  16, 16, 16, 16, 16, 8, 4
};

int notesCount = sizeof(melody) / sizeof(uint16_t);

// --- Variables para el texto ---
const char* songTitle = "BLACKOUT";
const char* artistName = "EMILIA, TINI, NICKI";

void drawKaraokeInfo() {
  display.clearDisplay();
  display.setTextColor(WHITE);
  
  // Título
  display.setTextSize(2);
  display.setCursor(15, 15); 
  display.print(songTitle);
  
  // Artistas
  display.setTextSize(1);
  display.setCursor(10, 45);
  display.print(artistName);
  
  display.display();
}

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (true);
  }
  drawKaraokeInfo(); // Texto fijo desde el inicio
}

void loop() {
  for (int i = 0; i < notesCount; i++) {
    int note = pgm_read_word(&melody[i]);
    int dur  = pgm_read_byte(&durations[i]);
    
    // Calculamos la duración de la nota en milisegundos
    int duration = 1000 / dur;

    if (note > 0) {
      tone(BUZZER_PIN, note, duration);
    }

    // El delay es un poco más largo que la nota para que no se junten las palabras
    delay(duration * 1.3); 
    noTone(BUZZER_PIN);
  }
  
  delay(3000); // Pausa larga antes de que empiece a cantar de nuevo
}
