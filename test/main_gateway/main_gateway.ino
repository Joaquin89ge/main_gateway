
// main.cpp
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "node_identity.h"
#include "radio_manager.h"
#include "app_logic.h"
#include "rtc_manager.h"
#include "config.h"

NodeIdentity* identity = nullptr;
RadioManager* radio = nullptr;
AppLogic* logic = nullptr;
RtcManager* rtc = nullptr;
bool errorFlag = false;

void setup()
{
    Serial.begin(115200);
    delay(100);
    WiFi.mode(WIFI_OFF); // Evita interacciones con radio WiFi

    identity = new NodeIdentity();
    radio = new RadioManager(identity->getNodeID());
    if (!radio->init())
    {
        DEBUG_PRINTLN("Error al inicializar RadioManager");
        errorFlag = true; // Detener ejecución
    }
    
    // Inicializar RTC con los pines definidos en config.h
    rtc = new RtcManager(RTC_DAT, RTC_CLK, RTC_RST);
    if (!rtc->begin())
    {
        DEBUG_PRINTLN("Error al inicializar RTC");
        errorFlag = true; // Detener ejecución
    }
    
    logic = new AppLogic(*identity, *radio, *rtc);
    logic->begin();
    if (errorFlag==false) DEBUG_PRINT("todo ok en gateway");
}

void loop()
{
    if (1 == 1)
    {
        logic->update();
        
    }
}
