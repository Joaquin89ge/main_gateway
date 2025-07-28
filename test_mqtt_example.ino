/**
 * @file test_mqtt_example.ino
 * @brief Ejemplo de uso del sistema MQTT en el Gateway
 * @date 2025
 */

#include <Arduino.h>
#include "app_logic.h"
#include "node_identity.h"
#include "radio_manager.h"
#include "rtc_manager.h"

// Instancias de los componentes
NodeIdentity identity;
RadioManager radio(identity.getNodeID());
RtcManager rtc;
AppLogic logic(identity, radio, rtc);

void setup() {
    Serial.begin(115200);
    delay(1000);
    
    Serial.printf("=== Test MQTT Gateway ===\n");
    
    // Inicializar componentes
    if (!rtc.begin()) {
        Serial.printf("ERROR: Fallo en inicialización del RTC\n");
        return;
    }
    
    if (!radio.begin()) {
        Serial.printf("ERROR: Fallo en inicialización de la radio\n");
        return;
    }
    
    logic.begin();
    
    Serial.printf("✓ Sistema inicializado correctamente\n");
    Serial.printf("✓ MQTT configurado para publicar en:\n");
    Serial.printf("  - Tópico atmosférico: %s\n", MQTT_TOPIC_ATMOSPHERIC);
    Serial.printf("  - Tópico suelo: %s\n", MQTT_TOPIC_GROUND);
    Serial.printf("  - Broker: %s:%d\n", MQTT_SERVER, MQTT_PORT);
}

void loop() {
    // Actualizar lógica principal
    logic.update();
    
    // Simular recepción de datos cada 30 segundos
    static unsigned long lastData = 0;
    if (millis() - lastData > 30000) {
        Serial.printf("Simulando recepción de datos...\n");
        
        // Simular datos atmosféricos
        Protocol::AtmosphericSample atmosData;
        atmosData.temp = 250;      // 25.0°C
        atmosData.moisture = 600;  // 60.0%
        atmosData.pressure = 1013;
        atmosData.altitude = 100;
        atmosData.co2 = 400;
        atmosData.tvoc = 50;
        atmosData.hour = 12;
        atmosData.minute = 30;
        
        // Publicar datos atmosféricos simulados
        logic.publishAtmosphericData(0x42, atmosData);
        
        // Simular datos de suelo
        Protocol::GroundGpsPacket groundData;
        groundData.temperature = 220;    // 22.0°C
        groundData.moisture = 450;       // 45.0%
        groundData.n = 15;
        groundData.p = 25;
        groundData.k = 30;
        groundData.ec = 1200;
        groundData.ph = 65;              // 6.5 pH
        groundData.latitude = -34567890; // -34.567890
        groundData.longitude = -58123456; // -58.123456
        
        // Publicar datos de suelo simulados
        logic.publishGroundData(0x42, groundData);
        
        lastData = millis();
    }
    
    delay(100);
} 