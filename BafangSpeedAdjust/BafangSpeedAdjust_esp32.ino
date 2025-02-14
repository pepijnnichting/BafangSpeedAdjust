#include <SPI.h>
#include <CAN.h>

bool speedSet = false;
const long canBaudRate = 250E3;
const long esp32BaudRate = 115200;
const int speedKmH = 35; // Speed in km/h
const long canId = 0x85103203; // CAN ID
const bool logOnlyMode = true; // Set to true for log-only mode

void sendSpeed(int speedKmH) {
  int speed = speedKmH * 100; // Convert km/h to the required format
  CAN.beginExtendedPacket(canId);
  CAN.write((speed >> 8) & 0xFF); // High byte
  CAN.write(speed & 0xFF);        // Low byte
  CAN.write(0xC0);
  CAN.write(0x2B);
  CAN.write(0xCE);
  CAN.write(0x08);
  CAN.endPacket();
}

void printRepeatedMessage(const char* message, int count) {
  for (int i = 0; i < count; i++) {
    Serial.println(message);
  }
}

void setup() {
  Serial.begin(esp32BaudRate);
  
  if (!CAN.begin(canBaudRate)) {
    Serial.println("Starting CAN failed!");
    while (1);
  }

  Serial.println("Setup ok");
  Serial.println();

  if (!logOnlyMode) {
    sendSpeed(speedKmH);
  }
}

void loop() {
  unsigned long currentMillis = millis();
  
  int packetSize = CAN.parsePacket();
  if (packetSize) {
    if (CAN.packetExtended()) {
      Serial.print("Extended ID: 0x");
      Serial.print(CAN.packetId(), HEX);
    } else {
      Serial.print("Standard ID: 0x");
      Serial.print(CAN.packetId(), HEX);
    }

    Serial.print(" DLC: ");
    Serial.print(packetSize);

    while (CAN.available()) {
      Serial.print(" ");
      Serial.print(CAN.read(), HEX);
    }
    Serial.println();
  }

  //Write speed setting after 10 seconds waiting
  if(!logOnlyMode && !speedSet && currentMillis > 10000){
    speedSet = true;
    printRepeatedMessage("------ WRITING SPEED ------", 6);

    sendSpeed(speedKmH);

    printRepeatedMessage("----------- DONE ----------", 6);
  }
}
