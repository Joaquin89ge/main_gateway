#include "rtc_manager.h"
#include <Arduino.h>

// Constructor - inicializa los pines para DS1302 (IO, SCLK, CE)
RtcManager::RtcManager(int ioPin, int sclkPin, int cePin) 
    : myWire(ioPin, sclkPin, cePin), rtc(myWire), isInitialized(false) {
}

// Inicializa el RTC
bool RtcManager::begin() {
    Serial.println("RtcManager::begin() - Iniciando inicialización");
    
    rtc.Begin();
    Serial.println("RtcManager::begin() - RTC.Begin() completado");
    
    // Obtener fecha y hora de compilación
    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
    Serial.print("RtcManager::begin() - Fecha de compilación: ");
    printDateTime(compiled);
    Serial.println();
    
    // Configurar el RTC con la fecha de compilación directamente
    Serial.println("RtcManager::begin() - Configurando RTC con fecha de compilación");
    rtc.SetDateTime(compiled);
    delay(200); // Dar más tiempo para que se configure
    
    // Verificar si el RTC está protegido contra escritura
    if (rtc.GetIsWriteProtected()) {
        Serial.println("RtcManager::begin() - RTC estaba protegido contra escritura, habilitando escritura");
        rtc.SetIsWriteProtected(false);
        delay(100);
    }
    
    // Verificar si el RTC está funcionando
    if (!rtc.GetIsRunning()) {
        Serial.println("RtcManager::begin() - RTC no estaba funcionando, iniciando ahora");
        rtc.SetIsRunning(true);
        delay(200); // Dar más tiempo para que inicie
    }
    
    // Obtener fecha/hora actual y verificar
    RtcDateTime now = rtc.GetDateTime();
    Serial.print("RtcManager::begin() - Fecha/hora actual del RTC: ");
    printDateTime(now);
    Serial.println();
    
    // Verificación final con más tolerancia
    if (!rtc.IsDateTimeValid()) {
        Serial.println("RtcManager::begin() - RTC no es válido, pero continuando de todas formas");
        // No retornamos false aquí, continuamos para ver si funciona
    }
    
    if (!rtc.GetIsRunning()) {
        Serial.println("RtcManager::begin() - RTC no está funcionando, pero continuando de todas formas");
        // No retornamos false aquí, continuamos para ver si funciona
    }
    
    Serial.println("RtcManager::begin() - RTC inicializado (con advertencias)");
    isInitialized = true;
    return true;
}

// Obtiene la fecha y hora actual
RtcDateTime RtcManager::getDateTime() {
    if (!isInitialized) {
        return RtcDateTime(0);
    }
    return rtc.GetDateTime();
}

// Establece la fecha y hora
bool RtcManager::setDateTime(const RtcDateTime& dateTime) {
    if (!isInitialized) {
        return false;
    }
    rtc.SetDateTime(dateTime);
    return true;
}

// Verifica si el RTC es válido
bool RtcManager::isDateTimeValid() {
    if (!isInitialized) {
        return false;
    }
    return rtc.IsDateTimeValid();
}

// Obtiene solo la hora y minutos como string (HH:MM)
String RtcManager::getTimeString() {
    if (!isInitialized) {
        Serial.println("RtcManager: No inicializado");
        return "00:00";
    }
    
    // Verificar si el RTC está funcionando
    if (!rtc.GetIsRunning()) {
        Serial.println("RtcManager: RTC no está funcionando");
        return "00:00";
    }
    
    // Verificar si la fecha es válida
    if (!rtc.IsDateTimeValid()) {
        Serial.println("RtcManager: Fecha/hora no válida");
        return "00:00";
    }
    
    // Obtener fecha/hora con validación adicional
    RtcDateTime now = rtc.GetDateTime();
    
    // Verificar que la fecha obtenida sea válida
    if (now.Year() < 2000 || now.Year() > 2100) {
        Serial.println("RtcManager: Año fuera de rango válido");
        return "00:00";
    }
    
    return getTimeString(now);
}

// Obtiene solo la hora y minutos como string (HH:MM) desde un RtcDateTime
String RtcManager::getTimeString(const RtcDateTime& dateTime) {
    char timeString[6];
    snprintf_P(timeString, sizeof(timeString), PSTR("%02u:%02u"), 
               dateTime.Hour(), dateTime.Minute());
    return String(timeString);
}

// Compara dos horas en formato HH:MM
bool RtcManager::compareHsAndMs(const String& time1, const String& time2) {
    Serial.print("RtcManager::compareHsAndMs: Comparando '");
    Serial.print(time1);
    Serial.print("' con '");
    Serial.print(time2);
    Serial.println("'");
    
    // Extraer horas y minutos de time1
    int colon1 = time1.indexOf(':');
    if (colon1 == -1) {
        Serial.println("RtcManager::compareHsAndMs: Error - time1 no tiene formato HH:MM");
        return false;
    }
    
    int hour1 = time1.substring(0, colon1).toInt();
    int minute1 = time1.substring(colon1 + 1).toInt();
    
    Serial.print("RtcManager::compareHsAndMs: time1 = ");
    Serial.print(hour1);
    Serial.print(":");
    Serial.println(minute1);
    
    // Extraer horas y minutos de time2
    int colon2 = time2.indexOf(':');
    if (colon2 == -1) {
        Serial.println("RtcManager::compareHsAndMs: Error - time2 no tiene formato HH:MM");
        return false;
    }
    
    int hour2 = time2.substring(0, colon2).toInt();
    int minute2 = time2.substring(colon2 + 1).toInt();
    
    Serial.print("RtcManager::compareHsAndMs: time2 = ");
    Serial.print(hour2);
    Serial.print(":");
    Serial.println(minute2);
    
    // Convertir a minutos totales para comparación
    int totalMinutes1 = hour1 * 60 + minute1;
    int totalMinutes2 = hour2 * 60 + minute2;
    
    Serial.print("RtcManager::compareHsAndMs: totalMinutes1 = ");
    Serial.print(totalMinutes1);
    Serial.print(", totalMinutes2 = ");
    Serial.println(totalMinutes2);
    
    bool result = (totalMinutes1 == totalMinutes2);
    Serial.print("RtcManager::compareHsAndMs: Resultado = ");
    Serial.println(result);
    
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
void RtcManager::printDateTime(const RtcDateTime& dt) {
    char datestring[26];
    snprintf_P(datestring, sizeof(datestring), PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
               dt.Month(), dt.Day(), dt.Year(), dt.Hour(), dt.Minute(), dt.Second());
    Serial.print(datestring);
}

// Verifica si el RTC está funcionando
bool RtcManager::isRunning() {
    if (!isInitialized) {
        return false;
    }
    return rtc.GetIsRunning();
}

// Habilita/deshabilita la protección de escritura
void RtcManager::setWriteProtected(bool writeProtected) {
    if (isInitialized) {
        rtc.SetIsWriteProtected(writeProtected);
    }
}

// Verifica si está protegido contra escritura
bool RtcManager::isWriteProtected() {
    if (!isInitialized) {
        return false;
    }
    return rtc.GetIsWriteProtected();
}