// Protocol.h (protocol bit support)
// 03:00 16/6/2025

#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <Arduino.h> // Para tipos como String, uint8_t
#include <vector>    // Para std::vector

// #define RH_MESH_MAX_MESSAGE_LEN 50
/**
 * @brief Espacio de nombres para las definiciones del protocolo de comunicación.
 *
 * Este espacio de nombres encapsula las estructuras de datos y funciones
 * para la serialización y deserialización de mensajes entre nodos.
 */
namespace Protocol
{

    const uint8_t KEY = 0x69; /**< @brief Longitud de la clave de seguridad del protocolo. */

    /**
     * @brief Enumeración que define los tipos de mensajes soportados por el protocolo.
     *
     * Cada valor representa un propósito específico para el mensaje, permitiendo
     * a los nodos interpretar y procesar el contenido del payload JSON adecuadamente.
     */
    enum MessageType : uint8_t
    {
        ANNOUNCE = 0x01,                 /**< @brief Mensaje de anuncio de un nodo. */
        REQUEST_DATA_ATMOSPHERIC = 0x02, /**< @brief Protocolo para  solicitud de datos atmosfericos.*/
        REQUEST_DATA_GPC_GROUND = 0x03,  /**< @brief Protocolo para  solicitud de datos gps y ground */
        DATA_ATMOSPHERIC = 0x04,         /**< @brief Protocolo para  envio de datos atmosfericos. */
        DATA_GPS_CROUND = 0x05,          /**< @brief Protocolo para  envio de datos gps y ground. */
        HELLO = 0x06,                    /**< @brief Mensaje de saludo/conexión inicial. */
        ERROR_DIRECCION = 0x07           /**< @brief Mensaje de direccion de nodo repetida volver hacer hash mac. */
    };

} // namespace Protocol

#pragma pack(push, 1) // Eliminar padding entre miembros de estructuras

/* Muestra atmosférica (DHT22)
 * Rango temperatura: -40.0°C a 80.0°C (con precisión ±0.5°C)
 * Rango humedad: 0.0% a 100.0% (con precisión ±3%)
 * Total: 6 bytes por muestra
 */
struct AtmosphericSample
{
    int16_t temp;      // Temperatura en décimas de grado [-400 a 800]
                       // (-400 = -40.0°C, 800 = 80.0°C)
    uint16_t moisture; // Humedad en décimas de porcentaje [0 a 1000]
                       // (0 = 0.0%, 1000 = 100.0%)
    uint8_t hour;      // Hora de medición [0-23]
    uint8_t minute;    // Minuto de medición [0-59]
};

/**
 * Sensor de suelo (NPK/PH/EC)
 * Total: 13 bytes
 */
struct GroundSensor
{
    int16_t temp;      // Temperatura en décimas de grado [-400 a 800]
    uint16_t moisture; // Humedad en décimas de porcentaje [0 a 1000]
    uint16_t n;        // Nitrógeno [0-1999] mg/kg (entero)
    uint16_t p;        // Fósforo [0-1999] mg/kg (entero)
    uint16_t k;        // Potasio [0-1999] mg/kg (entero)
    uint16_t EC;       // Conductividad eléctrica [0-20000] μS/cm (entero)
    uint8_t PH;        // PH en décimas [30-90] (30 = 3.0, 90 = 9.0)
};

/**
 * Datos GPS (Gy-neo6mv2)
 * Total: 13 bytes
 */
struct GpsSensor
{
    int32_t latitude;  // Latitud en grados * 10^7 [-1800000000 a 1800000000]
                       // (Valores negativos = Sur, Positivos = Norte)
    int32_t longitude; // Longitud en grados * 10^7 [-1800000000 a 1800000000]
                       // (Valores negativos = Oeste, Positivos = Este)
    int16_t altitude;  // Altitud en metros [-1000 a 9000]
    uint8_t hour;      // Hora GPS [0-23]
    uint8_t minute;    // Minuto GPS [0-59]
    uint8_t flags;     // Flags de validación (bitmask):
                       //   bit0 (0x01): Ubicación válida
                       //   bit1 (0x02): Altitud válida
                       //   bit2 (0x04): Hora válida
                       //   bits 3-7: Reservados
};

#pragma pack(pop) // Restaurar alineación normal

#pragma pack(push, 1) // Asegurar empaquetamiento sin padding

/**
 * Paquete combinado Suelo + GPS
 * Total: 26 bytes
 */
struct GroundGpsPacket
{
    GroundSensor ground; // 13 bytes
    GpsSensor gps;       // 13 bytes

}; // Total: 26 bytes

#pragma pack(pop) // Restaurar alineación normal
// clase propia para gestion consumo interno no se trasnmite
struct EnergyData
{
    float volt;
    float amp;
};

//* posible agrandamiento de gps para contemplar datos defecha (año mes dia)
/*
modulo dht22 para atmosferico
Rango de medicion de humedad: 0%~100%RH.
Rango de temperatura : -40~+80º.
Presicion de medicion Humedad: ±3%RH.
Presicion de medicion de temperatura: ±0.5º.

para gps uso Modulo Gps Gy-neo6mv2
110617 – representa la hora en la que se tomó la ubicación fija, 11:06:17 UTC
41XX.XXXXX,N – latitud 41 grados XX.XXXXX' N
00831.54761,W – Longitud 008 grados 31.54761′ O

129.0, M – Altitud, en metros sobre el nivel del mar

para sensor ground Sensor Suelo Npk Ph T/h Ec Modbus Rs485 Nutrientes Tierra
- Temperatura Rango: -40 a 80 [°C]
- Resolución: 0.1 °C - Exactitud: +/- 5 °C
- Humedad Rango: 0 a 100 % [RH]
- Resolución: 0.1% - Exactitud: +/- 2 a 3%
- Conductividad (EC) Rango: 0 a 20000 uS/cm [micro-Siemens / cm]
- Resolución: 0.1% - Exactitud: +/- 3 a 5%
- pH Rango: 3 a 9 [pH]
- Resolución: 0.1 [pH]
- Nitrógeno/Fósforo/Potasio (NPK) Rango: 1 a 1999 [mg/kg]
- Resolución: 1 [mg/kg]
*/

#endif // PROTOCOL_H
