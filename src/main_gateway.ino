
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
    
    // Inicializar RTC con los pines definidos en config.h
    rtc = new RtcManager(RTC_DAT, RTC_CLK, RTC_RST);
    if (!rtc->begin())
    {
        DEBUG_PRINTLN("Error al inicializar RTC");
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
    
    // Configurar watchdog
    ESP.wdtEnable(WDTO_8S);
    ESP.wdtDisable();
}

void loop()
{
    if (1 == 1)
    {
        // Alimentar watchdog
        ESP.wdtFeed();
        
        // Verificar memoria crítica
        if (ESP.getFreeHeap() < 1024) {
            Serial.println("ADVERTENCIA: Memoria crítica detectada");
            delay(1000); // Dar tiempo para liberar memoria
        }
        
        // Verificar memoria extremadamente baja
        if (ESP.getFreeHeap() < 512) {
            Serial.println("ERROR: Memoria extremadamente baja, reiniciando...");
            ESP.restart();
        }
        
        if (logic != nullptr && !errorFlag) {
            logic->update();
        }
        
        // Monitoreo periódico de memoria
        static unsigned long lastMemoryCheck = 0;
        if (millis() - lastMemoryCheck > 30000) { // Cada 30 segundos
                    Serial.printf("Memoria libre: %d bytes\n", ESP.getFreeHeap());
            lastMemoryCheck = millis();
        }
    }
}
