
#include <WiFi.h>       
#include <HTTPClient.h> 
#include <ArduinoJson.h>

// WiFi credentials
const char* ssid = "";
const char* password = "";

String payload;

HTTPClient http; // HTTPClient's object

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET -1
Adafruit_SSD1306 display(OLED_RESET);

#define LOGO16_GLCD_HEIGHT 64 
#define LOGO16_GLCD_WIDTH  128

long currenttime;
long oldtime;

void setup() {

  Serial.begin(115200);
  Serial.println();
  delay(1000);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  delay(2000);
  // Clear the buffer.
  display.clearDisplay();

  WiFi.disconnect(); // First disconnects from WiFi to then connect properly
  WiFi.mode(WIFI_STA); // Station mode
  Serial.println("[SETUP] Trying WiFi connection...");
  WiFi.begin(ssid, password); // Use credentials to connect
  if (WiFi.waitForConnectResult() == WL_CONNECTED) // 
    
  {
    Serial.println("[SETUP] Connected successfully!");
  } else
  {
    Serial.println("[SETUP] Connection failed, restarting ESP32");
    ESP.restart();
  }

  http.begin("https://economia.awesomeapi.com.br/json/USD-EUR/1"); // The URL to fetch currency conversion from
    
}

void loop() {
  
  currenttime= millis();
  if(currenttime - oldtime > 30000){  // enters the code only every 30000 ms (30 seconds)
    oldtime= millis();

    Serial.println("[HTTP] GET...");
    int httpCode = http.GET(); 
    
    Serial.print("[HTTP] GET... code: ");
    Serial.println(httpCode);
    if (httpCode == HTTP_CODE_OK)
      
    {
      Serial.println("[HTTP] GET... OK! Answer: ");

      payload = http.getString(); // This is where the magic happens, "payload" receives the newly fetched JSON here
      Serial.println(payload); 
    } else 
    {
      Serial.print("HTTP GET... Error message: ");
      Serial.println(http.errorToString(httpCode).c_str()); 
    }

    http.end();// Close HTPP requisition

  
    JsonDocument remotedata;

    
    // This is also part of the magic. Since AwesomeAPI's JSON comes to us with brackets ("[" and "]") 
    // we have to remove both so that ArduinoJSON can read it
    payload.replace("[", "");
    payload.replace("]", "");

    DeserializationError error = deserializeJson(remotedata, (char*) payload.c_str()); // Here the JSON is deserialized

    
    float conversion= remotedata["bid"]; // We want to read the "bid" price within the string received
    Serial.println(conversion, 4);

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println("EUR to USD");
    display.setCursor(15,10);
    display.println("One USD buys ");
    display.setCursor(15,20);
    display.println(conversion, 4); // Showing converted value on screen
    display.setCursor(50,20);
    display.println(" Euros");
    display.display();
  }

}
