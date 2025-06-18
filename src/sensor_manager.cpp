// 03:00 16/6/2025
#include "sensor_manager.h"

// Constructor (opcionalmente inicializar sensores aquí)
SensorManager::SensorManager()
{
}

// Método privado para simular o leer sensores atmosfera temp y humedad
void SensorManager::readAtmosphericSensors()
{
  if (atmosSampleCount <= 8)
  {
    // Lectura de ambiente (ej: añadir una nueva muestra al array)
    atmosSamples[atmosSampleCount].temp = -25.3;
    atmosSamples[atmosSampleCount].moisture = 60.5;
    atmosSamples[atmosSampleCount].hour = 10;
    atmosSamples[atmosSampleCount].minute = 22;
    atmosSampleCount++;
  }
}
// Método privado para simular o leer sensores en tierra npk,humedad,temp,ph,ec(electroconductividad)
void SensorManager::readGroundSensors()
{
  // Lectura del suelo
  groundData.temp = -20.1;
  groundData.moisture = 75.0;
  groundData.n = 1000;
  groundData.n = 150;
  groundData.n = 133;
  groundData.EC = 2556;
  groundData.PH = 6.8;
}

void SensorManager::readGpsSensors()
{
  /** // Llenar la estructura con los datos del GPS y sus estados de validez.
  gpsData.locationValid = gps.location.isValid();
  if (gpsData.locationValid)
  {
    gpsData.latitude = gps.location.lat();
    gpsData.longitude = gps.location.lng();
  }
  else
  {
    gpsData.latitude = 0.0;
    gpsData.longitude = 0.0;
  }

  gpsData.altitudeValid = gps.altitude.isValid();
  if (gpsData.altitudeValid)
  {
    gpsData.altitudeMeters = gps.altitude.meters();
  }
  else
  {
    gpsData.altitudeMeters = 0.0;
  }

  gpsData.dateTimeValid = gps.date.isValid() && gps.time.isValid();
  if (gps.date.isValid() && gps.time.isValid())
  {
    gpsData.year = gps.date.year();
    gpsData.month = gps.date.month();
    gpsData.day = gps.date.day();
    gpsData.hour = gps.time.hour();
    gpsData.minute = gps.time.minute();
    gpsData.second = gps.time.second();
  }
  else
  {
    gpsData.year = 0;
    gpsData.month = 0;
    gpsData.day = 0;
    gpsData.hour = 0;
    gpsData.minute = 0;
    gpsData.second = 0;
  }*/
  gpsData.latitude = -1800000000;
  gpsData.longitude = 1800000000;
  gpsData.altitude = 100;
  gpsData.hour = 0;
  gpsData.minute = 0;
  gpsData.flags = 1;
}

// Método privado para simular o leer sensores energeticos voltaje y/o intensidad y/o carga
void SensorManager::readEnergySensors()
{

  // Lectura de energía
  energyData.amp = 2.5;
  energyData.volt = 12.0;
}
void SensorManager::update()
{
  unsigned long currentMillis = millis(); // Obtiene el tiempo actual en milisegundos

  // Comprueba si ha pasado suficiente tiempo desde la última muestra
  if (currentMillis - lastSampleTime >= sampleIntervalMs)
  {
    lastSampleTime = currentMillis; // Actualiza el tiempo de la última muestra
    readAtmosphericSensors();       // Toma una nueva muestra
  }
}

void SensorManager::verificFullAtmosSamples()
{
  while (atmosSampleCount <= 8)
  {
    readAtmosphericSensors();
    delay(10);
  }
}

// Método para recolectar datos de los sensores
void SensorManager::readGroundGpsSensors()
{

  // Simulación de lectura de sensores (reemplaza con tu hardware)
  readGroundSensors();
  readEnergySensors();
  readGpsSensors();
}

void SensorManager::readSensorsAtmospheric()
{
  update();                  // llamo  a update por cuestion de actualizacion de muestre
  verificFullAtmosSamples(); // verifico
}