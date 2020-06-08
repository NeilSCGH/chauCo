#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

//SENSOR
#define DHTPIN 2
#define DHTTYPE DHT11
DHT_Unified dht(DHTPIN, DHTTYPE);

//WIFI & POST
HTTPClient http;
const int httpPort = 80;

const char *ssid = "MySSID";
const char *password = "TheBestPasswordEverCreated";
const String host = "http://192.168.43.237/data";

String urlToPing="";
double temperature=0;
double humidity=0;
const double smoothingFactor=0.5;//x% of the new value, 1-x% of the last computed value

void setup()
{
    Serial.begin(115200);
    Serial.print("Connecting to ");
    Serial.print(ssid);
    
    /*Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default, would try to act as both a client and an access-point and could cause network-issues with your other WiFi-devices on your WiFi-network. */
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.waitForConnectResult() != WL_CONNECTED) {
        delay(5000);
        Serial.print("Wifi connection failed, rebooting...");
        ESP.restart();
    }
    
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    
    ArduinoOTA.setHostname("TheSensor8266");
    ArduinoOTA.setPassword((const char *)"mypassword");
    ArduinoOTA.begin();
    Serial.println("OTA ready");

    dht.begin();
    Serial.println("Sensor ready");
}

void loop()
{
    //WIFI
    ArduinoOTA.handle();
    

    //SENSOR
    sensors_event_t event;
    dht.temperature().getEvent(&event);
    temperature = smoothingFactor * event.temperature + (1-smoothingFactor) * temperature;
    
    dht.humidity().getEvent(&event);
    humidity = smoothingFactor * event.relative_humidity + (1-smoothingFactor) * humidity;
    
    if (isnan(temperature) or isnan(humidity)) {
      Serial.println(F("Error reading sensor!"));
      urlToPing=host + "?error";
    }
    else {
      //SENDING THE TEMPERATURE
      urlToPing=host + "?temp=" + String(temperature) + "&hum=" + String(humidity);
    }
    
    Serial.print(sendGet(urlToPing));
    Serial.println("   " + urlToPing);

    delay(30000);
}

int sendGet(String uri)
{
  HTTPClient http;
  http.begin(uri.c_str());
  http.addHeader("Content-Type", "text/plain");
  http.end();
  return http.GET();
}
