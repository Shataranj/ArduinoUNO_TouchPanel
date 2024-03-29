#include <Keypad.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

BLECharacteristic *pCharacteristic;
BLEServer *pServer;

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

byte rowPins[ROWS] = {32, 33, 25, 26, 27, 14, 12, 13}; //connect to the row pinouts of the kpd
byte colPins[COLS] = {23, 22, 21, 19, 18, 17, 16, 15}; //connect to the column pinouts of the kpd

Keypad kpd = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

unsigned long loopCount;
unsigned long startTime;
String move;
int numberOfKeyPresses;
void startBLE();

void setup()
{
  Serial.begin(9600);
  numberOfKeyPresses = 0;
  move = "";
  startBLE();
}

void loop()
{
  char key = kpd.getKey();
  if (key != NO_KEY)
  {
    int index = (int)key;
    String cell = cells[index - 1];
    Serial.println(move);
    Serial.println(cell);
    Serial.println(move != cell);

    if (numberOfKeyPresses < 2 && move != cell)
    {
      move = move + cell;
      numberOfKeyPresses++;
    }

    if (numberOfKeyPresses == 2)
    {
      Serial.print("SENDING:");
      Serial.println(move);
      pCharacteristic->setValue(move.c_str());
      pCharacteristic->notify();
      move = "";
      numberOfKeyPresses = 0;
      Serial.flush();
    }
    }

  // if (Serial.available() > 0)
  // {
  //   String input = Serial.readStringUntil('\n');
  //   if (numberOfKeyPresses < 2)
  //   {
  //     move = move + input;
  //     numberOfKeyPresses++;
  //   }

  //   if (numberOfKeyPresses == 2)
  //   {
  //     Serial.print("SENDING:");
  //     Serial.println(move);
  //     pCharacteristic->setValue(move.c_str());
  //     pCharacteristic->notify();
  //     move = "";
  //     numberOfKeyPresses = 0;
  //   }
  // }

  // if(pServer->getConnectedCount() == 0){
  //   BLEDevice::startAdvertising();
  // }
}

void startBLE()
{
  Serial.println("Starting BLE work!");

  BLEDevice::init("Rolling Pawn");
  pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
      CHARACTERISTIC_UUID,
         BLECharacteristic::PROPERTY_READ |
          BLECharacteristic::PROPERTY_WRITE |
          BLECharacteristic::PROPERTY_NOTIFY);

  pCharacteristic->addDescriptor(new BLE2902());
  pCharacteristic->setValue("Hello World says Neil");
  pService->start();
  // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06); // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined! Now you can read it in your phone!");
}