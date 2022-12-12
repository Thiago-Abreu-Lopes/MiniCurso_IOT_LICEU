#include <ESP8266WiFi.h>   
//#include <WiFi.h>
#include "SinricPro.h"
#include "SinricProSwitch.h"

#define WIFI_SSID //"NOME DA REDE"    
#define WIFI_PASS //"SENHA DA REDE"
#define APP_KEY //"ACESSO DO SINRIC PRO"
#define APP_SECRET //"SENHA DO SINRIC PRO"

#define Abajour_ID //"ID DO ABAJUR"
#define Abajour_Pin 13  // O pino fisico onde está ligado
#define Cafeteira_ID //"ID DA CAFETEIRA"
#define Cafeteira_Pin 12  // O pino fisico onde está ligado

#define BAUD_RATE         9600                // Se precisar, pode trocar o baud rate


void setupWiFi();
void setupSinricPro();
bool AbajourState(const String &deviceId, bool &state);                                                                                                                      
bool CafeteiraState(const String &deviceId, bool &state);


// main setup function
void setup() {
  Serial.begin(BAUD_RATE); Serial.printf("\r\n\r\n");
  setupWiFi();
  setupSinricPro(); 
  pinMode(Abajour_Pin, OUTPUT);
  pinMode(Cafeteira_Pin, OUTPUT);
}

void loop() {
  SinricPro.handle();
}


bool AbajourState(const String &deviceId, bool &state) {
  Serial.printf("O abajour foi %s\r\n", state?"ligado":"desligado");
  digitalWrite(Abajour_Pin, state);
  return true; // request handled properly
}

bool CafeteiraState(const String &deviceId, bool &state) {
  Serial.printf("O cafeteira foi %s\r\n", state?"ligado":"desligado");
  digitalWrite(Cafeteira_Pin, state);
  return true; // request handled properly
}

// setup das conexões Wifi
void setupWiFi() {
  Serial.printf("\r\n[Wifi]: Conectando...");
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.printf(".");
    delay(250);
  }

  Serial.printf("Conectado!\r\n[WiFi]: Endereço de IP e %s\r\n", WiFi.localIP().toString().c_str());
}

// setup das funções para o SinricPro
void setupSinricPro() {
  // add devices and callbacks to SinricPro
  SinricProSwitch &mySwitch1 = SinricPro[Abajour_ID];
  mySwitch1.onPowerState(AbajourState);

  SinricProSwitch &mySwitch2 = SinricPro[Cafeteira_ID];
  mySwitch2.onPowerState(CafeteiraState);

  // setup SinricPro
  SinricPro.onConnected([](){ Serial.printf("Conectado a nuvem SinricPro\r\n"); }); 
  SinricPro.onDisconnected([](){ Serial.printf("Desconectado a nuvem SinricPro\r\n"); });
  SinricPro.begin(APP_KEY, APP_SECRET);
}
