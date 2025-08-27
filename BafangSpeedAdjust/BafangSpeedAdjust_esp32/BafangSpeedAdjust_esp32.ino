#include <CAN.h>

bool speedSet = false;
const long canBaudRate = 250E3;
const long esp32BaudRate = 115200;
const long canId = 0x85103203; // CAN ID
const int speedLimit = 35; // Speed limit in km/h
const int wheelSize = 11040; // Wheel size in mm (calculated to match 0xC0 and 0x2B)
const int wheelPerimeter = 2254; // Wheel perimeter in mm (calculated to match 0xCE and 0x08)
const bool logOnlyMode = true; // Set to true for log-only mode
const int CAN_TX_PIN = 5; // Set your CAN TX pin
const int CAN_RX_PIN = 4; // Set your CAN RX pin

void writeToCan(int speedLimit, int wheelSize, int wheelPerimeter) {
  int speed = speedLimit * 100; // Convert km/h to the required format
  uint32_t temp_wheel_size = (wheelSize / 10) & 0x0F;
  temp_wheel_size = temp_wheel_size << 4;
  temp_wheel_size += (wheelSize % 10);

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
  
  // Set CAN pins
  CAN.setPins(CAN_TX_PIN, CAN_RX_PIN);

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

    // Print human-readable data if packetid ends with 0x3203
    if (packetSize >= 6 && (CAN.packetId() & 0xFFFF) == 0x3203) {
      CAN.beginPacket();
      int speedHigh = CAN.read();
      int speedLow = CAN.read();
      int speed = (speedHigh << 8) | speedLow;
      Serial.print("Speed Limit: ");
      Serial.print(speed / 100.0);
      Serial.println(" km/h");

      int wheelSizeLow = CAN.read();
      int wheelSizeHigh = CAN.read();
      int wheelSize = ((wheelSizeHigh & 0x0F) << 4) | (wheelSizeLow & 0x0F);
      Serial.print("Wheel Size: ");
      Serial.print(wheelSize * 10);
      Serial.println(" mm");

      int wheelPerimeterLow = CAN.read();
      int wheelPerimeterHigh = CAN.read();
      int wheelPerimeter = (wheelPerimeterHigh << 8) | wheelPerimeterLow;
      Serial.print("Wheel Perimeter: ");
      Serial.print(wheelPerimeter);
      Serial.println(" mm");
    }
  }

  // Write speed setting after 10 seconds waiting
  if (!logOnlyMode && !speedSet && currentMillis > 10000) {
    speedSet = true;
    printRepeatedMessage("------ WRITING SPEED ------", 6);

    writeToCan(speedLimit, wheelSize, wheelPerimeter);

    printRepeatedMessage("----------- DONE ----------", 6);
  }
}
