# ESP32 Soil Moisture and LED Control

This project demonstrates how to use an ESP32 microcontroller to monitor soil moisture and control an LED remotely through a web interface.

## Description

The ESP32 microcontroller connects to a local WiFi network and hosts a web server. Users can access the web server to view the current soil moisture level and control an LED attached to the ESP32.

## Features

- Displays real-time soil moisture level on a web interface.
- Allows users to turn an LED on or off remotely.
- Provides a simple and intuitive user interface.

## Installation

1. Clone this repository to your local machine.
2. Open the project in your preferred Arduino IDE.
3. Install the necessary libraries:
   - WiFi.h
   - ESPAsyncWebServer.h
4. Update the `ssid` and `password` variables in the code with your WiFi network credentials.
5. Upload the code to your ESP32 board.
6. Open the serial monitor to view the ESP32's IP address.
7. Access the provided IP address from a web browser to interact with the interface.

## Usage

- Connect the soil moisture sensor to the designated pin on the ESP32.
- Connect an LED to the specified pin on the ESP32.
- Power up the ESP32 and wait for it to connect to the WiFi network.
- Open a web browser and enter the ESP32's IP address.
- View the soil moisture level and control the LED using the buttons provided on the web interface.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgements

- This project was inspired by the need for remote monitoring and control of environmental conditions.
- Special thanks to the creators of the ESP32 Arduino Core and the AsyncWebServer library for making this project possible.
