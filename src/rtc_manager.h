/**
 * @file rtc_manager.h
 * @brief Gestor del módulo RTC DS1307
 * @date 2025
 * 
 * Este módulo maneja la comunicación con el RTC DS1307 a través de I2C.
 * Proporciona funciones para obtener fecha/hora y comparar intervalos de tiempo.
 * 
 * Características técnicas:
 * - RTC: DS1307 (I2C, dirección 0x68)
 * - Precisión: ±2ppm a 0°C
 * - Batería: CR2032 (hasta 10 años)
 * - Temperatura de operación: -40°C a +85°C
 * - Consumo: <500nA en modo batería
 */

#ifndef RTC_MANAGER_H
#define RTC_MANAGER_H

#include <Arduino.h>
#include <Wire.h>
#include <RTClib.h>

// Dirección I2C del DS1307
#define RTC_I2C_ADDRESS 0x68

class RtcManager {
private:
    RTC_DS1307 rtc;
    bool initialized;
    
    /**
     * @brief Convierte DateTime a string de hora (HH:MM)
     * @param dt Objeto DateTime
     * @return String con formato HH:MM
     */
    String getTimeString(const DateTime& dt);

public:
    /**
     * @brief Constructor del RtcManager
     */
    RtcManager();
    
    /**
     * @brief Inicializa el RTC DS1307
     * @return true si la inicialización fue exitosa
     */
    bool begin();
    
    /**
     * @brief Obtiene la fecha y hora actual
     * @return Objeto DateTime con la fecha/hora actual
     */
    DateTime getDateTime();
    
    /**
     * @brief Establece la fecha y hora del RTC
     * @param dt Objeto DateTime con la nueva fecha/hora
     * @return true si se estableció correctamente
     */
    bool setDateTime(const DateTime& dt);
    
    /**
     * @brief Verifica si la fecha/hora es válida
     * @param dt Objeto DateTime a verificar
     * @return true si la fecha/hora es válida
     */
    bool isDateTimeValid(const DateTime& dt);
    
    /**
     * @brief Obtiene la hora actual como string (HH:MM)
     * @return String con formato HH:MM
     */
    String getTimeString();
    
    /**
     * @brief Compara dos horas en formato HH:MM
     * @param time1 Primera hora (HH:MM)
     * @param time2 Segunda hora (HH:MM)
     * @return true si las horas son iguales
     */
    bool compareHsAndMs(const String& time1, const String& time2);
    
    /**
     * @brief Imprime la fecha/hora en formato legible
     * @param dt Objeto DateTime a imprimir
     */
    void printDateTime(const DateTime& dt);
    
    /**
     * @brief Verifica si el RTC está funcionando
     * @return true si el RTC está funcionando
     */
    bool isRunning();
    
    /**
     * @brief Verifica si el RTC está inicializado
     * @return true si el RTC está inicializado
     */
    bool getInitialized() const { return initialized; }
};

#endif // RTC_MANAGER_H