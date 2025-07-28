/**
 * @file test_rtc_ds1307.ino
 * @brief Test simple para verificar RTC DS1307
 * @date 2025
 */

#include <Arduino.h>
#include "rtc_manager.h"

RtcManager rtc;

void setup() {
    Serial.begin(115200);
    delay(1000);
    
    Serial.printf("=== Test RTC DS1307 ===\n");
    
    // Inicializar RTC
    if (!rtc.begin()) {
        Serial.printf("ERROR: Fallo en inicialización del RTC\n");
        return;
    }
    
    Serial.printf("✓ RTC inicializado correctamente\n");
    
    // Test de funciones básicas
    DateTime now = rtc.getDateTime();
    Serial.printf("Fecha/hora actual: ");
    rtc.printDateTime(now);
    Serial.printf("\n");
    
    String timeStr = rtc.getTimeString();
    Serial.printf("Hora en formato string: %s\n", timeStr.c_str());
    
    // Test de comparación de horas
    Serial.printf("\n=== Test comparación de horas ===\n");
    
    String testTime1 = "12:30";
    String testTime2 = "12:30";
    String testTime3 = "13:45";
    
    bool result1 = rtc.compareHsAndMs(testTime1, testTime2);
    bool result2 = rtc.compareHsAndMs(testTime1, testTime3);
    
    Serial.printf("Comparando '%s' con '%s': %s\n", 
                  testTime1.c_str(), testTime2.c_str(), 
                  result1 ? "IGUAL" : "DIFERENTE");
    Serial.printf("Comparando '%s' con '%s': %s\n", 
                  testTime1.c_str(), testTime3.c_str(), 
                  result2 ? "IGUAL" : "DIFERENTE");
    
    Serial.printf("\n=== Test completado ===\n");
}

void loop() {
    // Mostrar hora cada 30 segundos
    static unsigned long lastTime = 0;
    if (millis() - lastTime > 30000) {
        String currentTime = rtc.getTimeString();
        Serial.printf("Hora actual: %s\n", currentTime.c_str());
        lastTime = millis();
    }
    
    delay(100);
} 