#include <SPI.h>
#include "LedMatrix.h"

#define DELAY 60

#define NUMBER_OF_DEVICES 1
#define CS_PIN D4
LedMatrix ledMatrix = LedMatrix(NUMBER_OF_DEVICES, CS_PIN);

#define SIZE_OF_MATRIX  8
#define NUMBER_OF_FRAZES  8

const byte FRAZES[NUMBER_OF_FRAZES][SIZE_OF_MATRIX] = {
  { //СВЕТ МОЕЙ ЖИЗНИ 0
    B01111000,
    B11110000,
    B00000000,
    B00111110,
    B00000000,
    B00000000,
    B00000000,
    B00000000
  }, {//МИЛАЯ 1
    B00000000,
    B10000000,
    B10000000,
    B10000000,
    B10000000,
    B10000000,
    B00000000,
    B00000000
  },
  { //СОЛНЦЕ 2
    B01000000,
    B01000000,
    B01000000,
    B01000000,
    B01000000,
    B01000000,
    B00000000,
    B00000000
  },
  { //ЛЮБИМАЯ 3
    B00000000,
    B00000001,
    B00000001,
    B00000001,
    B00000001,
    B00000001,
    B00000001,
    B00000001
  }, {//ЯТЛ 4
    B10001111,
    B00000000,
    B01111100,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000
  },
  { //С ДР 5
    B01000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00001111,
    B00000000,
    B11111111
  },
  { //ДУШИ НЕ ПРОБЛЕМА 6
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00001111,
    B00000110,
    B11111111,
    B00000000
  },
  {
    B00000000,
    B00100100,
    B01111110,
    B11111111,
    B01111110,
    B00111100,
    B00011000,
    B00000000
  }
};

void setup() {
  ledMatrix.init();
  ledMatrix.clear();
  render();
}
int current_fraze = 0;
int next_fraze = 1;

void loop() {
  delay(10000);
  next();
}

void render() {
  if (false) {
    doors(FRAZES[current_fraze], FRAZES[next_fraze]);
    return;
  }
  switch (current_fraze) {
    case 1:
    case 3:
      fill_from_left_angle(FRAZES[current_fraze], FRAZES[next_fraze]);
      break;
    case 2:
    case 6:
      fill_horizontal(FRAZES[current_fraze], FRAZES[next_fraze]);
      break;
    case 4:
    case 7:
      doors(FRAZES[current_fraze], FRAZES[next_fraze]);
      break;
    case 0:
    case 5:
      left_right_fill(FRAZES[current_fraze], FRAZES[next_fraze]);
      break;
    default:
      fill_horizontal(FRAZES[current_fraze], FRAZES[next_fraze]);
  }
}


void next() {
  current_fraze = next_fraze;
  if (++next_fraze == NUMBER_OF_FRAZES) {
    next_fraze = 0;
  }
  render();
}


void left_right_fill(const byte old_fraze[8], const byte new_fraze[8]) {
  for (int i = 0; i < SIZE_OF_MATRIX; i++) {

    for (int j = 0; j < SIZE_OF_MATRIX; j++) {
      if (i % 2 == 0)
        ledMatrix.setColumn(i, ((B11111111 << (7 - j)) | old_fraze[i]));
      else
        ledMatrix.setColumn(i, ((B11111111 >> (7 - j))) | old_fraze[i]);
      ledMatrix.commit();
      delay(20);
    }
  }
  for (int i = 0; i < SIZE_OF_MATRIX; i++) {
    for (int j = 0; j < SIZE_OF_MATRIX; j++) {
      if (i % 2 == 0)
        ledMatrix.setColumn(i, ((B11111111 >> (j + 1)) | new_fraze[i]));
      else
        ledMatrix.setColumn(i, ((B11111111 << (j + 1)) | new_fraze[i]));
      ledMatrix.commit();
      delay(20);
    }
  }
}

void fill_horizontal(const byte old_fraze[8], const byte new_fraze[8]) {
  for (int i = 0; i < SIZE_OF_MATRIX; i++) {
    ledMatrix.setColumn(i, B11111111);
    ledMatrix.commit();
    delay(DELAY);
  }
  for (int i = 0; i < SIZE_OF_MATRIX; i++) {
    ledMatrix.setColumn(i, new_fraze[i]);
    ledMatrix.commit();
    delay(DELAY);
  }
}

void fill_from_left_angle(const byte old_fraze[8], const byte new_fraze[8]) {
  for (int i = 0; i < SIZE_OF_MATRIX; i++) {
    for (int j = 0; j <= i; j++) {
      ledMatrix.setColumn(j, ((B11111111 << (7 - i)) | old_fraze[j]));
    }
    for (int j = i + 1; j < SIZE_OF_MATRIX; j++) {
      ledMatrix.setColumn(j, old_fraze[j]);
    }
    ledMatrix.commit();
    delay(DELAY);
  }
  for (int i = 0; i < SIZE_OF_MATRIX; i++) {
    for (int j = 0; j <= i; j++) {
      ledMatrix.setColumn(j, new_fraze[j]);
    }
    for (int j = i + 1; j < SIZE_OF_MATRIX; j++) {
      ledMatrix.setColumn(j, (B11111111 >> (i + 1)) | new_fraze[j]);
    }

    ledMatrix.commit();
    delay(DELAY);
  }
}



void fill_vertical(const byte old_fraze[8], const byte new_fraze[8]) {
  const byte ROW[9] = {
    B11111111,
    B01111111,
    B00111111,
    B00011111,
    B00001111,
    B00000111,
    B00000011,
    B00000001,
    B00000000
  };
  for (int j = 0; j < 9; j++) {
    for (int i = 0; i < SIZE_OF_MATRIX; i++) {
      ledMatrix.setColumn(i, (ROW[8 - j] | old_fraze[i]));
    }
    ledMatrix.commit();
    delay(DELAY);
  }
  for (int j = 0; j < 9; j++) {
    for (int i = 0; i < SIZE_OF_MATRIX; i++) {
      ledMatrix.setColumn(i,  (ROW[j] | new_fraze[i]));
    }
    ledMatrix.commit();
    delay(DELAY);
  }
}

void doors(const byte old_fraze[8], const byte new_fraze[8]) {
  for (int i = 0; i <= SIZE_OF_MATRIX / 2; i++) {
    for (int j = 0; j < SIZE_OF_MATRIX; j++) {
      int doors = B11110000 << (4 - i);
      doors = doors |  (B00001111 >> (4 - i));
      ledMatrix.setColumn(j, (doors | old_fraze[j]));
    }
    ledMatrix.commit();
    delay(DELAY);
  }
  for (int i = 0; i <= SIZE_OF_MATRIX / 2; i++) {
    for (int j = 0; j < SIZE_OF_MATRIX; j++) {
      int doors = B11110000 << (i);
      doors = doors |  (B00001111 >> (i));
      ledMatrix.setColumn(j, (doors | new_fraze[j]));
    }
    ledMatrix.commit();
    delay(DELAY);
  }
}
