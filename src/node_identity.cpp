/**
 * @file node_identity.cpp
 * @brief Implementación de identidad de nodo sin persistencia
 * @date 22:00 17/6/2025
 */

#include "node_identity.h"

/**
 * @brief Constructor de NodeIdentity
 * @details Inicializa el sistema de identidad sin persistencia
 */
NodeIdentity::NodeIdentity()
{
    Serial.printf("NodeIdentity: Inicializando sin persistencia\n");
    begin();
}

/**
 * @brief Obtiene el identificador lógico del nodo
 * @param blacklist_len Tamaño de la lista negra
 * @param blacklist Valores prohibidos para el identificador
 * @return uint8_t Identificador entre 1-254 (0x00 y 0xFF reservados)
 * @details Genera un hash único basado en la MAC del dispositivo
 */
uint8_t NodeIdentity::getNodeID(size_t blacklist_len, const uint8_t *blacklist)
{
    Serial.printf("--- NodeIdentity::getNodeID() INICIO ---\n");
    Serial.printf("Parámetros recibidos: blacklist_len = %d\n", blacklist_len);

    Serial.printf("Generando nuevo ID basado en MAC...\n");

    String mac = getDeviceMAC();
    Serial.printf("MAC del dispositivo obtenida: %s\n", mac.c_str());

    uint8_t macBytes[6];
    int values[6];

    // Intenta parsear la MAC del string a valores hexadecimales
    int sscanfResult = sscanf(mac.c_str(), "%x:%x:%x:%x:%x:%x",
                              &values[0], &values[1], &values[2],
                              &values[3], &values[4], &values[5]);

    Serial.printf("sscanf(): Numero de items convertidos: %d\n", sscanfResult);

    // Verifica si sscanf fue exitoso (debe convertir 6 valores)
    if (sscanfResult != 6) {
        Serial.printf("ERROR: Fallo al parsear la MAC. Se generara un hash con MAC invalida.\n");
        // Considera aquí un manejo de error más robusto, como retornar un ID por defecto o reiniciar.
    } else {
        Serial.printf("Bytes MAC parseados (HEX): ");
        for (int i = 0; i < 6; ++i)
        {
            macBytes[i] = (uint8_t)values[i];
            Serial.printf("%02X ", macBytes[i]);
        }
        Serial.printf("\n");
    }

    uint8_t hash = generateSafeHash(macBytes, 6, blacklist, blacklist_len);

    Serial.printf("Hash generado: 0x%02X\n", hash);
    Serial.printf("--- NodeIdentity::getNodeID() FIN (Nuevo ID generado) ---\n");
    return hash;
}

/**
 * @brief Cambia el ID del nodo regenerando uno nuevo
 * @param blacklist_len Tamaño de la lista negra
 * @param blacklist Valores prohibidos para el identificador
 * @return uint8_t Nuevo ID generado
 */
uint8_t NodeIdentity::changeNodeID(const size_t blacklist_len, uint8_t *blacklist)
{
    Serial.printf("--- NodeIdentity::changeNodeID() INICIO ---\n");
    
    String mac = getDeviceMAC();
    uint8_t mac_bytes[6];
    int values[6];
    
    int sscanfResult = sscanf(mac.c_str(), "%x:%x:%x:%x:%x:%x",
                              &values[0], &values[1], &values[2],
                              &values[3], &values[4], &values[5]);

    if (sscanfResult == 6) {
        for (int i = 0; i < 6; ++i)
        {
            mac_bytes[i] = (uint8_t)values[i];
        }
    } else {
        Serial.printf("ERROR: Fallo al parsear MAC en changeNodeID\n");
    }

    uint8_t hash = generateSafeHash(mac_bytes, 6, blacklist, blacklist_len);
    
    Serial.printf("Nuevo ID generado: 0x%02X\n", hash);
    Serial.printf("--- NodeIdentity::changeNodeID() FIN ---\n");
    return hash;
}

/**
 * @brief Genera hash seguro evitando colisiones
 * @param data Datos a hashear
 * @param len Longitud de datos
 * @param blacklist Valores prohibidos
 * @param blacklist_len Tamaño de lista negra
 * @return uint8_t Hash seguro
 */
uint8_t NodeIdentity::generateSafeHash(
    const uint8_t *data,
    size_t len,
    const uint8_t *blacklist,
    size_t blacklist_len)
{
    uint8_t hash = crc8(data, len);
    bool conflict = true;
    uint8_t attempts = 0;
    const uint8_t max_attempts = 255;

    while (conflict && attempts < max_attempts)
    {
        conflict = false;
        for (size_t i = 0; i < blacklist_len; ++i)
        {
            if (hash == blacklist[i] || hash == 255) // valor prohibido prevee errores de faltar pasarlo en blacklist
            {
                hash = (hash + 1) % 256;
                conflict = true;
                attempts++;
                break;
            }
        }
    }
    
    if (attempts >= max_attempts) {
        Serial.printf("ADVERTENCIA: No se pudo generar hash único después de %d intentos\n", max_attempts);
    }
    
    return hash;
}

/**
 * @brief Implementación CRC-8 (polinomio 0x07)
 * @param data Buffer de entrada
 * @param len Tamaño del buffer
 * @return uint8_t Checksum calculado
 */
uint8_t NodeIdentity::crc8(const uint8_t *data, size_t len)
{
    uint8_t crc = 0x00;
    for (size_t i = 0; i < len; ++i)
    {
        crc ^= data[i];
        for (uint8_t j = 0; j < 8; ++j)
        {
            if (crc & 0x80)
                crc = (crc << 1) ^ 0x07;
            else
                crc <<= 1;
        }
    }
    return crc;
}

/**
 * @brief Obtiene la dirección MAC del dispositivo
 * @return String MAC en formato XX:XX:XX:XX:XX:XX
 */
String NodeIdentity::getDeviceMAC()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin();
    delay(100);
    String mac = WiFi.macAddress();
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
    return mac;
}

/**
 * @brief Verifica si tiene guardada dirección de gateway
 * @param getwayAdress Referencia donde se devuelve la dirección guardada
 * @return false (siempre false sin persistencia)
 */
bool NodeIdentity::getGetway(uint8_t &getwayAdress)
{
    Serial.printf("Gateway no configurado (sin persistencia)\n");
    getwayAdress = GETWAY_NOT_SET;
    return false;
}

/**
 * @brief Guarda dirección de gateway (sin persistencia)
 * @param getwayAdress Dirección del gateway a guardar
 */
void NodeIdentity::saveGetway(uint8_t getwayAdress)
{
    Serial.printf("Gateway address no guardado (sin persistencia): 0x%02X\n", getwayAdress);
}

/**
 * @brief Inicialización del sistema de identidad
 */
void NodeIdentity::begin()
{
    Serial.printf("NodeIdentity inicializado (modo sin persistencia)\n");
}