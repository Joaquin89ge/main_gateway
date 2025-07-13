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
 * @brief Frecuencia LoRa configurada en MHz.
 *
 * Este define establece la frecuencia de operación para la comunicación LoRa.
 * Debe elegirse en función de las regulaciones regionales y otros nodos de la red.
 */
#define FRECUENCI_CONF 415.0

/**
 * @brief Nivel de potencia de transmisión para LoRa en dBm.
 *
 * Este define establece la potencia de transmisión del módulo LoRa.
 * Un valor más alto generalmente significa un mayor alcance pero consume más energía.
 * La potencia máxima para el RFM95 es típicamente de 20 dBm.
 */
#define POWER_LORA 23

RadioManager::RadioManager(uint8_t address)
    : driver(RFM95_CS, RFM95_INT), manager(driver, address)
{
    init();
}

bool RadioManager::init()
{
    while (!driver.init())
    {

        DEBUG_PRINTLN("LoRa radio init failed");

        delay(100);
    }

    DEBUG_PRINTLN("LoRa radio init OK!");

    if (!driver.setFrequency(FRECUENCI_CONF))
    {

        DEBUG_PRINTLN("setFrequency failed");

        delay(100);
    }

    DEBUG_PRINT("Set Freq to: ");
    DEBUG_PRINTLN(FRECUENCI_CONF);
    driver.setTxPower(POWER_LORA, false);
    return true;
}
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

bool RadioManager::recvMessageTimeout(uint8_t *buf, uint8_t *len, uint8_t *from, uint8_t *flag, uint16_t timeout)
{
    uint8_t dest;      // Variable interna para la dirección de destino
    uint8_t messageId; // Variable interna para el ID del mensaje de RadioHead

    // Llama a manager.recvfromAckTimeout con los parámetros adecuados
    // Los punteros 'from', 'dest', 'messageId' y 'flags' deben ser proporcionados a la función.
    // En tu caso, necesitas el 'from' y el 'flag' que se pasan,
    // y 'dest' y 'messageId' se pueden usar como variables temporales internas.
    // La función recvfromAckTimeout es:
    // bool recvfromAckTimeout(uint8_t* buf, uint8_t* len, uint16_t timeout, uint8_t* from = NULL, uint8_t* dest = NULL, uint8_t* id = NULL, uint8_t* flags = NULL);

    // Aquí, 'flag' del parámetro de tu función se mapea a 'flags' de recvfromAckTimeout
    // y 'from' de tu función se mapea a 'from' de recvfromAckTimeout.
    // 'dest' y 'messageId' son variables temporales que no necesitas devolver.
    if (manager.recvfromAckTimeout(buf, len, timeout, from, &dest, &messageId, flag))
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
