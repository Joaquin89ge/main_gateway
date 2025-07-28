#ifndef RTC_MANAGER_H
#define RTC_MANAGER_H

#include <RTClib.h>  // Librería RTClib para DS1307
#include <Wire.h>     // Para comunicación I2C

class RtcManager {
private:
    RTC_DS1307 rtc;
    bool isInitialized;

public:
    // Constructor - inicializa el RTC DS1307 (usa I2C)
    RtcManager();
    
    // Inicializa el RTC
    bool begin();
    
    // Obtiene la fecha y hora actual
    DateTime getDateTime();
    
    // Establece la fecha y hora
    bool setDateTime(const DateTime& dateTime);
    
    // Verifica si el RTC es válido
    bool isDateTimeValid();
    
    // Obtiene solo la hora y minutos como string (HH:MM)
    String getTimeString();
    
    // Obtiene solo la hora y minutos como string (HH:MM) desde un DateTime
    static String getTimeString(const DateTime& dateTime);
    
    // Compara dos horas en formato HH:MM
    static bool compareHsAndMs(const String& time1, const String& time2);
    
    // Compara la hora actual con una hora específica
    bool compareCurrentTimeWith(const String& targetTime);
    
    // Imprime la fecha y hora en formato legible
    void printDateTime(const DateTime& dt);
    
    // Verifica si el RTC está funcionando
    bool isRunning();
    
    // Habilita/deshabilita la protección de escritura
    void setWriteProtected(bool writeProtected);
    
    // Verifica si está protegido contra escritura
    bool isWriteProtected();
    
    // Funciones adicionales para EEPROM AT24C32
    bool writeToEEPROM(uint16_t address, uint8_t data);
    uint8_t readFromEEPROM(uint16_t address);
    bool writeStringToEEPROM(uint16_t address, const String& data);
    String readStringFromEEPROM(uint16_t address, uint8_t maxLength);
};

#endif // RTC_MANAGER_H