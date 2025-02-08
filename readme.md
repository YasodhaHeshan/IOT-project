# Smart Greenhouse System (SGS)

This IoT-based smart greenhouse system provides automated monitoring and control of environmental conditions for optimal plant growth. It leverages sensors, cloud computing, and IoT technologies to maintain ideal growing conditions while offering remote monitoring and control capabilities.

## Project Goals

* Real-time monitoring of greenhouse environmental conditions
* Automated control of temperature, humidity, and irrigation
* Data logging and analysis for crop optimization
* Remote management via mobile application
* Energy-efficient operation and resource management

## Core Features

* **Environmental Monitoring:** Real-time tracking of temperature, humidity, soil moisture, light levels, and CO2 through integrated sensors
* **Automated Climate Control:** Smart control of ventilation fans, heating systems, and shade screens to maintain optimal growing conditions
* **Intelligent Irrigation System:** Automated watering based on soil moisture levels and customizable schedules
* **Cloud-Based Data Management:** Stores environmental data and system logs in Firebase for analysis and monitoring
* **Mobile Application Interface:** User-friendly app for monitoring conditions, adjusting settings, and receiving alerts
* **Smart Light Management:** Automated LED grow lights that adjust based on natural light levels
* **Resource Optimization:** AI-driven algorithms to minimize water and energy usage while maximizing crop yield

## System Architecture

The system consists of the following components:

* **Sensor Network:** DHT22 (temperature/humidity), soil moisture sensors, light sensors, CO2 sensors
* **Control Units:** Relay modules for fans, pumps, lights, and heating systems
* **Main Controller:** NodeMCU/ESP32 for local processing and WiFi connectivity
* **Cloud Platform:** Firebase for data storage and real-time monitoring
* **Mobile Application:** Cross-platform app for system control and monitoring
* **Actuators:** Ventilation fans, water pumps, LED grow lights, heating elements

## Software and Technologies

* **Programming Languages:** C++ (Arduino), Python (data processing), Flutter (mobile app)
* **Cloud Services:** Firebase (Database, Authentication, Cloud Functions)
* **IoT Protocols:** MQTT for device communication
* **Development Tools:** Arduino IDE, VS Code, Android Studio
* **Libraries:** DHT sensor library, Firebase ESP client, PubSubClient

## Key Features

* **Automated Climate Control:** Maintains optimal temperature and humidity levels
* **Smart Irrigation:** Waters plants based on soil moisture and schedules
* **Data Analytics:** Tracks and analyzes growing conditions over time
* **Remote Monitoring:** Access system status and controls from anywhere
* **Alert System:** Notifications for abnormal conditions or system issues

## Blynk App Setup

To set up the Blynk app for the Smart Greenhouse System, follow these steps:

1. **Download the Blynk App:**
   - Install the Blynk app from the [App Store](https://apps.apple.com/us/app/blynk-iot/id808760481) or [Google Play](https://play.google.com/store/apps/details?id=cc.blynk).

2. **Create a New Project:**
   - Open the Blynk app and create a new project.
   - Enter the project name (e.g., "Smart Greenhouse").
   - Select the device as "ESP32".
   - Choose the connection type as "WiFi".
   - Click "Create".

3. **Add Widgets:**
   - Add the following widgets to your project:
     - **Button:** For Fan Control (Virtual Pin V1)
     - **Button:** For Pump Control (Virtual Pin V0)
     - **Button:** For LED Control (Virtual Pin V2)
     - **Button:** For Buzzer Control (Virtual Pin V3)
     - **Gauge:** For Temperature Display (Virtual Pin V4)
     - **Gauge:** For Humidity Display (Virtual Pin V5)
     - **Gauge:** For Soil Moisture Display (Virtual Pin V6)
     - **Gauge:** For Light Intensity Display (Virtual Pin V7)

4. **Configure Widgets:**
   - For each button widget, set the mode to "Switch".
   - For each gauge widget, set the appropriate range based on the sensor data.

5. **Auth Token:**
   - After creating the project, an Auth Token will be sent to your registered email.
   - Replace the `BLYNK_AUTH_TOKEN` in the Arduino code with this Auth Token.

6. **Upload the Code:**
   - Upload the Arduino code to your ESP32 device.

7. **Run the Project:**
   - Ensure your ESP32 is connected to WiFi.
   - Open the Blynk app and start the project.
   - You should now be able to monitor and control your Smart Greenhouse system remotely.

This setup allows you to interact with your Smart Greenhouse system using the Blynk app, providing real-time monitoring and control capabilities.

## Future Enhancements

* Machine learning for crop-specific optimization
* Computer vision for plant health monitoring
* Weather API integration for predictive control
* Expandable zones for different plant types
* Automated nutrient dosing system
* Integration with other smart farming systems

This project aims to provide an efficient and scalable solution for modern greenhouse management, combining IoT technology with sustainable farming practices.