// RadioManager.h
#ifndef RADIO_MANAGER_H
#define RADIO_MANAGER_H

#include <RHMesh.h>
#include <RH_RF95.h>
#include <SPI.h>
#include "config.h"

/**
 * @class RadioManager
 * @brief Gestiona la comunicación LoRa en red mesh para un nodo agrícola.
 *
 * Esta clase abstrae la inicialización, envío y recepción de mensajes LoRa usando RadioHead RHMesh y RH_RF95.
 * Permite la integración de nodos sensores en una red mesh robusta y auto-configurable, facilitando la transmisión eficiente de datos al gateway central.
 *
 * Forma parte de la arquitectura mesh agrícola, asegurando la comunicación resiliente y el enrutamiento dinámico de mensajes entre nodos y el gateway.
 */
class RadioManager
{
public:
    /**
     * @brief Constructor de RadioManager.
     * @param address Dirección de red única para este nodo (usada por RHMesh).
     */
    RadioManager(uint8_t address);

    /**
     * @brief Inicializa el módulo LoRa y la red mesh.
     * Configura pines, reinicia el módulo y prepara RHMesh para operar.
     * @return true si la inicialización fue exitosa, false en caso contrario.
     */
    bool init();

    /**
     * @brief Envía un mensaje a un nodo destino a través de la red mesh LoRa.
     * @param to Dirección del nodo destino.
     * @param data Puntero al buffer de datos a enviar.
     * @param len Longitud del mensaje en bytes.
     * @param flag Tipo de mensaje/protocolo (ver protocol.h).
     * @return true si el mensaje fue enviado y reconocido, false en caso contrario.
     */
    bool sendMessage(uint8_t to, uint8_t *data, uint8_t len, uint8_t flag);

    /**
     * @brief Intenta recibir un mensaje de la red mesh LoRa.
     * @param buf Buffer donde se almacenará el mensaje recibido.
     * @param len Puntero a la longitud máxima; al regresar, contiene la longitud real.
     * @param from Puntero a la dirección del remitente.
     * @param flag Puntero al tipo de mensaje/protocolo recibido.
     * @return true si se recibió un mensaje válido, false en caso contrario.
     */
    bool recvMessage(uint8_t *buf, uint8_t *len, uint8_t *from, uint8_t *flag);

    /**
     * @brief Intenta recibir un mensaje con timeout.
     * @param buf Buffer donde se almacenará el mensaje recibido.
     * @param len Puntero a la longitud máxima; al regresar, contiene la longitud real.
     * @param from Puntero a la dirección del remitente.
     * @param flag Puntero al tipo de mensaje/protocolo recibido.
     * @param timeout Tiempo máximo de espera en milisegundos.
     * @return true si se recibió un mensaje válido dentro del tiempo, false en caso contrario.
     */
    bool recvMessageTimeout(uint8_t *buf, uint8_t *len, uint8_t *from, uint8_t *flag, uint16_t timeout);

    /**
     * @brief Actualiza el estado interno del gestor de radio (placeholder para futuras extensiones).
     */
    void update();
    
    /**
     * @brief Incrementa el contador de fallos y resetea el módulo si es necesario.
     * @return true si se realizó un reset, false en caso contrario
     */
    bool handleTransmissionFailure();
    
    /**
     * @brief Resetea el contador de fallos (llamar después de un envío exitoso).
     */
    void resetFailureCounter();
    
    /**
     * @brief Obtiene el número actual de fallos consecutivos.
     * @return Número de fallos consecutivos
     */
    uint8_t getFailureCount() const;
    
    /**
     * @brief Fuerza un reset del módulo radio.
     */
    void forceRadioReset();

private:
    RH_RF95 driver;  ///< Controlador de radio LoRa (bajo nivel)
    RHMesh manager;  ///< Gestor de red mesh (enrutamiento y lógica mesh)
    uint8_t failureCount;  ///< Contador de fallos consecutivos
};

#endif // RADIO_MANAGER_H
