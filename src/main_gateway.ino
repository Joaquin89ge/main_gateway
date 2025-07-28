
// main.cpp
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "node_identity.h"
#include "radio_manager.h"
#include "app_logic.h"
#include "rtc_manager.h"
#include "config.h"
#include <ESP8266WiFi.h>

NodeIdentity* identity = nullptr;
RadioManager* radio = nullptr;
AppLogic* logic = nullptr;
RtcManager* rtc = nullptr;
bool errorFlag = false;

void setup()
{
    Serial.begin(115200);
    delay(100);
    
    // Inicialización segura de memoria
    ESP.getFreeHeap(); // Forzar inicialización del heap
    
    // Verificar que la memoria está en un estado válido
    if (ESP.getFreeHeap() < 1024) {
        Serial.println("ERROR: Memoria insuficiente al inicio");
        ESP.restart();
    }
    
    // Mostrar información de memoria disponible
    Serial.printf("Memoria libre: %d bytes\n", ESP.getFreeHeap());
    Serial.printf("Tamaño de sketch: %d bytes\n", ESP.getSketchSize());
    Serial.printf("Tamaño máximo de sketch: %d bytes\n", ESP.getFreeSketchSpace());
    
    WiFi.mode(WIFI_OFF); // Evita interacciones con radio WiFi

    identity = new NodeIdentity();
    if (identity == nullptr) {
        Serial.println("ERROR: No se pudo crear NodeIdentity");
        errorFlag = true;
    } else {
        radio = new RadioManager(identity->getNodeID());
        if (radio == nullptr) {
            Serial.println("ERROR: No se pudo crear RadioManager");
            errorFlag = true;
        } else if (!radio->init()) {
            DEBUG_PRINTLN("Error al inicializar RadioManager");
            errorFlag = true; // Detener ejecución
        }
    }
    
    // Inicializar RTC DS1307 (usa I2C, no necesita pines específicos)
    rtc = new RtcManager();
    if (!rtc->begin())
    {
        DEBUG_PRINTLN("Error al inicializar RTC DS1307");
        errorFlag = true; // Detener ejecución
    }
    
    logic = new AppLogic(*identity, *radio, *rtc);
    if (logic == nullptr) {
        Serial.println("ERROR: No se pudo crear AppLogic");
        errorFlag = true;
    } else {
        logic->begin();
        if (errorFlag==false) DEBUG_PRINT("todo ok en gateway");
    }
    

}

void loop()
{
  
            logic->update();
        
        
    
}
