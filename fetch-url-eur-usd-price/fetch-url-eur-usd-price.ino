
// Inclusão da(s) biblioteca(s)
#include <WiFi.h>       // Biblioteca nativa do ESP32
#include <HTTPClient.h> // Biblioteca nativa do ESP32
#include <ArduinoJson.h>

// Configurações da rede WiFi à se conectar
const char* ssid = "";
const char* password = "";

String payload;

HTTPClient http; // o objeto da classe HTTPClient

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

  // Inicia Serial
  Serial.begin(115200);
  Serial.println();
  delay(1000);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  delay(2000);
  // Clear the buffer.
  display.clearDisplay();

  WiFi.disconnect(); // Desconecta do WiFI se já houver alguma conexão
  WiFi.mode(WIFI_STA); // Configura o ESP32 para o modo de conexão WiFi Estação
  Serial.println("[SETUP] Tentando conexão com o WiFi...");
  WiFi.begin(ssid, password); // Conecta-se à rede
  if (WiFi.waitForConnectResult() == WL_CONNECTED) // aguarda até que o módulo se
    //                                                  conecte ao ponto de acesso
  {
    Serial.println("[SETUP] WiFi iniciado com sucesso!");
  } else
  {
    Serial.println("[SETUP] Houve falha na inicialização do WiFi. Reiniciando ESP.");
    ESP.restart();
  }

  http.begin("https://economia.awesomeapi.com.br/json/USD-EUR/1"); // configura o URL para fazer requisição no servidor
    // entra em um laço de repetição infinito
  //while (1);
}

void loop() {
  
  currenttime= millis();
  if(currenttime - oldtime > 30000){
    oldtime= millis();

    Serial.println("[HTTP] GET...");
    int httpCode = http.GET(); // inicia uma conexão e envia um cabeçalho HTTP para o
    //                              URL do servidor configurado
    Serial.print("[HTTP] GET... código: ");
    Serial.println(httpCode);
    if (httpCode == HTTP_CODE_OK) // se o cabeçalho HTTP foi enviado e o cabeçalho de
      //                               resposta do servidor foi tratado, ...
    {
      Serial.println("[HTTP] GET... OK! Resposta: ");

      payload = http.getString(); // armazena a resposta da requisição
      Serial.println(payload); // imprime a resposta da requisição
    } else // se não, ...
    {
      Serial.print("HTTP GET... Erro. Mensagem de Erro: ");
      Serial.println(http.errorToString(httpCode).c_str()); // Imprime a mensagem de erro da requisição
    }

    http.end();// Fecha a requisição HTTP

  
    JsonDocument remotedata;

    

    payload.replace("[", "");
    payload.replace("]", "");

    DeserializationError error = deserializeJson(remotedata, (char*) payload.c_str());

    
    float conversion= remotedata["bid"];
    Serial.println(conversion, 4);

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println("EUR to USD");
    display.setCursor(15,10);
    display.println("One USD buys ");
    display.setCursor(15,20);
    display.println(conversion, 4);
    display.setCursor(50,20);
    display.println(" Euros");
    display.display();
  }

}
