/******************************************************************************
                    Cotação do Dólar em Tempo Real com ESP32
                   Sketch Auxiliar: Exemplo de Requisição GET

                          Criado em 30 de Maio de 2022
                                por Michel Galvão

  Eletrogate - Loja de Arduino \\ Robótica \\ Automação \\ Apostilas \\ Kits
                            https://www.eletrogate.com/
******************************************************************************/

// Inclusão da(s) biblioteca(s)
#include <WiFi.h>       // Biblioteca nativa do ESP32
#include <HTTPClient.h> // Biblioteca nativa do ESP32
#include <ArduinoJson.h>

// Configurações da rede WiFi à se conectar
const char* ssid = "Clovis 2.4G";
const char* password = "99143304";

JsonDocument doc;
String payload;

void setup() {

  // Inicia Serial
  Serial.begin(115200);
  Serial.println();
  delay(1000);

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

  HTTPClient http; // o objeto da classe HTTPClient

  Serial.println("[HTTP] começar...");
  http.begin("https://economia.awesomeapi.com.br/json/USD-EUR/1"); // configura o URL para fazer requisição no servidor

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

  // entra em um laço de repetição infinito
  //while (1);
}

void loop() {
  
  deserializeJson(doc, payload);
  String cotacao= doc["bid"];
  Serial.println(cotacao);

delay(1000);
}