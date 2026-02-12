#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "pitches.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define BUZZER_PIN 9

// ===== TABLERO =====
#define BLOCK 8
#define GRID_W 16
#define GRID_H 8

bool grid[GRID_H][GRID_W];

// ===== TETROMINOS (4x4) =====
const bool tetrominos[7][4][4] = {
  {{0,0,0,0},{0,1,1,0},{0,1,1,0},{0,0,0,0}}, // O
  {{0,0,0,0},{1,1,1,1},{0,0,0,0},{0,0,0,0}}, // I
  {{0,0,0,0},{1,1,1,0},{0,1,0,0},{0,0,0,0}}, // T
  {{0,0,0,0},{1,1,1,0},{1,0,0,0},{0,0,0,0}}, // L
  {{0,0,0,0},{1,1,1,0},{0,0,1,0},{0,0,0,0}}, // J
  {{0,0,0,0},{0,1,1,0},{1,1,0,0},{0,0,0,0}}, // S
  {{0,0,0,0},{1,1,0,0},{0,1,1,0},{0,0,0,0}}  // Z
};

// ===== PIEZA ACTUAL =====
int pieza, px, py;

// ===== TIEMPO =====
int tick = 0;
int ticksParaCaer = 3;
const int minTicks = 1;

// ===== MELODÍA =====
int melody[] = {
  NOTE_E5, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_C5, NOTE_B4,
  NOTE_A4, NOTE_A4, NOTE_C5, NOTE_E5, NOTE_D5, NOTE_C5,
  NOTE_B4, NOTE_C5, NOTE_D5, NOTE_E5,
  NOTE_C5, NOTE_A4, NOTE_A4, NOTE_A4, NOTE_B4, NOTE_C5,
  NOTE_D5, NOTE_F5, NOTE_A5, NOTE_G5, NOTE_F5,
  NOTE_E5, NOTE_C5, NOTE_E5, NOTE_D5, NOTE_C5,
  NOTE_B4, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_E5,
  NOTE_C5, NOTE_A4, NOTE_A4, REST
};

int durations[] = {
  4,8,8,4,8,8,4,8,8,4,8,8,4,8,4,4,
  4,4,8,4,8,8,
  4,8,4,8,8,
  4,8,4,8,8,
  4,8,8,4,4,
  4,4,4,4
};

int melodySize = sizeof(durations) / sizeof(int);

// ===== FUNCIONES =====
bool colision(int nx, int ny) {
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      if (tetrominos[pieza][y][x]) {
        int gx = nx + x;
        int gy = ny + y;

        if (gx < 0 || gx >= GRID_W || gy >= GRID_H) return true;
        if (gy >= 0 && grid[gy][gx]) return true;
      }
    }
  }
  return false;
}

void fijarPieza() {
  for (int y = 0; y < 4; y++)
    for (int x = 0; x < 4; x++)
      if (tetrominos[pieza][y][x])
        if (py + y >= 0)
          grid[py + y][px + x] = true;
}

void dibujarTablero() {
  for (int y = 0; y < GRID_H; y++)
    for (int x = 0; x < GRID_W; x++)
      if (grid[y][x])
        display.fillRect(x * BLOCK, y * BLOCK, BLOCK, BLOCK, WHITE);
}

void dibujarPieza() {
  for (int y = 0; y < 4; y++)
    for (int x = 0; x < 4; x++)
      if (tetrominos[pieza][y][x])
        display.fillRect((px + x) * BLOCK, (py + y) * BLOCK, BLOCK, BLOCK, WHITE);
}

void nuevaPieza() {
  pieza = random(0, 7);
  px = random(0, GRID_W - 4);
  py = -1;
}

// ===== ANIMACIÓN GAME OVER =====
void animacionGameOver() {
  noTone(BUZZER_PIN);

  unsigned long inicio = millis();
  bool grande = false;

  while (millis() - inicio < 5000) {   // 3 segundos
    display.clearDisplay();

    display.setTextColor(WHITE);

    if (grande) {
      display.setTextSize(2);
      display.setCursor(13, 24);   // centrado para size 2
    } else {
      display.setTextSize(1);
      display.setCursor(40, 28);   // centrado para size 1
    }

    display.print("GAME OVER");
    display.display();

    grande = !grande;
    delay(450);   // latido suave
  }
}


// ===== SETUP =====
void setup() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) while (1);
  randomSeed(analogRead(0));
  nuevaPieza();
}

// ===== LOOP PRINCIPAL =====
void loop() {

  int note = 0;

  while (true) {

    display.clearDisplay();
    dibujarTablero();
    dibujarPieza();
    display.display();

    tick++;
    if (tick >= ticksParaCaer) {
      tick = 0;

      if (!colision(px, py + 1)) {
        py++;
      } else {
        fijarPieza();
        nuevaPieza();

        if (ticksParaCaer > minTicks)
          ticksParaCaer--;

        if (colision(px, py)) {
          animacionGameOver();
          while (1);
        }
      }
    }

    int duration = 1000 / durations[note];
    tone(BUZZER_PIN, melody[note], duration);
    delay(duration * 1.3);
    noTone(BUZZER_PIN);

    note++;
    if (note >= melodySize) note = 0; // LOOP MUSICAL
  }
}
