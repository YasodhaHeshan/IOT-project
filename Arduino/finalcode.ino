#define BLYNK_TEMPLATE_ID "TMPL6CFkTD2qI"
#define BLYNK_TEMPLATE_NAME "SmartGreenHouse"
#define BLYNK_AUTH_TOKEN "YdQgEAv43ROLYtPhmbDO2Lzj1XWB0cvA"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "DHT.h"
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h> // Provide token generation functions
#include <addons/RTDBHelper.h>  // Provide RTDB helper functions

// DHT22 Sensor Configuration
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// GPIO Definitions
#define RELAYPIN_FAN 5
#define SOIL_MOISTURE_AO 34
#define SOIL_MOISTURE_DO 23
#define RELAYPIN_PUMP 26
#define LDR_AO 35
#define LDR_DO 22
#define LED_PIN 27
#define MOTION_SENSOR_PIN 21
#define BUZZER_PIN 19

// Firebase Configuration
#define FIREBASE_HOST "https://smartgreenhouse-84f9c-default-rtdb.asia-southeast1.firebasedatabase.app/"
#define FIREBASE_AUTH "AIzaSyB3hN_V9MB84y4NZ70YQkOAPPJ4wkVtM-g"
FirebaseData firebaseData;
FirebaseAuth auth;
FirebaseConfig config;

// WiFi Credentials
char ssid[] = "HUAWEI Y7a";
char pass[] = "12345678";

// Device States for App Control
bool fanManual = false;
bool pumpManual = false;
bool ledManual = false;
bool buzzerManual = false;

// Blynk Virtual Pins
#define FAN_VPIN V1
#define PUMP_VPIN V0
#define LED_VPIN V2
#define BUZZER_VPIN V3
#define TEMP_VPIN V4
#define HUMIDITY_VPIN V5
#define SOIL_MOISTURE_VPIN V6
#define LIGHT_INTENSITY_VPIN V7

void setup() {
  Serial.begin(9600);
  Serial.println("Blynk Smart Greenhouse");

  // Initialize WiFi
  WiFi.begin(ssid, pass);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected!");

  // Initialize Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Firebase Configuration
  config.host = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  if (Firebase.ready()) {
    Serial.println("Connected to Firebase!");
  } else {
    Serial.println("Failed to connect to Firebase!");
  }

  // Initialize Sensors and Outputs
  dht.begin();
  pinMode(RELAYPIN_FAN, OUTPUT);
  digitalWrite(RELAYPIN_FAN, HIGH);

  pinMode(RELAYPIN_PUMP, OUTPUT);
  digitalWrite(RELAYPIN_PUMP, HIGH);

  pinMode(SOIL_MOISTURE_DO, INPUT);
  pinMode(LDR_DO, INPUT);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  pinMode(MOTION_SENSOR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
}

// Blynk Button Handlers
BLYNK_WRITE(FAN_VPIN) {
  fanManual = param.asInt();
  digitalWrite(RELAYPIN_FAN, fanManual ? LOW : HIGH);
  Serial.println(fanManual ? "Fan Control: Manual ON" : "Fan Control: Manual OFF");
  sendOutputStatesToFirebase();
}

BLYNK_WRITE(PUMP_VPIN) {
  pumpManual = param.asInt();
  digitalWrite(RELAYPIN_PUMP, pumpManual ? LOW : HIGH);
  Serial.println(pumpManual ? "Pump Control: Manual ON" : "Pump Control: Manual OFF");
  sendOutputStatesToFirebase();
}

BLYNK_WRITE(LED_VPIN) {
  ledManual = param.asInt();
  digitalWrite(LED_PIN, ledManual ? HIGH : LOW);
  Serial.println(ledManual ? "LED Control: Manual ON" : "LED Control: Manual OFF");
  sendOutputStatesToFirebase();
}

BLYNK_WRITE(BUZZER_VPIN) {
  buzzerManual = param.asInt();
  digitalWrite(BUZZER_PIN, buzzerManual ? HIGH : LOW);
  Serial.println(buzzerManual ? "Buzzer Control: Manual ON" : "Buzzer Control: Manual OFF");
  sendOutputStatesToFirebase();
}

void loop() {
  Blynk.run();
  delay(2000); // Delay between readings

  // --- Temperature and Humidity ---
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
  } else {
    Blynk.virtualWrite(TEMP_VPIN, temperature);
    Blynk.virtualWrite(HUMIDITY_VPIN, humidity);

    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" *C");

    if (!fanManual) {
      if (temperature > 35 || humidity > 80) {
        digitalWrite(RELAYPIN_FAN, LOW);
        Blynk.virtualWrite(FAN_VPIN, 1);
        Serial.println("Conditions met: Turning ON the fan.");
      } else {
        digitalWrite(RELAYPIN_FAN, HIGH);
        Blynk.virtualWrite(FAN_VPIN, 0);
        Serial.println("Conditions not met: Turning OFF the fan.");
      }
    }

    // --- Soil Moisture ---
    int soilMoistureValue = analogRead(SOIL_MOISTURE_AO);
    int soilDry = digitalRead(SOIL_MOISTURE_DO);
    Blynk.virtualWrite(SOIL_MOISTURE_VPIN, soilMoistureValue);

    Serial.print("Soil Moisture (Analog): ");
    Serial.println(soilMoistureValue);
    Serial.print("Soil Moisture (Digital): ");
    Serial.println(soilDry ? "Dry" : "Wet");

    if (!pumpManual) {
      if (soilDry == HIGH) {
        digitalWrite(RELAYPIN_PUMP, LOW);
        Blynk.virtualWrite(PUMP_VPIN, 1);
        Serial.println("Soil is dry: Turning ON the water pump.");
      } else {
        digitalWrite(RELAYPIN_PUMP, HIGH);
        Blynk.virtualWrite(PUMP_VPIN, 0);
        Serial.println("Soil is wet: Turning OFF the water pump.");
      }
    }

    // --- LDR Sensor ---
    int ldrValue = analogRead(LDR_AO);
    int isDark = digitalRead(LDR_DO);
    Blynk.virtualWrite(LIGHT_INTENSITY_VPIN, ldrValue);

    Serial.print("LDR Value (Analog): ");
    Serial.println(ldrValue);
    Serial.print("LDR Value (Digital): ");
    Serial.println(isDark ? "Dark" : "Bright");

    if (!ledManual) {
      if (isDark == HIGH) {
        digitalWrite(LED_PIN, HIGH);
        Blynk.virtualWrite(LED_VPIN, 1);
        Serial.println("It's dark: Turning ON the LED.");
      } else {
        digitalWrite(LED_PIN, LOW);
        Blynk.virtualWrite(LED_VPIN, 0);
        Serial.println("It's bright: Turning OFF the LED.");
      }
    }

    // Send all the sensor data and output states to Firebase
    sendSensorDataToFirebase(temperature, humidity, soilMoistureValue, ldrValue, 
                             digitalRead(RELAYPIN_FAN) == LOW, 
                             digitalRead(RELAYPIN_PUMP) == LOW, 
                             digitalRead(LED_PIN) == HIGH, 
                             digitalRead(BUZZER_PIN) == HIGH);
  }

  // --- Motion Sensor and Buzzer ---
  int motionDetected = digitalRead(MOTION_SENSOR_PIN);
  Serial.print("Motion Sensor: ");
  Serial.println(motionDetected ? "Motion Detected!" : "No Motion");
}

void sendSensorDataToFirebase(float temperature, float humidity, int soilMoistureValue, int ldrValue, bool fanState, bool pumpState, bool ledState, bool buzzerState) {
  // Create a JSON object to store all the data
  FirebaseJson json;

  // Add sensor data to the JSON object
  json.set("temperature", temperature);
  json.set("humidity", humidity);
  json.set("soilMoisture", soilMoistureValue);
  json.set("lightIntensity", ldrValue);

  // Add output device states to the JSON object
  json.set("fan", fanState);
  json.set("pump", pumpState);
  json.set("led", ledState);
  json.set("buzzer", buzzerState);

  // Send the JSON object to Firebase
  if (Firebase.RTDB.setJSON(&firebaseData, "/sensor_data", &json)) {
    Serial.println("All sensor and output data sent to Firebase!");
  } else {
    Serial.println("Failed to send data to Firebase.");
    Serial.println("Error: " + firebaseData.errorReason());
  }
}

// Function to send output states to Firebase
void sendOutputStatesToFirebase() {
  FirebaseJson json;
  json.set("fan", digitalRead(RELAYPIN_FAN) == LOW);
  json.set("pump", digitalRead(RELAYPIN_PUMP) == LOW);
  json.set("led", digitalRead(LED_PIN) == HIGH);
  json.set("buzzer", digitalRead(BUZZER_PIN) == HIGH);

  if (Firebase.RTDB.setJSON(&firebaseData, "/output_states", &json)) {
    Serial.println("Output states sent to Firebase!");
  } else {
    Serial.println("Failed to send output states to Firebase.");
    Serial.println("Error: " + firebaseData.errorReason());
  }
}
