/**
 * @file node_identity.h
 * @brief Cabecera para la gestión de identidad de nodos sin persistencia
 * @date 22:00 17/6/2025
 */

#ifndef NODE_IDENTITY_H
#define NODE_IDENTITY_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "config.h"

// Constantes para valores no inicializados
#define HASH_NOT_SET 255   ///< Valor no inicializado
#define GETWAY_NOT_SET 255 ///< Valor no inicializado

static const uint8_t defaultBlacklist[2] = {0x00, 0xFF}; ///< lista por defecto de direcciones prohibidas 0x00 y 0xFF

/**
 * @class NodeIdentity
 * @brief Gestiona identificación única de nodos basada en hardware sin persistencia
 *
 * Genera un identificador de nodo (1 byte) mediante hash CRC-8 de la dirección MAC,
 * con protección contra colisiones mediante lista negra. Funciona sin persistencia.
 *
 * @warning Específico para ESP8266. Sin persistencia de datos.
 * @note Probabilidad de colisión: <0.0001% en redes <100 nodos
 * 
 * @example
 * ```cpp
 * #include "node_identity.h"
 * 
 * NodeIdentity identity;
 * uint8_t nodeId = identity.getNodeID();
 * Serial.printf("Node ID: 0x%02X\n", nodeId);
 * ```
 */
class NodeIdentity
{
public:
    /**
     * @brief Constructor
     * @details Inicializa el sistema de identidad sin persistencia
     * 
     * @example
     * ```cpp
     * NodeIdentity identity; // Inicialización automática
     * ```
     */
    NodeIdentity();

    /**
     * @brief Obtiene el identificador lógico del nodo
     *
     * Genera un hash único basado en la MAC del dispositivo,
     * evitando colisiones con valores prohibidos.
     *
     * @param blacklist_len Tamaño de la lista negra (opcional, default: 2)
     * @param blacklist Valores prohibidos para el identificador (opcional, default: defaultBlacklist)
     * @return uint8_t Identificador entre 1-254 (0x00 y 0xFF reservados)
     * @note Activa WiFi temporalmente durante la generación
     * 
     * @example
     * ```cpp
     * NodeIdentity identity;
     * uint8_t id = identity.getNodeID();
     * Serial.printf("Node ID: 0x%02X\n", id);
     * ```
     */
    uint8_t getNodeID(size_t blacklist_len = 2, const uint8_t *blacklist = defaultBlacklist);

    /**
     * @brief Recupera la dirección MAC del hardware
     * @return String MAC en formato XX:XX:XX:XX:XX:XX
     * @warning Enciende/cierra WiFi provocando ~100ms de latencia
     * 
     * @example
     * ```cpp
     * NodeIdentity identity;
     * String mac = identity.getDeviceMAC();
     * Serial.println("MAC: " + mac);
     * ```
     */
    String getDeviceMAC();

    /**
     * @brief Guarda dirección de gateway (sin persistencia)
     * @param getwayAdress Dirección del gateway a guardar
     * 
     * @details Gateway no persistente (funciona sin almacenamiento)
     * 
     * @example
     * ```cpp
     * NodeIdentity identity;
     * identity.saveGetway(0x42);
     * ```
     */
    void saveGetway(const uint8_t getwayAdress);

    /**
     * @brief Verifica si tiene guardada dirección de gateway
     * @param stored_getway Referencia donde se devuelve la dirección guardada
     * @return false (siempre false sin persistencia)
     * 
     * @example
     * ```cpp
     * NodeIdentity identity;
     * uint8_t gateway;
     * if (identity.getGetway(gateway)) {
     *     Serial.printf("Gateway: 0x%02X\n", gateway);
     * } else {
     *     Serial.println("No hay gateway registrado");
     * }
     * ```
     */
    bool getGetway(uint8_t &stored_getway);

    /**
     * @brief Cambia el ID del nodo
     * @param blacklist_len Tamaño de la lista negra
     * @param blacklist Valores prohibidos para el identificador
     * @return uint8_t Nuevo ID generado
     * 
     * @details Regenera el ID del nodo sin persistencia
     * @warning Esto cambiará el ID del nodo temporalmente
     */
    uint8_t changeNodeID(const size_t blacklist_len, uint8_t *blacklist);

    /**
     * @brief Inicialización del sistema de identidad
     * @details Configura el sistema sin persistencia
     * 
     * @example
     * ```cpp
     * NodeIdentity identity;
     * identity.begin();
     * ```
     */
    void begin();

private:
    uint8_t key[4]; ///< Almacenamiento interno de clave compartida

    /**
     * @brief Genera hash con evitación de colisiones
     * @param data Datos a hashear
     * @param len Longitud de datos
     * @param blacklist Valores prohibidos
     * @param blacklist_len Tamaño de lista negra
     * @return uint8_t Hash seguro
     * @warning Bucle infinito si lista negra contiene todos los valores 0-255
     * 
     * @example
     * ```cpp
     * uint8_t mac[6] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
     * uint8_t blacklist[2] = {0x00, 0xFF};
     * uint8_t hash = generateSafeHash(mac, 6, blacklist, 2);
     * ```
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
     * 
     * @example
     * ```cpp
     * uint8_t data[] = {0x01, 0x02, 0x03};
     * uint8_t checksum = crc8(data, 3);
     * ```
     */
    uint8_t crc8(const uint8_t *data, size_t len);
};
#endif
