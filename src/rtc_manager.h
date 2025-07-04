#ifndef RTC_MANAGER // Guards para evitar inclusiones múltiples
#define RTC_MANAGER

#include <Arduino.h> // Necesario para tipos como String y Serial (aunque Wire y RTClib ya lo incluyen indirectamente)
#include <Wire.h>    // Para la comunicación I2C
#include <RTClib.h>  // Librería de Adafruit para RTC
#include "config.h"

class RtcManager
{
public:
    // Constructor de la clase
    RtcManager();

    // Método para inicializar el RTC. Debe llamarse en setup().
    bool begin();

    // --- Funciones para obtener la hora y minutos en uint8_t ---
    uint8_t getHora();
    uint8_t getMinutos();
    uint8_t getSegundos();

    // --- Función para devolver la fecha en un formato considerado apropiado ---
    String getFecha_DDMMYYYY();
    String getFecha_YYYYMMDD();

    // --- Función para devolver fecha y tiempo (horas, minutos y segundos) en string ---
    String getFechaHoraCompleta();

private:
    // Instancia privada del objeto RTC_DS3231
    RTC_DS3231 rtc; // Usa RTC_DS3231 para el módulo DS3231.
                    // Si usas DS1307, cámbialo a: RTC_DS1307 rtc;
};

#endif