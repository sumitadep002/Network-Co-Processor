## Project Overview
This project implements an **IoT Gateway system** where an **STM32** acts as the main controller and an **ESP32** is used as a **Network Co-Processor (NCP)**.

The STM32 handles the core gateway logic, peripheral control, and data collection, while the ESP32 is responsible for providing **WiFi connectivity** and enabling **secure communication between the local network and the cloud**.

The ESP32 receives data from STM32 and forwards it to the cloud (MQTT/HTTP), and also delivers cloud commands back to STM32 for execution.

