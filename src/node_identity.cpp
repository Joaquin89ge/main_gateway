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
    DEBUG_PRINT("Inicializando ");
    begin();
}


/**
 * @brief Obtiene el identificador lógico del nodo
 * @param blacklist_len Tamaño de la lista negra
 * @param blacklist Valores prohibidos para el identificador
 * @return uint8_t Identificador entre 1-254 (0x00 y 0xFF reservados)
 * @details Genera un hash único o recupera el valor almacenado en LittleFS
 */
uint8_t NodeIdentity::getNodeID(size_t blacklist_len, const uint8_t *blacklist)
{
    DEBUG_PRINTLN("--- NodeIdentity::getNodeID() INICIO ---");
    DEBUG_PRINT("Parámetros recibidos: blacklist_len = ");
    DEBUG_PRINTLN(String(blacklist_len));
    // No imprimimos el 'blacklist' completo aquí, ya que puede ser largo.
    // Si necesitas depurarlo, puedes iterar sobre él y DEBUG_PRINTLN() cada byte.

    uint8_t storedHash = HASH_NOT_SET;
    DEBUG_PRINT("Intentando cargar NODE_ID desde el archivo '");
    DEBUG_PRINT(NODE_ID_FILE);
    DEBUG_PRINT("'. Valor inicial: 0x");
    DEBUG_PRINTLN(String(storedHash, HEX));

    // Llama a la función para cargar el byte desde el archivo
    loadByteFromFile(NODE_ID_FILE, storedHash);

    DEBUG_PRINT("Valor cargado desde archivo: 0x");
    DEBUG_PRINTLN(String(storedHash, HEX));
    DEBUG_PRINT("HASH_NOT_SET es: 0x");
    DEBUG_PRINTLN(String(HASH_NOT_SET, HEX));


    if (storedHash != HASH_NOT_SET && storedHash != 0)
    {
        DEBUG_PRINTLN("HASH encontrado en el archivo (no es HASH_NOT_SET).");
        DEBUG_PRINT("Devolviendo ID almacenado: 0x");
        DEBUG_PRINTLN(String(storedHash, HEX));
        DEBUG_PRINTLN("--- NodeIdentity::getNodeID() FIN (ID existente) ---");
        return storedHash; // Si hay un hash guardado, lo devuelve
    }

    DEBUG_PRINTLN("No hay HASH almacenado o es HASH_NOT_SET. Generando nuevo ID...");

    String mac = getDeviceMAC();
    DEBUG_PRINT("MAC del dispositivo obtenida: ");
    DEBUG_PRINTLN(mac);

    uint8_t macBytes[6];
    int values[6];

    // Intenta parsear la MAC del string a valores hexadecimales
    int sscanfResult = sscanf(mac.c_str(), "%x:%x:%x:%x:%x:%x",
                              &values[0], &values[1], &values[2],
                              &values[3], &values[4], &values[5]);

    DEBUG_PRINT("sscanf(): Numero de items convertidos: ");
    DEBUG_PRINTLN(String(sscanfResult));

    // Verifica si sscanf fue exitoso (debe convertir 6 valores)
    if (sscanfResult != 6) {
        DEBUG_PRINTLN("ERROR: Fallo al parsear la MAC. Se generara un hash con MAC invalida.");
        // Considera aquí un manejo de error más robusto, como retornar un ID por defecto o reiniciar.
    } else {
        DEBUG_PRINT("Bytes MAC parseados (HEX): ");
        for (int i = 0; i < 6; ++i)
        {
            macBytes[i] = (uint8_t)values[i];
            DEBUG_PRINT(String(macBytes[i], HEX));
            DEBUG_PRINT(" ");
        }
        DEBUG_PRINTLN(""); // Nueva línea después de imprimir los bytes
    }


    uint8_t hash = generateSafeHash(macBytes, 6, blacklist, blacklist_len);

    DEBUG_PRINT("Hash generado: 0x");
    DEBUG_PRINTLN(String(hash, HEX));

    // Guarda el nuevo hash generado en el archivo
    saveByteToFile(NODE_ID_FILE, hash);
    DEBUG_PRINT("Nuevo ID (0x");
    DEBUG_PRINT(String(hash, HEX));
    DEBUG_PRINTLN(") guardado en el archivo.");

    DEBUG_PRINTLN("--- NodeIdentity::getNodeID() FIN (Nuevo ID generado) ---");
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
    uint8_t value = GETWAY_NOT_SET;
    if (loadByteFromFile(GATEWAY_ADDR_FILE, value) == false)
    {
        DEBUG_PRINTLN("error en load memori");
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
    DEBUG_PRINTLN("Inicializando LittleFS en sketch principal...");
    if (!LittleFS.begin())
    {
        DEBUG_PRINTLN("Fallo al montar LittleFS. Intentando formatear...");
        // LittleFS.format(); // ¡Ten precaución! Esto borra todos los archivos.
        if (!LittleFS.begin())
        {
            DEBUG_PRINTLN("Fallo crítico: LittleFS no se pudo montar.");
            return; // Detener la ejecución si LittleFS no está listo
        }
    }
}