#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

HTTPClient http;
const int httpPort = 80;

const char *ssid = "MySSID";
const char *password = "TheBestPasswordEverCreated";
const String host = "http://192.168.43.237/temp";

int value = 0;
String urlToPing="";

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
}

void loop()
{
    ArduinoOTA.handle();
    HTTPClient http;

    urlToPing=host + "?val=" + String(value);
    Serial.println(urlToPing);
    http.begin(urlToPing.c_str());
    http.addHeader("Content-Type", "text/plain");
    
    Serial.println(http.GET());
    
    http.end();  //Close connection
    
    value+=1;
    delay(5000);
}
