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

ESP32-Bidirectional-Environmental-Monitoring/
│
├── code/ # ESP32 Arduino sketch
│ └── main.ino
│
├── node-red/ # Exported Node-RED flow (flow.json)
│
├── docs/ # Diagrams and reference images
│
└── README.md


---

## Prerequisites

The system was tested with the following software and environment:

- **Arduino IDE** (legacy version with ESP32 board support)  
- **Mosquitto MQTT Broker** (local installation)  
- **Node-RED** (local installation)  
- **MongoDB Community Server** (local installation)

---

## Setup Instructions

### 1. Hardware Requirements

- ESP32-C3 Development Board  
- DHT11 Temperature and Humidity Sensor  
- Single NeoPixel (WS2812B) RGB LED  
- USB cable and jumper wires  
- Breadboard for connections  

---

### 2. Wiring Connections

| Component | Pin | ESP32-C3 Pin | Description |
|------------|-----|--------------|--------------|
| DHT11 | VCC | 3.3 V | Power supply |
| DHT11 | GND | GND | Common ground |
| DHT11 | DATA | GPIO 4 | Digital signal |
| NeoPixel | DIN | GPIO 5 | LED control signal |
| NeoPixel | VCC | 5 V | Power from ESP32 USB |
| NeoPixel | GND | GND | Shared ground |

---

### 3. Run Mosquitto (Local MQTT Broker)

Install Mosquitto, then start the broker via terminal:

```
mosquitto -v
This runs the broker on localhost port 1883.
```
## 4. Start Node-RED

Launch Node-RED from the terminal:

```
node-red
```

Then open the editor in a browser:
http://localhost:1880

Import Node-RED Flow

Open node-red/flow.json from this repository.

In Node-RED, click Menu > Import > Paste JSON > Import.

Configure MQTT nodes:

Server: localhost

Port: 1883

Deploy the flow once configuration is complete

## 5. Configure MongoDB

Ensure MongoDB is running locally.
In Node-RED, configure the MongoDB node to point to your local instance:

mongodb://localhost:27017

## 6. Flash the ESP32-C3

Open the Arduino IDE.

Load the sketch file:
code/main.ino

Select the correct ESP32-C3 board and COM port.

Update Wi-Fi SSID and password in the sketch if required.

Upload the sketch to the ESP32-C3.

Upon successful upload:

The ESP32-C3 connects to your Wi-Fi.

Connects to the local MQTT broker.

Publishes temperature and humidity readings.

Subscribes to LED and mode control topics.

## 7. Run and Verify

Open the Node-RED dashboard.

Confirm that:

Live temperature and humidity readings are displayed.

The LED responds correctly in automatic mode (based on temperature thresholds).

Manual mode commands from the dashboard update the ESP32-C3.

Data is logged in MongoDB.


**Notes**

All components (MQTT broker, Node-RED, MongoDB) were locally hosted.

No external cloud MQTT services were used.

Security features such as TLS and authentication were not enabled in this prototype.

## References

This work was completed as part of an academic project titled:
“ESP32-Based Environmental Monitoring System with Cloud Analytics and Bi-Directional Control.”

L. D. Xu, W. He, and S. Li, "Internet of Things in Industries: A Survey," IEEE Transactions on Industrial Informatics, vol. 10, no. 4, pp. 2233–2243, 2014.

G. Fortino and P. Trunfio, Internet of Things Based on Smart Objects: Technology, Middleware and Applications. Springer, 2014.

D. Hercog, T. Lerher, M. Trunčič, and O. Težak, "Design and Implementation of ESP32-Based IoT Devices," Sensors, vol. 23, no. 15, p. 6739, 2023.

B. Mishra and A. Kertesz, "The Use of MQTT in M2M and IoT Systems: A Survey," IEEE Access, vol. 8, pp. 201071–201086, 2020.

A. Zare and M. T. Iqbal, "Low-Cost ESP32, Raspberry Pi, Node-Red, and MQTT Protocol Based SCADA System," in Proc. IEEE IEMTRONICS, pp. 1–5.

P. Jupyter, “Jupyter Notebook Documentation,” 2024. [Online]. Available: https://jupyter.org

J. J. Correa-Quiroz, M. A. Toribio-Barrueto, and C. Castro-Vargas, "IoT System with ESP32 for Smart Drip Irrigation and Climate Monitoring in Greenhouses," Emerging Science Journal, vol. 9, no. 3, pp. 1133–1157, 2025.

P. Burgess, Adafruit NeoPixel Best Practices Guide, 2013. [Online]. Available: https://learn.adafruit.com/adafruit-neopixel-uberguide/best-practices
