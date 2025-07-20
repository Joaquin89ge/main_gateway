#ifndef RTC_MANAGER_H
#define RTC_MANAGER_H

#include <RtcDS1302.h>  // Librería RtcDS1302
#include <ThreeWire.h>   // Para comunicación 3-wire del DS1302

class RtcManager {
private:
    ThreeWire myWire;
    RtcDS1302<ThreeWire> rtc;
    bool isInitialized;

public:
    // Constructor - inicializa los pines para DS1302 (IO, SCLK, CE)
    RtcManager(int ioPin, int sclkPin, int cePin);
    
    // Inicializa el RTC
    bool begin();
    
    // Obtiene la fecha y hora actual
    RtcDateTime getDateTime();
    
    // Establece la fecha y hora
    bool setDateTime(const RtcDateTime& dateTime);
    
    // Verifica si el RTC es válido
    bool isDateTimeValid();
    
    // Obtiene solo la hora y minutos como string (HH:MM)
    String getTimeString();
    
    // Obtiene solo la hora y minutos como string (HH:MM) desde un RtcDateTime
    static String getTimeString(const RtcDateTime& dateTime);
    
    // Compara dos horas en formato HH:MM
    static bool compareHsAndMs(const String& time1, const String& time2);
    
    // Compara la hora actual con una hora específica
    bool compareCurrentTimeWith(const String& targetTime);
    
    // Imprime la fecha y hora en formato legible
    void printDateTime(const RtcDateTime& dt);
    
    // Verifica si el RTC está funcionando
    bool isRunning();
    
    // Habilita/deshabilita la protección de escritura
    void setWriteProtected(bool writeProtected);
    
    // Verifica si está protegido contra escritura
    bool isWriteProtected();
};

#endif // RTC_MANAGER_H