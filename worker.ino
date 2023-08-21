#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <DHT.h>
#include <HX711.h>

#define DHTPIN 5     
#define DHTTYPE DHT22

#define LOADCELL_DOUT 2
#define LOADCELL_SCK 3

DHT dht(DHTPIN, DHTTYPE);
WiFiClient wifiClient;

const float HIVE_WEIGHT = 440317;
const float CONVERSION = -8755.555;

HX711 scale;

const char* ssid = "Bees";
const char* password = "password";

void setup() {
  // Start serial communication
  Serial.begin(9600);
  dht.begin();
  WiFi.mode(WIFI_STA);
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi... ");
  }

  scale.begin(LOADCELL_DOUT, LOADCELL_SCK);

  // Print ESP8266 IP address
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    float temperature = dht.readTemperature(true);
    float humidity = dht.readHumidity();

    float scaleRead = (scale.read_average(40));
    float weight = (HIVE_WEIGHT + scaleRead);

    Serial.println("Sent to server: Temperature " + String(temperature));
    Serial.println("Sent to server: Humidity " + String(humidity));

    HTTPClient http;
    Serial.println(http.begin(wifiClient, "http://192.168.0.85:3000/hive/?temperature=" + String(temperature) + "&humidity=" + String(humidity) + "&weight=" + (weight)/(CONVERSION)));
    http.GET();
    http.end();


    ESP.deepSleep(3.6e+9); // sleep for 1 hour before reporting back again
  }
}
