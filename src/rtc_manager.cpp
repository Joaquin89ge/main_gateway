/**
 * @file rtc_manager.cpp
 * @brief Implementación del gestor del módulo RTC DS1307
 * @date 2025
 */

#include "rtc_manager.h"

// Constructor
RtcManager::RtcManager() : initialized(false) {
}

// Inicializa el RTC
bool RtcManager::begin() {
    Serial.printf("RtcManager::begin() - Iniciando DS1307\n");
    
    // Inicializar I2C
    Wire.begin();
    Wire.setClock(100000); // 100kHz para compatibilidad
    delay(100);
    
    // Verificar si hay dispositivos I2C conectados
    bool foundDS1307 = false;
    
    for (byte addr = 1; addr < 127; addr++) {
        Wire.beginTransmission(addr);
        byte error = Wire.endTransmission();
        if (error == 0) {
            if (addr == 0x68) {
                foundDS1307 = true;
            }
        }
    }
    
    if (!foundDS1307) {
        Serial.printf("RtcManager::begin() - ERROR: DS1307 no encontrado\n");
        return false;
    }
    
    // Inicializar RTC DS1307
    if (!rtc.begin()) {
        Serial.printf("RtcManager::begin() - ERROR: No se pudo inicializar DS1307\n");
        return false;
    }
    
    // Verificar si el RTC está funcionando
    if (!rtc.isrunning()) {
        Serial.printf("RtcManager::begin() - Configurando RTC con fecha de compilación\n");
        
        // Configurar con fecha de compilación
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
        
        // Verificar que se configuró correctamente
        delay(100);
        if (!rtc.isrunning()) {
            Serial.printf("RtcManager::begin() - ERROR: RTC no responde después de configuración\n");
            return false;
        }
    }
    
    // Obtener fecha/hora actual y verificar
    DateTime now = rtc.now();
    
    // Verificar que la fecha sea válida
    if (now.year() < 2000 || now.year() > 2100) {
        Serial.printf("RtcManager::begin() - ADVERTENCIA: Año fuera de rango válido (%d)\n", now.year());
    }
    
    // Verificar que la hora sea razonable
    if (now.hour() > 23 || now.minute() > 59 || now.second() > 59) {
        Serial.printf("RtcManager::begin() - ERROR: Hora inválida\n");
        return false;
    }
    
    Serial.printf("RtcManager::begin() - DS1307 inicializado correctamente\n");
    
    initialized = true;
    return true;
}

// Obtiene la fecha y hora actual
DateTime RtcManager::getDateTime() {
    if (!initialized) {
        return DateTime(2000, 1, 1, 0, 0, 0);
    }
    
    if (!rtc.isrunning()) {
        return DateTime(2000, 1, 1, 0, 0, 0);
    }
    
    return rtc.now();
}

// Establece la fecha y hora
bool RtcManager::setDateTime(const DateTime& dt) {
    if (!initialized) {
        return false;
    }
    
    rtc.adjust(dt);
    return true;
}

// Verifica si la fecha/hora es válida
bool RtcManager::isDateTimeValid(const DateTime& dt) {
    return (dt.year() >= 2000 && dt.year() <= 2100);
}

// Obtiene solo la hora y minutos como string (HH:MM)
String RtcManager::getTimeString() {
    if (!initialized) {
        return "00:00";
    }
    
    // Verificar si el RTC está funcionando
    if (!rtc.isrunning()) {
        return "00:00";
    }
    
    // Obtener fecha/hora
    DateTime now = rtc.now();
    
    // Verificar que la fecha obtenida sea válida
    if (now.year() < 2000 || now.year() > 2100) {
        return "00:00";
    }
    
    return getTimeString(now);
}

// Convierte DateTime a string de hora (HH:MM)
String RtcManager::getTimeString(const DateTime& dt) {
    char timeStr[6];
    sprintf(timeStr, "%02d:%02d", dt.hour(), dt.minute());
    return String(timeStr);
}

// Compara dos horas en formato HH:MM
bool RtcManager::compareHsAndMs(const String& time1, const String& time2) {
    // Extraer horas y minutos de time1
    int colon1 = time1.indexOf(':');
    if (colon1 == -1) {
        return false;
    }
    
    int hour1 = time1.substring(0, colon1).toInt();
    int minute1 = time1.substring(colon1 + 1).toInt();
    
    // Extraer horas y minutos de time2
    int colon2 = time2.indexOf(':');
    if (colon2 == -1) {
        return false;
    }
    
    int hour2 = time2.substring(0, colon2).toInt();
    int minute2 = time2.substring(colon2 + 1).toInt();
    
    // Convertir a minutos totales para comparación
    int totalMinutes1 = hour1 * 60 + minute1;
    int totalMinutes2 = hour2 * 60 + minute2;
    
    return (totalMinutes1 == totalMinutes2);
}

// Imprime la fecha y hora en formato legible
void RtcManager::printDateTime(const DateTime& dt) {
    char datestring[26];
    sprintf(datestring, "%04d-%02d-%02d %02d:%02d:%02d", 
            dt.year(), dt.month(), dt.day(), 
            dt.hour(), dt.minute(), dt.second());
    Serial.printf("%s", datestring);
}

// Verifica si el RTC está funcionando
bool RtcManager::isRunning() {
    if (!initialized) {
        return false;
    }
    return rtc.isrunning();
}