// RadioManager.h
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
 */
class RadioManager
{
public:
    /**
     * @brief Constructor de la clase RadioManager.
     * @param address La dirección de red única para este nodo de radio.
     * Esta dirección es utilizada por RHMesh para enrutar mensajes.
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
     */
    bool recvMessageTimeout(uint8_t *buf, uint8_t *len, uint8_t *from, uint8_t *flag, uint16_t timeout);
    /**
     * @brief Actualiza el estado interno del gestor de radio.
     *
     * Este método está destinado a tareas periódicas o para permitir que la librería RHMesh
     * realice su procesamiento interno (por ejemplo, actualizaciones de la tabla de enrutamiento, retransmisiones).
     * Actualmente, RHMesh maneja la mayoría de estos aspectos internamente, por lo que este método
     * se reserva para futuras extensiones o lógica personalizada.
     */
    void update();

private:
    RH_RF95 driver; /**< @brief La instancia del controlador de radio RFM95. Maneja las operaciones de radio de bajo nivel. */
    RHMesh manager; /**< @brief La instancia del gestor de red RHMesh. Maneja el enrutamiento y el direccionamiento. */
};

#endif // RADIO_MANAGER_H
