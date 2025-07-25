// RadioManager.cpp
#include "radio_manager.h"

/**
 * @brief Pin Chip Select (CS) para el módulo LoRa RFM95.
 *
 * Este pin se utiliza para habilitar/deshabilitar la comunicación con el módulo RFM95
 * a través del bus SPI. Corresponde al pin digital D3 en algunas placas de desarrollo.
 */
#define RFM95_CS 2 // d4

/**
 * @brief Pin de Interrupción (INT) para el módulo LoRa RFM95.
 *
 * Este pin es utilizado por el módulo RFM95 para señalar eventos (por ejemplo, paquete recibido)
 * al microcontrolador. Corresponde al pin digital D1 en algunas placas de desarrollo.
 */
#define RFM95_INT 5 // d1

/**
 * @brief Constructor de RadioManager
 * @param address Dirección de red única para este nodo
 * 
 * @details Inicializa el driver RFM95 y el gestor de red RHMesh
 * @note La dirección es utilizada por RHMesh para enrutar mensajes
 */
RadioManager::RadioManager(uint8_t address)
    : driver(RFM95_CS, RFM95_INT), manager(driver, address)
{
    init();
}

/**
 * @brief Inicializa el módulo de radio RFM95 y el gestor de red RHMesh
 * @return true si la inicialización fue exitosa, false en caso contrario
 * 
 * @details Configura el módulo RFM95 y inicializa RHMesh para comunicación mesh
 */
bool RadioManager::init()
{
   if (!manager.init())
    DEBUG_PRINT("RF95 MESH init failed");
DEBUG_PRINT("RF95 MESH init okay");
 
    return true;
}

/**
 * @brief Envía un mensaje a un nodo específico
 * @param to Dirección del nodo destino
 * @param data Datos a enviar
 * @param len Longitud de los datos
 * @param flag Flags del mensaje
 * @return true si el envío fue exitoso, false en caso contrario
 * 
 * @details Envía mensaje unicast con espera de ACK
 * @note Timeout configurado en config.h
 */
bool RadioManager::sendMessage(uint8_t to, uint8_t *data, uint8_t len, uint8_t flag)
{
    // Envía el mensaje y espera un acuse de recibo.
    // RH_ROUTER_ERROR_NONE indica una transmisión y acuse de recibo exitosos.
    if (manager.sendtoWait(data, len, to, flag) == RH_ROUTER_ERROR_NONE)
    {
        return true;
    }
    return false;
}

/**
 * @brief Recibe un mensaje de cualquier nodo
 * @param buf Buffer para almacenar los datos recibidos
 * @param len Puntero a longitud de datos recibidos
 * @param from Puntero a dirección del nodo origen
 * @param flag Puntero a flags del mensaje
 * @return true si se recibió un mensaje, false en caso contrario
 * 
 * @details Recibe mensaje con ACK automático
 * @note No bloqueante, retorna inmediatamente si no hay mensajes
 */
bool RadioManager::recvMessage(uint8_t *buf, uint8_t *len, uint8_t *from, uint8_t *flag)
{
    uint8_t dest;
    uint8_t id;
    // Intenta recibir un mensaje reconocido.
    if (manager.recvfromAck(buf, len, from, &dest, &id, flag))
    {
        return true; // Mensaje recibido con éxito.
    }
    return false; // No se recibió ningún mensaje o falló el acuse de recibo.
}

/**
 * @brief Recibe un mensaje con timeout
 * @param buf Buffer para almacenar los datos recibidos
 * @param len Puntero a longitud de datos recibidos
 * @param from Puntero a dirección del nodo origen
 * @param flag Puntero a flags del mensaje
 * @param timeout Tiempo de espera en milisegundos
 * @return true si se recibió un mensaje, false en caso contrario
 * 
 * @details Recibe mensaje con timeout y ACK automático
 * @note Bloqueante hasta timeout o mensaje recibido
 */
bool RadioManager::recvMessageTimeout(uint8_t *buf, uint8_t *len, uint8_t *from, uint8_t *flag, uint16_t timeout)
{
    //uint8_t dest;      // Variable interna para la dirección de destino
    //uint8_t messageId; // Variable interna para el ID del mensaje de RadioHead

    // Llama a manager.recvfromAckTimeout con los parámetros adecuados
    // Los punteros 'from', 'dest', 'messageId' y 'flags' deben ser proporcionados a la función.
    // En tu caso, necesitas el 'from' y el 'flag' que se pasan,
    // y 'dest' y 'messageId' se pueden usar como variables temporales internas.
    // La función recvfromAckTimeout es:
    // bool recvfromAckTimeout(uint8_t* buf, uint8_t* len, uint16_t timeout, uint8_t* from = NULL, uint8_t* dest = NULL, uint8_t* id = NULL, uint8_t* flags = NULL);

    // Aquí, 'flag' del parámetro de tu función se mapea a 'flags' de recvfromAckTimeout
    // y 'from' de tu función se mapea a 'from' de recvfromAckTimeout.
    // 'dest' y 'messageId' son variables temporales que no necesitas devolver.
    if (manager.recvfromAckTimeout(buf, len, timeout, from, NULL, NULL, flag))
    {
        // Mensaje recibido y reconocido con éxito dentro del tiempo
        return true;
    }
    // No se recibió un mensaje reconocido dentro del tiempo
    return false;
}

/**
 * @brief Método de marcador de posición para actualizar el estado interno del gestor de radio.
 *
 * Actualmente, la librería RHMesh maneja la mayoría de sus procesos internos (como
 * retransmisión, actualizaciones de la tabla de enrutamiento y manejo de acuses de recibo) automáticamente.
 * Este método se proporciona como un marcador de posición vacío para futuras expansiones o
 * si son necesarias tareas periódicas específicas relacionadas con la gestión de radio.
 * Por ejemplo, podría usarse para la gestión de energía personalizada, sondeo de estado,
 * o lógica de enrutamiento avanzada si el comportamiento predeterminado de RHMesh es insuficiente.
 */
void RadioManager::update()
{
    // RHMesh maneja internamente el reintento y enrutamiento.
    // Este método se reserva para futuras ampliaciones si se requiere.
} 