#include <SPI.h>
#include <CAN.h>

bool speedSet = false;
const long canBaudRate = 250E3;
const long esp32BaudRate = 115200;
const int speedKmH = 35; // Speed in km/h
const long canId = 0x85103203; // CAN ID

void setup() {
  Serial.begin(esp32BaudRate);
  
  if (!CAN.begin(canBaudRate)) {
    Serial.println("Starting CAN failed!");
    while (1);
  }

  Serial.println("Setup ok");
  Serial.println();

  //Original read: 82F83203 6 C4 09 C0 2B CE 08 => C4 09 => 09C4 => 2500 => 25km/h
  //Set:           85103203 6 AC 0D C0 2B CE 08 => AC 0D => 0DAC => 3500 => 35km/h
  //New read:      82F83203 6 AC 0D C0 2B CE 08
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
  if(!speedSet && currentMillis > 10000){
    speedSet = true;
    Serial.println("------ WRITING SPEED ------");
    Serial.println("------ WRITING SPEED ------");
    Serial.println("------ WRITING SPEED ------");
    Serial.println("------ WRITING SPEED ------");
    Serial.println("------ WRITING SPEED ------");
    Serial.println("------ WRITING SPEED ------");

    int speed = speedKmH * 100; // Convert km/h to the required format

    CAN.beginExtendedPacket(canId);
    CAN.write((speed >> 8) & 0xFF); // High byte
    CAN.write(speed & 0xFF);        // Low byte
    CAN.write(0xC0);
    CAN.write(0x2B);
    CAN.write(0xCE);
    CAN.write(0x08);
    CAN.endPacket();

    Serial.println("----------- DONE ----------");
    Serial.println("----------- DONE ----------");
    Serial.println("----------- DONE ----------");
    Serial.println("----------- DONE ----------");
    Serial.println("----------- DONE ----------");
    Serial.println("----------- DONE ----------");
  }
}
