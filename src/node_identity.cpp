/**
 * @file node_identity.cpp
 * @brief Implementación de identidad de nodo con persistencia EEPROM
 * @date 22:00 17/6/2025
 */

#include "node_identity.h"

// La definición de defaultBlacklist ahora está en el header como static const

/**
 * @brief Constructor de NodeIdentity
 * @details Inicializa el sistema de identidad y llama a begin()
 */
NodeIdentity::NodeIdentity()
{
    Serial.printf("Inicializando \n");
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

    // Comentado: Código de LittleFS
    /*
    uint8_t storedHash = HASH_NOT_SET;
    Serial.printf("Intentando cargar NODE_ID desde el archivo '");
    Serial.printf(NODE_ID_FILE);
    Serial.printf("'. Valor inicial: 0x");
    Serial.printf(String(storedHash, HEX));

    // Llama a la función para cargar el byte desde el archivo
    loadByteFromFile(NODE_ID_FILE, storedHash);

    Serial.printf("Valor cargado desde archivo: 0x");
    Serial.printf(storedHash, HEX);
    Serial.printf("HASH_NOT_SET es: 0x");
    Serial.printf(HASH_NOT_SET, HEX);

    if (storedHash != HASH_NOT_SET && storedHash != 0)
    {
        Serial.printf("HASH encontrado en el archivo (no es HASH_NOT_SET).\n");
        Serial.printf("Devolviendo ID almacenado: 0x");
        Serial.printf(storedHash, HEX);
        Serial.printf("--- NodeIdentity::getNodeID() FIN (ID existente) ---\n");
        return storedHash; // Si hay un hash guardado, lo devuelve
    }
    */

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
        Serial.printf("\n"); // Nueva línea después de imprimir los bytes
    }

    uint8_t hash = generateSafeHash(macBytes, 6, blacklist, blacklist_len);

    Serial.printf("Hash generado: 0x%02X\n", hash);

    // Comentado: Guardado en LittleFS
    /*
    // Guarda el nuevo hash generado en el archivo
    saveByteToFile(NODE_ID_FILE, hash);
    Serial.printf("Nuevo ID (0x%02X) guardado en el archivo.\n", hash);
    */

    Serial.printf("--- NodeIdentity::getNodeID() FIN (Nuevo ID generado) ---\n");
    return hash; // Devuelve el hash recién generado
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

    // Comentado: Guardado en LittleFS
    // saveByteToFile(NODE_ID_FILE, hash);
    return hash;
}

// Comentado: Carga un solo byte desde un archivo de LittleFS
/*
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
*/

// Comentado: Guarda un solo byte en un archivo de LittleFS
/*
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
*/

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
            if (hash == blacklist[i] || hash == 255) // valor prohibido prevee errores de faltar pasarlo en blacklist
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
    // Comentado: Código de LittleFS
    /*
    uint8_t value = GETWAY_NOT_SET;
    if (loadByteFromFile(GATEWAY_ADDR_FILE, value) == false)
    {
        Serial.printf("error en load memori\n");
        return false;
    }
    if (value != GETWAY_NOT_SET)
    {
        getwayAdress = value;
        return true;
    }
    */
    
    // Por ahora, retornar false ya que no hay persistencia
    Serial.printf("Gateway no configurado (LittleFS deshabilitado)\n");
    return false;
}

void NodeIdentity::saveGetway(uint8_t getwayAdress)
{
    // Comentado: Guardado en LittleFS
    // saveByteToFile(GATEWAY_ADDR_FILE, getwayAdress);
    Serial.printf("Gateway address no guardado (LittleFS deshabilitado): 0x%02X\n", getwayAdress);
}

void NodeIdentity::begin()
{
    // Comentado: Inicialización de LittleFS
    /*
    // --- ¡IMPORTANTE! Inicializar LittleFS una sola vez en setup() ---
    Serial.printf("Inicializando LittleFS en sketch principal...\n");
    if (!LittleFS.begin())
    {
        Serial.printf("Fallo al montar LittleFS. Intentando formatear...\n");
        // LittleFS.format(); // ¡Ten precaución! Esto borra todos los archivos.
        if (!LittleFS.begin())
        {
            Serial.printf("Fallo crítico: LittleFS no se pudo montar.\n");
            return; // Detener la ejecución si LittleFS no está listo
        }
    }
    */
    
    Serial.printf("NodeIdentity inicializado (LittleFS deshabilitado)\n");
}