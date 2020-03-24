
#include <stdint.h>
#include <Arduino.h>
#include "TouchScreen.h"

#define YP A0  // must be an analog pin, use "An" notation!
#define XP A1  // can be a digital pin
#define YM A2  // can be a digital pin
#define XM A3 // must be an analog pin, use "An" notation!

boolean isValidTouch();
void readTouchPosition();


// #define X_MIN 130
// #define X_MAX 755
// #define Y_MIN 128
// #define Y_MAX 890


#define X_MIN 103
#define X_MAX 919
#define Y_MIN 252
#define Y_MAX 964

#define X_CELL_SIZE ((X_MAX - X_MIN)/8)
#define Y_CELL_SIZE ((Y_MAX - Y_MIN)/8)

const char ranks[] = {'1', '2', '3', '4', '5', '6', '7', '8'};
const char files[] = {'h', 'g', 'f', 'e', 'd', 'c', 'b', 'a'};

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 000);
typedef struct {
  int x, y;
} POSITION;

char rank, file;
POSITION pos;

void setup(void) {
  Serial.begin(9600);
  
}

void loop(void) {

  readTouchPosition();
  while(isValidTouch()){
    Serial.print(files[((pos.y -Y_MIN)/ Y_CELL_SIZE)]);
    Serial.println(ranks[((pos.x - X_MIN)/ X_CELL_SIZE)]);
    readTouchPosition();
    delay(20);
  }
  delay(100);

}


boolean isValidTouch()
{
  return pos.x>X_MIN && pos.x<X_MAX && pos.y>Y_MIN && pos.y<Y_MAX;
}


void readTouchPosition() {
  TSPoint p = ts.getPoint();
  pos.x = p.x;
  pos.y = p.y;
  delay(100);
}