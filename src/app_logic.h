// 03:00 16/6/2025
//  AppLogic.h
#ifndef APP_LOGIC_H
#define APP_LOGIC_H
// hoola
#include "node_identity.h"  // Para NodeIdentity (dirección MAC, clave)
#include "radio_manager.h"  // Para RadioManager (gestión de radio LoRa)
#include "protocol.h"       // Para Protocol (serialización/deserialización de mensajes)
#include "sensor_manager.h" // Para GetData (obtención de datos de sensores)
/**
 * @class AppLogic
 * @brief Clase que encapsula la lógica de aplicación para un nodo sensor.
 *
 * Esta clase orquesta la interacción entre los diferentes módulos (identidad del nodo,
 * gestión de radio, protocolo de comunicación y adquisición de datos) para implementar
 * el comportamiento de un nodo sensor. Su rol principal es gateway por ende envia announchers
 * solicita data y escucha hello.
 */

#define MAX_NODES 250
#define NUMERO_MUESTRAS_ATMOSFERICAS 8

class AppLogic
{
private:

    NodeIdentity nodeIdentity;                      /**< @brief Referencia al objeto NodeIdentity para gestionar la identidad del nodo. */
    RadioManager radio;                             /**< @brief Referencia al objeto RadioManager para la comunicación por radio. */
    uint8_t gatewayAddress;                         /**< @brief Dirección de red del Gateway con el que este sensor se comunica. */
    uint8_t nodeID[MAX_NODES];                      /**< @brief Direcciónes de red de nodos conocidos. */
    const unsigned long intervaloAnnounce = 120000; /**< @brief Intervalo en milisegundos (120 segundo)*/
    unsigned long temBuf = 0;                       // bufer guarda tiempo

    // implementar logica de envio y recepcion datos

    void sendAnnounce(uint8_t *buf, uint8_t len, uint8_t from);

    /**
     * @brief Envía un mensaje HELLO al Gateway.
     *
     * Este método es llamado por el nodo sensor al iniciar para anunciarse
     * a la red y al Gateway.
     */
    void handleHello();

    void handleAtmosphericData();
    void handleGroungGpsData();

    void sendChangeID(uint8_t *buf, uint8_t len);

public:
    AtmosphericSample atmosSamples[NUMERO_MUESTRAS_ATMOSFERICAS]; // Ejemplo: array para 8 muestras
    GroundSensor groundData;
    GpsSensor gpsData;
    /**
     * @brief Constructor de la clase AppLogic para un nodo sensor.
     *
     * @param identity Referencia al objeto NodeIdentity para la gestión de la identidad del nodo.
     * @param radioMgr Referencia al objeto RadioManager para la comunicación por radio.
     */
    AppLogic(NodeIdentity identity, RadioManager radioMgr);

    /**
     * @brief Inicia la lógica de la aplicación del nodo sensor.
     *
     * Este método se llama una vez al inicio del programa para realizar
     * cualquier configuración inicial necesaria, como enviar un mensaje HELLO
     * para registrarse en el Gateway.
     */
    void begin();

    /**
     * @brief Actualiza la lógica de la aplicación en cada ciclo del bucle principal.
     *
     * Este método debe ser llamado repetidamente en el bucle principal (`loop()`)
     * del programa. Se encarga de verificar si hay mensajes entrantes, parsearlos
     * y delegar su manejo a las funciones `handle*` apropiadas (actualmente solo `handleRequestData`).
     */
    void update();
};

#endif // APP_LOGIC_H
