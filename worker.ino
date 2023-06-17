#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <DHT.h>

#define DHTPIN 5     
#define DHTTYPE DHT22   

DHT dht(DHTPIN, DHTTYPE);
WiFiClient wifiClient;

// Replace with your network credentials
const char* ssid = "Bees";
const char* password = "password";

void setup() {
  // Start serial communication
  Serial.begin(9600);
  dht.begin();
  Serial.printf("Wi-Fi mode set to WIFI_STA %s\n", WiFi.mode(WIFI_STA) ? "" : "Failed!"); // some weird WiFi problems with this ESP, verify it's in station mode
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi... ");
  }

  // Print ESP8266 IP address
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    float temperature = dht.readTemperature(true);
    float humidity = dht.readHumidity();

    Serial.println("Sent to server: Temperature " + String(temperature));
    Serial.println("Sent to server: Humidity " + String(humidity));

    HTTPClient http;
    Serial.println(http.begin(wifiClient, "http://192.168.0.85:3000/hive/?temperature=" + String(temperature) + "&humidity=" + String(humidity)));
    http.GET();
    http.end();

    ESP.deepSleep(3.6e9); // sleep for an hour before reporting back again
  }
}
