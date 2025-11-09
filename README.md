# ESP32-Bidirectional-Environmental-Monitoring
ESP32-C3 based environmental monitoring system with MQTT, Node-RED dashboard, and bi-directional LED control.
# ESP32-Based Environmental Monitoring System with Cloud Analytics and Bi-Directional Control

This repository contains the implementation files for an ESP32-C3 based environmental monitoring system using a DHT11 sensor and NeoPixel RGB LED. The system integrates a locally hosted Mosquitto MQTT broker, Node-RED dashboard, and MongoDB for data logging and visualization.

## System Features

- Real-time temperature and humidity monitoring
- Bi-directional control between Node-RED dashboard and ESP32-C3
- Local MQTT publish/subscribe communication
- MongoDB database for historical telemetry
- Visual LED feedback in manual and automatic modes

## System Overview

**Architecture:**
- ESP32-C3 with DHT11 and NeoPixel for sensing and actuation
- Local Mosquitto MQTT broker
- Node-RED for flow management and dashboard interface
- MongoDB for data storage

## Demonstration Video

A full system demonstration video is available on YouTube:  


## Repository Structure

