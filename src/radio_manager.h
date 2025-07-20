/**
 * @file radio_manager.h
 * @brief Gestión de comunicación LoRa para red mesh agrícola
 * @author Joaquin89ge
 * @date 2025-01-20
 * @version 1.0
 * 
 * @details Esta clase proporciona una capa de abstracción para la comunicación
 * LoRa utilizando el módulo RFM95/SX1278 y la librería RadioHead RHMesh.
 * Maneja inicialización, envío y recepción de mensajes en la red mesh.
 * 
 * @see https://www.airspayce.com/mikem/arduino/RadioHead/
 * @see https://github.com/mikem/RadioHead
 */

#ifndef RADIO_MANAGER_H
#define RADIO_MANAGER_H

#include <RHMesh.h>
#include <RH_RF95.h>
#include <SPI.h>
#include "config.h"

/**
 * @class RadioManager
 * @brief Gestiona la comunicación de radio LoRa utilizando el módulo RFM95 y la librería RHMesh.
 *
 * Esta clase proporciona una capa de abstracción para inicializar, enviar y recibir
 * mensajes a través de una red LoRa. Utiliza el controlador RH_RF95 de la librería RadioHead
 * para la interacción con el hardware y RHMesh para las capacidades de enrutamiento de red.
 * 
 * @details La clase maneja:
 * - Inicialización del módulo RFM95
 * - Configuración de frecuencia y potencia
 * - Envío y recepción de mensajes con ACK
 * - Gestión de timeouts y reintentos
 * 
 * @note Requiere configuración de pines SPI en config.h
 * @see RFM95_CS, RFM95_INT
 * 
 * @example
 * ```cpp
 * #include "radio_manager.h"
 * 
 * RadioManager radio(0x42);
 * if (radio.init()) {
 *     uint8_t data[] = {0x01, 0x02, 0x03};
 *     radio.sendMessage(0x43, data, 3, 0x01);
 * }
 * ```
 */
class RadioManager
{
public:
    /**
     * @brief Constructor de la clase RadioManager.
     * @param address La dirección de red única para este nodo de radio.
     * Esta dirección es utilizada por RHMesh para enrutar mensajes.
     * 
     * @details Inicializa el driver RFM95 y el gestor de red RHMesh
     * @note Direcciones válidas: 1-254 (0 y 255 reservados)
     * 
     * @example
     * ```cpp
     * NodeIdentity identity;
     * uint8_t nodeId = identity.getNodeID();
     * RadioManager radio(nodeId);
     * ```
     */
    RadioManager(uint8_t address);

    /**
     * @brief Inicializa el módulo de radio RFM95 y el gestor de red RHMesh.
     *
     * Este método realiza la configuración de hardware necesaria para el módulo RFM95,
     * incluyendo la configuración de los modos de pin y una secuencia de reinicio.
     * Luego inicializa el gestor RHMesh y configura la frecuencia y la potencia de transmisión de la radio.
     *
     * @return Verdadero si la inicialización fue exitosa, falso en caso contrario.
     * 
     * @example
     * ```cpp
     * RadioManager radio(0x42);
     * if (radio.init()) {
     *     Serial.println("Radio inicializada");
     * } else {
     *     Serial.println("Error al inicializar radio");
     * }
     * ```
     */
    bool init();

    /**
     * @brief Envía un mensaje a una dirección de destino especificada a través de la red LoRa.
     *
     * Esta función utiliza el método `sendtoWait` de RHMesh para transmitir datos.
     * Se bloqueará hasta que se reciba un acuse de recibo o ocurra un tiempo de espera,
     * manejando las retransmisiones y el enrutamiento internamente.
     *
     * @param to La dirección de red de destino del nodo receptor.
     * @param data Un puntero al array de bytes que contiene el mensaje a enviar.
     * @param len La longitud del mensaje en bytes.
     * @param flag El MessageType del protocolo ver "protocolo.h"
     * .
     * @return Verdadero si el mensaje fue enviado y reconocido con éxito, falso en caso contrario.
     * 
     * @example
     * ```cpp
     * uint8_t message[] = {0x01, 0x02, 0x03};
     * if (radio.sendMessage(0x42, message, 3, Protocol::MessageType::DATA)) {
     *     Serial.println("Mensaje enviado exitosamente");
     * } else {
     *     Serial.println("Error al enviar mensaje");
     * }
     * ```
     * 
     * @see Protocol::MessageType
     */
    bool sendMessage(uint8_t to, uint8_t *data, uint8_t len, uint8_t flag);

    /**
     * @brief Intenta recibir un mensaje de la red LoRa.
     *
     * Este método verifica si hay mensajes entrantes e intenta recibir un paquete reconocido.
     * Si se recibe un mensaje, su contenido, longitud y dirección del remitente se
     * completan en los búferes proporcionados.
     *
     * @param buf Un puntero a un array de bytes donde se almacenará el mensaje recibido.
     * @param len Un puntero a un uint8_t donde se almacenará la longitud del mensaje recibido.
     * Al llamar, debe contener el tamaño máximo del búfer; al regresar, contiene la longitud real.
     * @param from Un puntero a un uint8_t donde se almacenará la dirección de red del remitente.
     * @param flag Un puntero a un uint8_t donde se almacenará el MessageType del protocolo
     * @see "protocolo.h".
     * @return Verdadero si un mensaje fue recibido y reconocido con éxito, falso en caso contrario.
     * 
     * @example
     * ```cpp
     * uint8_t buffer[50];
     * uint8_t len = sizeof(buffer), from, flag;
     * 
     * if (radio.recvMessage(buffer, &len, &from, &flag)) {
     *     Serial.printf("Mensaje de 0x%02X: %d bytes, flag: 0x%02X\n", from, len, flag);
     *     // Procesar buffer[0] a buffer[len-1]
     * }
     * ```
     */
    bool recvMessage(uint8_t *buf, uint8_t *len, uint8_t *from, uint8_t *flag);

    /**
     * @brief Intenta recibir un mensaje de la red LoRa con un tiempo de espera.
     *
     * Este método verifica si hay mensajes entrantes e intenta recibir un paquete
     * reconocido dentro del tiempo de espera especificado. Si se recibe un mensaje,
     * su contenido, longitud, dirección del remitente y flag de protocolo se
     * completan en los búferes proporcionados.
     *
     * @param buf Un puntero a un array de bytes donde se almacenará el mensaje recibido.
     * @param len Un puntero a un uint8_t donde se almacenará la longitud del mensaje recibido.
     * Al llamar, debe contener el tamaño máximo del búfer; al regresar, contiene la longitud real.
     * @param from Un puntero a un uint8_t donde se almacenará la dirección de red del remitente.
     * @param flag Un puntero a un uint8_t donde se almacenará el MessageType del protocolo @see "protocolo.h" .
     * @param timeout El tiempo máximo en milisegundos que se esperará por un mensaje.
     * @return Verdadero si un mensaje fue recibido y reconocido con éxito dentro del tiempo, falso en caso contrario.
     * 
     * @example
     * ```cpp
     * uint8_t buffer[50];
     * uint8_t len = sizeof(buffer), from, flag;
     * 
     * if (radio.recvMessageTimeout(buffer, &len, &from, &flag, 1000)) {
     *     Serial.printf("Mensaje recibido en 1s: %d bytes\n", len);
     * } else {
     *     Serial.println("Timeout - no se recibió mensaje");
     * }
     * ```
     */
    bool recvMessageTimeout(uint8_t *buf, uint8_t *len, uint8_t *from, uint8_t *flag, uint16_t timeout);

    /**
     * @brief Actualiza el estado interno del gestor de radio.
     *
     * Este método está destinado a tareas periódicas o para permitir que la librería RHMesh
     * realice su procesamiento interno (por ejemplo, actualizaciones de la tabla de enrutamiento, retransmisiones).
     * Actualmente, RHMesh maneja la mayoría de estos aspectos internamente, por lo que este método
     * se reserva para futuras extensiones o lógica personalizada.
     * 
     * @note Debe llamarse regularmente en el loop principal
     * 
     * @example
     * ```cpp
     * void loop() {
     *     radio.update();
     *     // Otro código...
     * }
     * ```
     */
    void update();

private:
    RH_RF95 driver; /**< @brief La instancia del controlador de radio RFM95. Maneja las operaciones de radio de bajo nivel. */
    RHMesh manager; /**< @brief La instancia del gestor de red RHMesh. Maneja el enrutamiento y el direccionamiento. */
};
 
#endif // RADIO_MANAGER_H
