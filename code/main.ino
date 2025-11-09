#include <WiFi.h>
#include <Adafruit_NeoPixel.h>    
#include <DHT.h>                
#include <ArduinoJson.h>          
#include <PubSubClient.h>


#define RGB_PIN 5
#define DHT_PIN 4


#define DHTTYPE DHT11


#define NEOPIXEL_TYPE NEO_GRB + NEO_KHZ800


#define DEF_ALARM_COLD 0.0
#define DEF_WARN_COLD 10.0
#define DEF_WARN_HOT  25.0
#define DEF_ALARM_HOT 30.0


#define UNIQUE_ID "18138"
#define MQTT_HOST "localhost"
#define MQTT_PORT 1883
#define MQTT_DEVICEID "d:hwu:esp32:18138"
#define MQTT_USER ""
#define MQTT_TOKEN ""


#define MQTT_TOPIC_TELEMETRY  "18138/evt/status/fmt/json"     // ESP32 -> Node-RED (sensor data)
#define MQTT_TOPIC_DISPLAY    "18138/cmd/display/fmt/json"    // Node-RED -> ESP32 (control)
#define MQTT_TOPIC_INTERVAL   "18138/cmd/interval/fmt/json"   // Node-RED -> ESP32 (reporting interval)
#define MQTT_TOPIC_STATE      "18138/evt/state/fmt/json"      // ESP32 -> Node-RED (current settings/state)

unsigned long publishInterval = 60000;
unsigned long lastPublish     = 0;


char ssid[] = "";
char pass[] = "";


Adafruit_NeoPixel pixel = Adafruit_NeoPixel(1, RGB_PIN, NEOPIXEL_TYPE);
DHT dht(DHT_PIN, DHTTYPE);

WiFiClient wifiClient;
PubSubClient mqtt(wifiClient);


StaticJsonDocument<256> jsonDoc;

float h = 0.0;
float t = 0.0;


float alarmCold = DEF_ALARM_COLD;
float warnCold  = DEF_WARN_COLD;
float warnHot   = DEF_WARN_HOT;
float alarmHot  = DEF_ALARM_HOT;

String mode = "auto";        // "auto", "manual", "off"
String manualColor = "off";  // used when mode == "manual"

unsigned char r = 0, g = 0, b = 0;

void callback(char* topic, byte* payload, unsigned int length);
void reconnect();
void applyLedLogic();
void setLedColorByName(const String &colorName);
void publishState();


void setLedRGB(uint8_t rr, uint8_t gg, uint8_t bb) {
  pixel.setPixelColor(0, rr, gg, bb);
  pixel.show();
}


void setLedColorByName(const String &colorName) {
  if (colorName == "red")         setLedRGB(255, 0, 0);
  else if (colorName == "green")  setLedRGB(0, 255, 0);
  else if (colorName == "blue")   setLedRGB(0, 0, 255);
  else if (colorName == "yellow") setLedRGB(255, 255, 0);
  else if (colorName == "purple") setLedRGB(128, 0, 128);
  else if (colorName == "off")    setLedRGB(0, 0, 0);
  else                            setLedRGB(0, 0, 0); // default off
}


void applyLedLogic() {
  if (mode == "off") {
    setLedRGB(0, 0, 0);
    return;
  }

  if (mode == "manual") {
    setLedColorByName(manualColor);
    return;
  }

  // mode == "auto": use temperature bands
  // Safety: if t is NaN, turn off
  if (isnan(t)) {
    setLedRGB(0, 0, 0);
    return;
  }

  // Reset
  r = g = b = 0;

  // BLUE: cold
  if (t < alarmCold) {
    b = 255;
  } else if (t < warnCold) {
    b = 150;
  }

  // RED: hot
  if (t >= alarmHot) {
    r = 255;
  } else if (t > warnHot) {
    r = 150;
  }

  // GREEN: comfortable/normal
  if (t > alarmCold && t <= warnHot) {
    g = 255;
  } else if (t > warnHot && t < alarmHot) {
    g = 150;
  }

  setLedRGB(r, g, b);
}


void publishState() {
  StaticJsonDocument<256> doc;
  JsonObject root = doc.to<JsonObject>();

  root["mode"]       = mode;
  root["color"]      = manualColor;
  root["alarmCold"]  = alarmCold;
  root["warnCold"]   = warnCold;
  root["warnHot"]    = warnHot;
  root["alarmHot"]   = alarmHot;
  root["intervalMs"] = publishInterval;

  char buf[256];
  size_t n = serializeJson(doc, buf, sizeof(buf));
  mqtt.publish(MQTT_TOPIC_STATE, buf, n);
}


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] : ");

  if (length > 255) length = 255;
  char msg[256];
  memcpy(msg, payload, length);
  msg[length] = '\0';

  Serial.println(msg);

  if (String(topic) == MQTT_TOPIC_INTERVAL) {
    StaticJsonDocument<128> doc;
    DeserializationError err = deserializeJson(doc, msg);
    if (err) {
      Serial.print("Interval JSON parse error: ");
      Serial.println(err.c_str());
      return;
    }

    if (doc.containsKey("interval_sec")) {
      long sec = doc["interval_sec"];
      if (sec < 5) sec = 5;
      if (sec > 3600) sec = 3600;
      publishInterval = (unsigned long)sec * 1000UL;

      Serial.print("Updated reporting interval to ");
      Serial.print(sec);
      Serial.println(" seconds");

      publishState();
    }
    return;
  }

  if (String(topic) == MQTT_TOPIC_DISPLAY) {
    StaticJsonDocument<256> doc;
    DeserializationError err = deserializeJson(doc, msg);
    if (err) {
      Serial.print("Display JSON parse error: ");
      Serial.println(err.c_str());
      return;
    }

    bool changed = false;

    // Mode: "auto", "manual", "off"
    if (doc.containsKey("mode")) {
      String newMode = doc["mode"].as<String>();
      newMode.toLowerCase();
      if (newMode == "auto" || newMode == "manual" || newMode == "off") {
        mode = newMode;
        Serial.print("Mode set to: ");
        Serial.println(mode);
        changed = true;
      }
    }

    // Manual color (used only if mode == manual)
    if (doc.containsKey("color")) {
      manualColor = doc["color"].as<String>();
      manualColor.toLowerCase();
      Serial.print("Manual color set to: ");
      Serial.println(manualColor);
      changed = true;
    }

   
    if (doc.containsKey("alarmCold")) {
      alarmCold = doc["alarmCold"].as<float>();
      changed = true;
    }
    if (doc.containsKey("warnCold")) {
      warnCold = doc["warnCold"].as<float>();
      changed = true;
    }
    if (doc.containsKey("warnHot")) {
      warnHot = doc["warnHot"].as<float>();
      changed = true;
    }
    if (doc.containsKey("alarmHot")) {
      alarmHot = doc["alarmHot"].as<float>();
      changed = true;
    }

    if (changed) {
      // Apply new logic immediately using last known temperature
      applyLedLogic();
      publishState();
    }
  }
}

void reconnect() {
  while (!mqtt.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (mqtt.connect(MQTT_DEVICEID, MQTT_USER, MQTT_TOKEN)) {
      Serial.println("connected");

      mqtt.subscribe(MQTT_TOPIC_DISPLAY);
      mqtt.subscribe(MQTT_TOPIC_INTERVAL);

      // publish initial state so Node-RED knows current config
      publishState();
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqtt.state());
      Serial.println(" - retry in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(2000);
  while (!Serial) { }

  Serial.println();
  Serial.println("ESP32 Sensor Application (Node-RED Controlled)");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi Connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  dht.begin();
  pixel.begin();
  setLedRGB(0, 0, 0); // start off

  mqtt.setServer(MQTT_HOST, MQTT_PORT);
  mqtt.setCallback(callback);

  reconnect();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    // simple reconnect (can be improved if needed)
    WiFi.begin(ssid, pass);
  }

  if (!mqtt.connected()) {
    reconnect();
  }
  mqtt.loop();

  unsigned long now = millis();
  if (now - lastPublish >= publishInterval) {
    lastPublish = now;

    h = dht.readHumidity();
    t = dht.readTemperature(); // Celsius

    if (isnan(h) || isnan(t)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

    // Apply LED behavior based on latest temp + current mode/settings
    applyLedLogic();

    // Build telemetry JSON
    jsonDoc.clear();
    JsonObject root = jsonDoc.to<JsonObject>();
    JsonObject dObj = root.createNestedObject("d");

    dObj["temp"] = t;
    dObj["humidity"] = h;
    dObj["mode"] = mode;

    char msg[128];
    size_t n = serializeJson(jsonDoc, msg, sizeof(msg));

    Serial.print("Publishing telemetry: ");
    Serial.println(msg);

    if (!mqtt.publish(MQTT_TOPIC_TELEMETRY, msg, n)) {
      Serial.println("MQTT Publish failed");
    } else {
      Serial.println("MQTT Publish OK");
    }
  }
}
