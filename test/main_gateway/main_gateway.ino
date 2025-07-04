
// main.cpp
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "node_identity.h"
#include "radio_manager.h"
#include "app_logic.h"


NodeIdentity identity;

RadioManager radio(identity.getNodeID());

AppLogic logic(identity, radio);


void setup()
{
    Serial.begin(115200);
    delay(100);
    WiFi.mode(WIFI_OFF); // Evita interacciones con radio WiFi


    if (!radio.init())
    {
        Serial.println("Error al inicializar RadioManager");
        
    }

    logic.begin();
    Serial.print("todo ok GATEWAY");
}

void loop()
{
    logic.update();
}
