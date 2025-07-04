/**
 * @file node_identity.cpp
 * @brief Implementación de identidad de nodo con persistencia EEPROM
 * @date 22:00 17/6/2025
 */

#include "node_identity.h"

// Definición de la lista de blacklist por defecto.
const uint8_t NodeIdentity::defaultBlacklist[2] = {0x00, 0xFF};

NodeIdentity::NodeIdentity()
{
    begin();
}

uint8_t NodeIdentity::getNodeID(size_t blacklist_len, const uint8_t *blacklist)
{

    uint8_t storedHash = HASH_NOT_SET;
    loadByteFromFile(NODE_ID_FILE, storedHash);

    if (storedHash != HASH_NOT_SET)
    {
        return storedHash;
    }

    String mac = getDeviceMAC();
    uint8_t macBytes[6];
    int values[6];
    sscanf(mac.c_str(), "%x:%x:%x:%x:%x:%x",
           &values[0], &values[1], &values[2],
           &values[3], &values[4], &values[5]);

    for (int i = 0; i < 6; ++i)
    {
        macBytes[i] = (uint8_t)values[i];
    }

    uint8_t hash = generateSafeHash(macBytes, 6, blacklist, blacklist_len);
    saveByteToFile(NODE_ID_FILE, hash);
    return hash;
}

uint8_t NodeIdentity::changeNodeID(const size_t blacklist_len, uint8_t *blacklist)
{
    String mac = getDeviceMAC();
    uint8_t mac_bytes[6];
    int values[6];
    sscanf(mac.c_str(), "%x:%x:%x:%x:%x:%x",
           &values[0], &values[1], &values[2],
           &values[3], &values[4], &values[5]);

    for (int i = 0; i < 6; ++i)
    {
        mac_bytes[i] = (uint8_t)values[i];
    }

    uint8_t hash = generateSafeHash(mac_bytes, 6, blacklist, blacklist_len);

    saveByteToFile(NODE_ID_FILE, hash);
    return hash;
}

// Carga un solo byte desde un archivo de LittleFS
bool NodeIdentity::loadByteFromFile(const char *filename, uint8_t &value)
{
    File file = LittleFS.open(filename, "r");
    if (!file)
    {
        // Serial.printf("NodeIdentity: Archivo '%s' no encontrado.\n", filename);
        return false;
    }

    if (file.available())
    {
        value = file.read(); // Lee el primer byte
        file.close();
        // Serial.printf("NodeIdentity: Valor %d cargado desde '%s'.\n", value, filename);
        return true;
    }
    // Serial.printf("NodeIdentity: Archivo '%s' vacío.\n", filename);
    file.close();
    return false;
}

// Guarda un solo byte en un archivo de LittleFS
void NodeIdentity::saveByteToFile(const char *filename, uint8_t value)
{
    File file = LittleFS.open(filename, "w"); // "w" sobrescribe el archivo existente
    if (!file)
    {
        // Serial.printf("NodeIdentity: Fallo al abrir archivo '%s' para escritura.\n", filename);
        return;
    }

    file.write(value); // Escribe el byte
    file.close();      // Cierra el archivo para asegurar que se guarde

    // Serial.printf("NodeIdentity: Valor %d guardado en '%s'.\n", value, filename);
}

uint8_t NodeIdentity::generateSafeHash(
    const uint8_t *data,
    size_t len,
    const uint8_t *blacklist,
    size_t blacklist_len)
{
    uint8_t hash = crc8(data, len);
    bool conflict = true;

    while (conflict)
    {
        conflict = false;
        for (size_t i = 0; i < blacklist_len; ++i)
        {
            if (hash == blacklist[i] && hash == 255) // valor prohibido prevee errores de faltar pasarlo en blacklist
            {
                hash = (hash + 1) % 256;
                conflict = true;
                break;
            }
        }
    }
    return hash;
}

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

bool NodeIdentity::getGetway(uint8_t &getwayAdress)
{
    uint8_t value = GETWAY_NOT_SET;
    if (loadByteFromFile(GATEWAY_ADDR_FILE, value) == false)
    {
        Serial.println("error en load memori");
        return false;
    }
    if (value != GETWAY_NOT_SET)
    {
        getwayAdress = value;
        return true;
    }
    return false;
}

void NodeIdentity::saveGetway(uint8_t getwayAdress)
{
    saveByteToFile(GATEWAY_ADDR_FILE, getwayAdress);
}

void NodeIdentity::begin()
{

    // --- ¡IMPORTANTE! Inicializar LittleFS una sola vez en setup() ---
    Serial.println("Inicializando LittleFS en sketch principal...");
    if (!LittleFS.begin())
    {
        Serial.println("Fallo al montar LittleFS. Intentando formatear...");
        // LittleFS.format(); // ¡Ten precaución! Esto borra todos los archivos.
        if (!LittleFS.begin())
        {
            Serial.println("Fallo crítico: LittleFS no se pudo montar.");
            return; // Detener la ejecución si LittleFS no está listo
        }
    }
}