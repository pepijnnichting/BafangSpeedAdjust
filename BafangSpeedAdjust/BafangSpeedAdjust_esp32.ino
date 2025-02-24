#include <CAN.h>

bool speedSet = false;
const long canBaudRate = 250E3;
const long esp32BaudRate = 115200;
const long canId = 0x85103203; // CAN ID
const int speedLimit = 35; // Speed limit in km/h
const int wheelSize = 11040; // Wheel size in mm (calculated to match 0xC0 and 0x2B)
const int wheelPerimeter = 2254; // Wheel perimeter in mm (calculated to match 0xCE and 0x08)
const bool logOnlyMode = true; // Set to true for log-only mode

void writeToCan(int speedLimit, int wheelSize, int wheelPerimeter) {
  int speed = speedLimit * 100; // Convert km/h to the required format

  CAN.beginExtendedPacket(canId);
  CAN.write((speed >> 8) & 0xFF); // Speed limit high byte
  CAN.write(speed & 0xFF);        // Speed limit low byte
  CAN.write(temp_wheel_size);     // Wheel size low byte
  CAN.write((wheelSize >> 8) & 0xFF); // Wheel size high byte
  CAN.write(wheelPerimeter & 0xFF);   // Wheel perimeter low byte
  CAN.write((wheelPerimeter >> 8) & 0xFF); // Wheel perimeter high byte
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
    writeToCan(speedLimit, wheelSize, wheelPerimeter);
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

  // Write speed setting after 10 seconds waiting
  if (!logOnlyMode && !speedSet && currentMillis > 10000) {
    speedSet = true;
    printRepeatedMessage("------ WRITING SPEED ------", 6);

    writeToCan(speedLimit, wheelSize, wheelPerimeter);

    printRepeatedMessage("----------- DONE ----------", 6);
  }
}
