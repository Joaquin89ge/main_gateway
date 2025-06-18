// 03:00 16/6/2025
#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include <Arduino.h>
#include "protocol.h"

class SensorManager
{
private:
  unsigned long sampleIntervalMs = 59000; // Intervalo de muestreo en milisegundos cada 1 minuto un poco menos por las dudas
  unsigned long lastSampleTime;           // Tiempo en millis() de la última muestra tomada
  int atmosSampleCount = 0;

  // Método privado para simular o leer sensores atmosfera temp y humedad
  void readAtmosphericSensors();
  // Método privado para simular o leer sensores en tierra npk,humedad,temp,ph,ec(electroconductividad)
  void readGroundSensors();
  // Método privado para simular o leer sensores energeticos voltaje y/o intensidad y/o carga
  void readEnergySensors();
  // Método privado para simular o leer sensores gps
  void readGpsSensors();
  // ve si esta lleno el array sino lo llena con lecturas inmediatas
  void verificFullAtmosSamples();

public:
  // Almacenamiento de los datos
  // Almacenamiento interno de los datos
  AtmosphericSample atmosSamples[8]; // Ejemplo: array para 8 muestras
  GroundSensor groundData;
  GpsSensor gpsData;
  EnergyData energyData;
  // Constructor (opcionalmente inicializar sensores aquí)
  SensorManager();
  // actualiza recurrentemente lecturas atmosfericas
  void update(); // TODO: llamar recurentemente a clase para tomar valores atmosfericos cada x tiempo
  // Método para recolectar datos de los sensores
  void readGroundGpsSensors();
  // Método para recolectar datos de los sensores
  void readSensorsAtmospheric();
};
#endif