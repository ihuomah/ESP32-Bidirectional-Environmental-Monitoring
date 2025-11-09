# ESP32-Bidirectional-Environmental-Monitoring

## ESP32-Based Environmental Monitoring System with Cloud Analytics and Bi-Directional Control

This repository contains the implementation files for an ESP32-C3-based environmental monitoring system using a DHT11 sensor and NeoPixel RGB LED.  
The system integrates a locally hosted Mosquitto MQTT broker, Node-RED dashboard, and MongoDB for data logging and visualization.

---

## System Features

- Real-time temperature and humidity monitoring  
- Bi-directional control between Node-RED dashboard and ESP32-C3  
- Local MQTT publish/subscribe communication  
- MongoDB database for historical telemetry  
- Visual LED feedback in manual and automatic modes  

---

## System Overview

### Architecture Components

- **ESP32-C3:**
- **DHT11**
- **NeoPixel RGB LED**  
- **Mosquitto MQTT Broker:** Local publish/subscribe message hub  
- **Node-RED:** Flow management, dashboard interface, and control logic  
- **MongoDB:** Local database for storage and retrieval  

---

## Demonstration Video

A full system demonstration video is available on YouTube:  
[▶ Watch Demonstration Video on YouTube](https://youtu.be/-6btnIcJw_A)

---

## Repository Structure

