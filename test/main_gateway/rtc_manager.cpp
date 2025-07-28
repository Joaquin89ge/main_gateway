#include "rtc_manager.h"
#include <Arduino.h>

// Constructor - inicializa el RTC DS1307 (usa I2C)
RtcManager::RtcManager() : isInitialized(false) {
}

// Inicializa el RTC
bool RtcManager::begin() {
    Serial.printf("RtcManager::begin() - Iniciando inicialización DS1307\n");
    
    // Inicializar I2C con configuración específica para ESP8266
    Wire.begin();
    Wire.setClock(100000); // 100kHz para mayor compatibilidad
    delay(100);
    
    // Verificar si hay dispositivos I2C conectados
    Serial.printf("RtcManager::begin() - Escaneando dispositivos I2C...\n");
    bool foundDS1307 = false;
    bool foundEEPROM = false;
    
    for (byte addr = 1; addr < 127; addr++) {
        Wire.beginTransmission(addr);
        byte error = Wire.endTransmission();
        if (error == 0) {
            Serial.printf("Dispositivo I2C encontrado en 0x%02X\n", addr);
            if (addr == 0x68) {
                Serial.printf("  → DS1307 RTC detectado\n");
                foundDS1307 = true;
            } else if (addr == 0x57) {
                Serial.printf("  → AT24C32 EEPROM detectada\n");
                foundEEPROM = true;
            }
        }
    }
    
    if (!foundDS1307) {
        Serial.printf("RtcManager::begin() - ERROR: DS1307 no encontrado en I2C\n");
        return false;
    }
    
    // Inicializar RTC DS1307
    if (!rtc.begin()) {
        Serial.printf("RtcManager::begin() - ERROR: No se pudo inicializar DS1307\n");
        return false;
    }
    
    Serial.printf("RtcManager::begin() - DS1307 inicializado correctamente\n");
    
    // Verificar si el RTC está funcionando
    if (!rtc.isrunning()) {
        Serial.printf("RtcManager::begin() - RTC no está funcionando, configurando con fecha de compilación\n");
        
        // Configurar con fecha de compilación
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
        Serial.printf("RtcManager::begin() - RTC configurado con fecha de compilación\n");
        
        // Verificar que se configuró correctamente
        delay(100);
        if (!rtc.isrunning()) {
            Serial.printf("RtcManager::begin() - ERROR: RTC no responde después de configuración\n");
            return false;
        }
    }
    
    // Obtener fecha/hora actual y verificar
    DateTime now = rtc.now();
    Serial.printf("RtcManager::begin() - Fecha/hora actual del RTC: ");
    printDateTime(now);
    Serial.printf("\n");
    
    // Verificar que la fecha sea válida
    if (now.year() < 2000 || now.year() > 2100) {
        Serial.printf("RtcManager::begin() - ADVERTENCIA: Año fuera de rango válido (%d)\n", now.year());
        // No retornamos false aquí, solo advertencia
    }
    
    // Verificar que la hora sea razonable
    if (now.hour() > 23 || now.minute() > 59 || now.second() > 59) {
        Serial.printf("RtcManager::begin() - ERROR: Hora inválida\n");
        return false;
    }
    
    Serial.printf("RtcManager::begin() - RTC DS1307 inicializado correctamente\n");
    if (foundEEPROM) {
        Serial.printf("RtcManager::begin() - EEPROM AT24C32 disponible\n");
    }
    
    isInitialized = true;
    return true;
}

// Obtiene la fecha y hora actual
DateTime RtcManager::getDateTime() {
    if (!isInitialized) {
        // Retornar la fecha actual del sistema como fallback
        return rtc.now();
    }
    return rtc.now();
}

// Establece la fecha y hora
bool RtcManager::setDateTime(const DateTime& dateTime) {
    if (!isInitialized) {
        return false;
    }
    rtc.adjust(dateTime);
    return true;
}

// Verifica si el RTC es válido
bool RtcManager::isDateTimeValid() {
    if (!isInitialized) {
        return false;
    }
    DateTime now = rtc.now();
    return (now.year() >= 2000 && now.year() <= 2100);
}

// Obtiene solo la hora y minutos como string (HH:MM)
String RtcManager::getTimeString() {
    if (!isInitialized) {
        Serial.printf("RtcManager: No inicializado\n");
        return "00:00";
    }
    
    // Verificar si el RTC está funcionando
    if (!rtc.isrunning()) {
        Serial.printf("RtcManager: RTC no está funcionando\n");
        return "00:00";
    }
    
    // Obtener fecha/hora
    DateTime now = rtc.now();
    
    // Verificar que la fecha obtenida sea válida
    if (now.year() < 2000 || now.year() > 2100) {
        Serial.printf("RtcManager: Año fuera de rango válido\n");
        return "00:00";
    }
    
    return getTimeString(now);
}

// Obtiene solo la hora y minutos como string (HH:MM) desde un DateTime
String RtcManager::getTimeString(const DateTime& dateTime) {
    char timeString[6];
    snprintf_P(timeString, sizeof(timeString), PSTR("%02u:%02u"), 
               dateTime.hour(), dateTime.minute());
    return String(timeString);
}

// Compara dos horas en formato HH:MM
bool RtcManager::compareHsAndMs(const String& time1, const String& time2) {
    Serial.printf("RtcManager::compareHsAndMs: Comparando '%s' con '%s'\n", time1.c_str(), time2.c_str());
    
    // Extraer horas y minutos de time1
    int colon1 = time1.indexOf(':');
    if (colon1 == -1) {
        Serial.printf("RtcManager::compareHsAndMs: Error - time1 no tiene formato HH:MM\n");
        return false;
    }
    
    int hour1 = time1.substring(0, colon1).toInt();
    int minute1 = time1.substring(colon1 + 1).toInt();
    
    Serial.printf("RtcManager::compareHsAndMs: time1 = %d:%d\n", hour1, minute1);
    
    // Extraer horas y minutos de time2
    int colon2 = time2.indexOf(':');
    if (colon2 == -1) {
        Serial.printf("RtcManager::compareHsAndMs: Error - time2 no tiene formato HH:MM\n");
        return false;
    }
    
    int hour2 = time2.substring(0, colon2).toInt();
    int minute2 = time2.substring(colon2 + 1).toInt();
    
    Serial.printf("RtcManager::compareHsAndMs: time2 = %d:%d\n", hour2, minute2);
    
    // Convertir a minutos totales para comparación
    int totalMinutes1 = hour1 * 60 + minute1;
    int totalMinutes2 = hour2 * 60 + minute2;
    
    Serial.printf("RtcManager::compareHsAndMs: totalMinutes1 = %d, totalMinutes2 = %d\n", totalMinutes1, totalMinutes2);
    
    bool result = (totalMinutes1 == totalMinutes2);
    Serial.printf("RtcManager::compareHsAndMs: Resultado = %s\n", result ? "true" : "false");
    
    return result;
}

// Compara la hora actual con una hora específica
bool RtcManager::compareCurrentTimeWith(const String& targetTime) {
    if (!isInitialized) {
        return false;
    }
    String currentTime = getTimeString();
    return compareHsAndMs(currentTime, targetTime);
}

// Imprime la fecha y hora en formato legible
void RtcManager::printDateTime(const DateTime& dt) {
    char datestring[26];
    snprintf_P(datestring, sizeof(datestring), PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
               dt.month(), dt.day(), dt.year(), dt.hour(), dt.minute(), dt.second());
    Serial.printf("%s", datestring);
}

// Verifica si el RTC está funcionando
bool RtcManager::isRunning() {
    if (!isInitialized) {
        return false;
    }
    return rtc.isrunning();
}

// Habilita/deshabilita la protección de escritura
void RtcManager::setWriteProtected(bool writeProtected) {
    if (isInitialized) {
        // DS1307 no tiene protección de escritura configurable
        Serial.printf("RtcManager: DS1307 no tiene protección de escritura configurable\n");
    }
}

// Verifica si está protegido contra escritura
bool RtcManager::isWriteProtected() {
    // DS1307 no tiene protección de escritura configurable
    return false;
}

// ===== FUNCIONES PARA EEPROM AT24C32 =====

// Escribe un byte en la EEPROM AT24C32
bool RtcManager::writeToEEPROM(uint16_t address, uint8_t data) {
    // Validar dirección (AT24C32 tiene 4KB = 4096 bytes)
    if (address >= 4096) {
        Serial.printf("RtcManager::writeToEEPROM: Dirección inválida 0x%04X\n", address);
        return false;
    }
    
    Wire.beginTransmission(0x57);
    Wire.write((address >> 8) & 0xFF);  // Dirección alta
    Wire.write(address & 0xFF);          // Dirección baja
    Wire.write(data);
    byte result = Wire.endTransmission();
    
    if (result == 0) {
        delay(5); // Tiempo de escritura
        return true;
    } else {
        Serial.printf("RtcManager::writeToEEPROM: Error al escribir en EEPROM, código: %d\n", result);
        return false;
    }
}

// Lee un byte de la EEPROM AT24C32
uint8_t RtcManager::readFromEEPROM(uint16_t address) {
    // Validar dirección
    if (address >= 4096) {
        Serial.printf("RtcManager::readFromEEPROM: Dirección inválida 0x%04X\n", address);
        return 0;
    }
    
    Wire.beginTransmission(0x57);
    Wire.write((address >> 8) & 0xFF);  // Dirección alta
    Wire.write(address & 0xFF);          // Dirección baja
    byte result = Wire.endTransmission();
    
    if (result != 0) {
        Serial.printf("RtcManager::readFromEEPROM: Error al configurar dirección, código: %d\n", result);
        return 0;
    }
    
    Wire.requestFrom(0x57, 1);
    if (Wire.available()) {
        return Wire.read();
    } else {
        Serial.printf("RtcManager::readFromEEPROM: No hay datos disponibles\n");
        return 0;
    }
}

// Escribe una cadena en la EEPROM AT24C32
bool RtcManager::writeStringToEEPROM(uint16_t address, const String& data) {
    // Validar longitud máxima (evitar desbordamiento)
    if (data.length() > 255) {
        Serial.printf("RtcManager::writeStringToEEPROM: String demasiado largo (%d caracteres)\n", data.length());
        return false;
    }
    
    // Validar que quepa en la EEPROM
    if (address + data.length() + 1 >= 4096) {
        Serial.printf("RtcManager::writeStringToEEPROM: No hay espacio suficiente en EEPROM\n");
        return false;
    }
    
    uint16_t currentAddress = address;
    
    // Escribir longitud de la cadena
    if (!writeToEEPROM(currentAddress, data.length())) {
        return false;
    }
    currentAddress++;
    
    // Escribir cada carácter
    for (uint8_t i = 0; i < data.length(); i++) {
        if (!writeToEEPROM(currentAddress + i, data.charAt(i))) {
            return false;
        }
    }
    
    Serial.printf("RtcManager::writeStringToEEPROM: Cadena guardada en dirección 0x%04X\n", address);
    return true;
}

// Lee una cadena de la EEPROM AT24C32
String RtcManager::readStringFromEEPROM(uint16_t address, uint8_t maxLength) {
    // Validar parámetros
    if (maxLength == 0 || maxLength > 255) {
        Serial.printf("RtcManager::readStringFromEEPROM: Longitud máxima inválida: %d\n", maxLength);
        return "";
    }
    
    // Leer longitud de la cadena
    uint8_t strLength = readFromEEPROM(address);
    if (strLength == 0 || strLength > maxLength) {
        Serial.printf("RtcManager::readStringFromEEPROM: Longitud inválida: %d\n", strLength);
        return "";
    }
    
    // Validar que quepa en la EEPROM
    if (address + strLength + 1 >= 4096) {
        Serial.printf("RtcManager::readStringFromEEPROM: Dirección fuera de rango\n");
        return "";
    }
    
    String result = "";
    for (uint8_t i = 0; i < strLength; i++) {
        char c = (char)readFromEEPROM(address + 1 + i);
        result += c;
    }
    
    Serial.printf("RtcManager::readStringFromEEPROM: Cadena leída desde dirección 0x%04X: '%s'\n", address, result.c_str());
    return result;
}