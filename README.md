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
```ESP32-Bidirectional-Env-Monitor/
│
├── code/  ESP32 Arduino sketch
├── node-red/ Exported Node-RED flow
├── docs/  Diagrams and reference images
└── README.md

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
## Prerequisites
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
The system was tested with the following:

Arduino IDE

Mosquitto MQTT broker (local installation)

Node-RED (local installation)

MongoDB Community Server (local installation)
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
## Setup Instructions
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Follow these steps to reproduce the system locally.

1. Hardware

ESP32-C3 development board

DHT11 sensor module

Single NeoPixel (WS2812B) RGB LED

USB cable and jumper wires

## Wire as documented in the report and diagrams:

DHT11:

VCC - 3.3 V (ESP32-C3)

GND - GND

DATA - GPIO 4

NeoPixel:

DIN - GPIO 5 

VCC - 5 V (from ESP32 USB input)

GND - GND (shared with ESP32)

2. Run Mosquitto (Local MQTT Broker)

Install Mosquitto.

Start the broker from the terminal:

mosquitto -v

This runs the broker on localhost port 1883.

3. Start Node-RED

Start Node-RED from the terminal:

node-red

Then open the editor in a browser:

http://localhost:1880

Import the Node-RED Flow

Open node-red/flow.json from this repository.

In Node-RED, click menu > Import > Paste the JSON > Import.

Confirm that:

MQTT nodes are configured to:

Server: localhost

Port: 1883

4. Deploy the flow.

5. Configure MongoDB

Ensure MongoDB is running locally.

In Node-RED, configure the MongoDB node to point to your local instance, for example:

mongodb://localhost:27017

6. Flash the ESP32-C3

Open the Arduino IDE.

Load code/main.ino from this repository.

Select the correct ESP32-C3 board and COM port.

Update Wi-Fi SSID and password in the sketch if required.

Upload the sketch to the ESP32-C3.

On successful upload and startup, the ESP32-C3 should:

Connect to your Wi-Fi.

Connect to the local MQTT broker.

Publish sensor readings periodically.

Subscribe to control topic(s) for LED and mode updates.

7. Run and Verify

Open the Node-RED dashboard 

Confirm:

Live temperature and humidity values are displayed.

The LED responds correctly in automatic mode (based on thresholds).

Manual mode and control commands from the dashboard are received by the ESP32-C3.

Data is logged into MongoDB.

## Notes

All components (MQTT broker, Node-RED, MongoDB) were run locally for this implementation.

No external cloud MQTT services or remote dashboards were used.

Security (TLS/authentication) was not enabled in this prototype

## References

This work was completed as part of an academic project titled:  
**“ESP32-Based Environmental Monitoring System with Cloud Analytics and Bi-Directional Control.”**
