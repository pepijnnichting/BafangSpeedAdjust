# Bafang Speed Adjust

Simple speed adjustment for my Veloretti ace two.
This Arduino project adjusts the speed setting of a Bafang motor controller using an ESP32 and CAN bus communication.

## Hardware Requirements

- ESP32 development board
- [Bafang 6 pin brake cable (Female)](https://nl.aliexpress.com/item/1005006053143927.html) = 3,5 euro
- CAN transceiver module (Mcp2551, SN65HVD230, or TJA1050)

## Software Requirements

- Arduino IDE
- ESP32 board support package
- `CAN` library for Arduino

## Installation

1. **Clone the repository:**

   ```sh
   git clone https://github.com/yourusername/BafangSpeedAdjust.git
   ```

2. **Open the project in Arduino IDE:**

   Open the `BafangSpeedAdjust_esp32.ino` file located in the `BafangSpeedAdjust/BafangSpeedAdjust` directory.

3. **Install the required libraries:**

   In the Arduino IDE, go to `Sketch` > `Include Library` > `Manage Libraries...` and install the `CAN` library.

4. **Select the ESP32 board:**

   In the Arduino IDE, go to `Tools` > `Board` and select your ESP32 board.

5. **Upload the code:**

   Connect your ESP32 board to your computer and upload the code.

## Configuration

The following constants can be configured in the `BafangSpeedAdjust_esp32.ino` file:

- `canBaudRate`: The baud rate for the CAN bus communication (default: `250E3`).
- `esp32BaudRate`: The baud rate for the ESP32 serial communication (default: `115200`).
- `canId`: The CAN ID for the Bafang motor controller (default: `0x85103203`).
- `speedLimit`: The speed setting in km/h (default: `35`).
- `wheelSize`: The wheel size in mm (default: `2100`).
- `wheelPerimeter`: The wheel perimeter in mm (default: `2100`).
- `logOnlyMode`: Set to `true` for log-only mode, which will not send the speed setting (default: `true`).
- `CAN_TX_PIN`: The TX pin for the CAN communication (default: `5`).
- `CAN_RX_PIN`: The RX pin for the CAN communication (default: `4`).

## Usage

1. **Connect the hardware:**

   - Connect the CAN transceiver module to the ESP32.
   - Connect the 6 pin cable to the CAN transceiver module (H = green cable, L = white cable).
   - Connect the 6 pin cable to the Bafang motor controller (purple connector, bike can be switched on).
   - Connect the ESP32 to your computer using a USB cable.
   - If all is connected correctly, you should see a lot of data passing by in the serial monitor.

2. **Power on the system:**

   Ensure that the ESP32 and the Bafang motor controller are powered on.

3. **Monitor the serial output:**

   Open the Serial Monitor in the Arduino IDE to view the output. The speed setting will be sent after a 10-second delay if `logOnlyMode` is set to `false`.

4. **Set wheel size and speed limit:**

   The wheel size and speed limit can be set using the `writeToCan` function in the code. For example:
   ```cpp
   writeToCan(35, 2100, 2100); // Set speed limit to 35 km/h, wheel size to 2100 mm, and wheel perimeter to 2100 mm
   ```

## Troubleshooting

- **CAN communication failed:** Ensure that the CAN transceiver module is properly connected to the ESP32 and that the correct pins are set in the code.
- **No data in Serial Monitor:** Check the baud rate settings in the Serial Monitor and ensure they match the `esp32BaudRate` in the code.
- **Speed setting not applied:** Ensure that `logOnlyMode` is set to `false` and that the `speedLimit`, `wheelSize`, and `canId` values are correct.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Acknowledgments

- [ESP32](https://www.espressif.com/)
- [Bafang](https://www.bafang-e.com/)
- [Endless Sphere Forum](https://endless-sphere.com/sphere/threads/bafang-m500-m600-thread.100777/page-53)


## FYI

The first 2 groups are representing the speed limit, the next 2 groups are representing the wheel size and the last one the perimeter of the wheel. (Casainho helped me understand that in his well commented code https://github.com/OpenSourceEBike/EV_Display_Bluetooth_Ant/blob/main/firmware/display/can.c)

Original read: 82F83203 6 C4 09 C0 2B CE 08 => C4 09 => 09C4 => 2500 => 25km/h
Set:           85103203 6 AC 0D C0 2B CE 08 => AC 0D => 0DAC => 3500 => 35km/h
New read:      82F83203 6 AC 0D C0 2B CE 08