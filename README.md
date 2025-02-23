# Bafang Speed Adjust

Simple speed adjustment for my my Veloretti ace two.
This Arduino project adjusts the speed setting of a Bafang motor controller using an ESP32 and CAN bus communication.

## Hardware Requirements

- ESP32 development board
- [Bafang 6 pin brake cable (Female)](https://nl.aliexpress.com/item/1005006053143927.html) = 3,5 euro
- transceiver module (Mcp2551, SN65HVD230 or TJA1050)

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
- `speedKmH`: The speed setting in km/h (default: `35`).
- `canId`: The CAN ID for the Bafang motor controller (default: `0x85103203`).
- `logOnlyMode`: Set to `true` for log-only mode, which will not send the speed setting (default: `true`).

## Usage

1. **Connect the hardware:**

   - Connect the CAN transceiver module to the ESP32.
   - Connect the 6 pin cable to the CAN transceiver module (H = green cable, L= white cable)
   - Connect the 6 pin cable to the Bafang motor controller (purple connector, bike can be switched on)
   - Connect the ESP32 to your computer using a USB cable.
   - If all is connected correctly, you should see a lot of data passing by in the serial monitor.


2. **Power on the system:**

   Ensure that the ESP32 and the Bafang motor controller are powered on.

3. **Monitor the serial output:**

   Open the Serial Monitor in the Arduino IDE to view the output. The speed setting will be sent after a 10-second delay if `logOnlyMode` is set to `false`.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Acknowledgments

- [ESP32](https://www.espressif.com/)
- [Bafang](https://www.bafang-e.com/)
https://endless-sphere.com/sphere/threads/bafang-m500-m600-thread.100777/page-53