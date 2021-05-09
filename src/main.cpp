#include <Keypad.h>

const byte ROWS = 8; //four rows
const byte COLS = 8; //three columns
char keys[ROWS][COLS] = {
    {1, 2, 3, 4, 5, 6, 7, 8},
    {9, 10, 11, 12, 13, 14, 15, 16},
    {17, 18, 19, 20, 21, 22, 23, 24},
    {25, 26, 27, 28, 29, 30, 31, 32},
    {33, 34, 35, 36, 37, 38, 39, 40},
    {41, 42, 43, 44, 45, 46, 47, 48},
    {49, 50, 51, 52, 53, 54, 55, 56},
    {57, 58, 59, 60, 61, 62, 63, 64},
};

String cells[64] = {"a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
                    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
                    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
                    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
                    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
                    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
                    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
                    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1"};

byte rowPins[ROWS] = {13, 14, 27, 26, 12, 32, 33, 25}; //connect to the row pinouts of the kpd
byte colPins[COLS] = {23, 22, 21, 19, 18, 17, 16, 15}; //connect to the column pinouts of the kpd

Keypad kpd = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

unsigned long loopCount;
unsigned long startTime;
String msg;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  char key = kpd.getKey();
  if (key != NO_KEY)
  {
    int index = (int)key;
    String cell = cells[index - 1];
    Serial.println(cell);
  }
}