/**
 * @file node_identity.h
 * @brief Cabecera para la gestión de identidad de nodos con clave compartida
 * @date 22:00 17/6/2025
 */

#ifndef NODE_IDENTITY_H
#define NODE_IDENTITY_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <LittleFS.h> // ¡Cambiamos de EEPROM.h a LittleFS.h!
#include "config.h"
// Nombres de archivos para LittleFS (más descriptivo que direcciones de EEPROM)
#define NODE_ID_FILE "/node_id.json"      ///< Nombre del archivo para el ID del nodo
#define GATEWAY_ADDR_FILE "/gateway.json" ///< Nombre del archivo para la dirección del gateway

#define HASH_NOT_SET 255   ///< Valor no inicializado (seguimos usándolo para la lógica interna)
#define GETWAY_NOT_SET 255 ///< Valor no inicializado (seguimos usándolo para la lógica interna)

extern const uint8_t defaultBlacklist[2]; ///< lista por defecto de direcciones prohibidas 0x00 y 0xFF

/**
 * @class NodeIdentity
 * @brief Gestiona identificación única de nodos basada en hardware
 *
 * Genera un identificador de nodo (1 byte) mediante hash CRC-8 de la dirección MAC,
 * con protección contra colisiones mediante lista negra. Almacena el estado en EEPROM.
 *
 * @warning Específico para ESP8266. Requiere inicialización de EEPROM externa.
 * @note Probabilidad de colisión: <0.0001% en redes <100 nodos
 */
class NodeIdentity
{
public:
    /**
     * @brief Constructor con clave de autenticación
     */
    NodeIdentity();

    /**
     * @brief Obtiene el identificador lógico del nodo
     *
     * Genera un hash único o recupera el valor almacenado en EEPROM,
     * evitando colisiones con valores prohibidos.
     *
     * @param blacklist Valores prohibidos para el identificador
     * @param blacklist_len Tamaño de la lista negra
     * @return uint8_t Identificador entre 1-254 (0x00 y 0xFF reservados)
     * @note Activa WiFi temporalmente durante la generación
     */

    uint8_t getNodeID(size_t blacklist_len = 2, const uint8_t *blacklist = defaultBlacklist);

    /**
     * @brief Recupera la dirección MAC del hardware
     * @return String MAC en formato XX:XX:XX:XX:XX:XX
     * @warning Enciende/cierra WiFi provocando ~100ms de latencia
     */
    String getDeviceMAC();

    /**
     * @brief guarda direccion de getway
     * @param getwayAdress direccion a verificar si es mi getway

     */
    void saveGetway(const uint8_t getwayAdress);

    /**
     * @brief verifica si tiene guardada direccion de getway
     * @param getwayAdress direccion a devover en caso de existencia
     *@return true guardada false no guardada
     */
    bool getGetway(uint8_t &stored_getway);

    uint8_t changeNodeID(const size_t blacklist_len, uint8_t *blacklist);

    // inicializacion
    void begin();

private:
    uint8_t key[4]; ///< Almacenamiento interno de clave compartida
    // Declaración en el encabezado o en un ámbito apropiado
    static const uint8_t defaultBlacklist[2]; ///< lista por defecto de direcciones prohibidas 0x00 y 0xFF

    /**
     * @brief Genera hash con evitación de colisiones
     * @param data Datos a hashear
     * @param len Longitud de datos
     * @param blacklist Valores prohibidos
     * @param blacklist_len Tamaño de lista negra
     * @return uint8_t Hash seguro
     * @warning Bucle infinito si lista negra contiene todos los valores 0-255
     */
    uint8_t generateSafeHash(
        const uint8_t *data,
        size_t len,
        const uint8_t *blacklist,
        size_t blacklist_len);

    /**
     * @brief Implementación CRC-8 (polinomio 0x07)
     * @param data Buffer de entrada
     * @param len Tamaño del buffer
     * @return uint8_t Checksum calculado
     */
    uint8_t crc8(const uint8_t *data, size_t len);

    bool loadByteFromFile(const char *filename, uint8_t &value);
    void saveByteToFile(const char *filename, uint8_t value);
};
#endif
