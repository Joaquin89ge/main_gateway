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
 * el comportamiento de un nodo sensor. Su rol principal es enviar datos y responder
 * a solicitudes de un Gateway.
 */
class AppLogic
{
private:
    SensorManager getData;                      /**< @brief Referencia al objeto SensorManager para obtener datos de sensores. */
    NodeIdentity nodeIdentity;                  /**< @brief Referencia al objeto NodeIdentity para gestionar la identidad del nodo. */
    RadioManager radio;                         /**< @brief Referencia al objeto RadioManager para la comunicación por radio. */
    uint8_t gatewayAddress;                     /**< @brief Dirección de red del Gateway con el que este sensor se comunica. */
    uint8_t nodeID;                             /**< @brief Dirección de red del nodo. */
    bool gatwayRegistred = false;               /**< @brief Flag que establece el registro de un getway por ende recepcion exitosa de un anunche. */
    const unsigned long intervaloHello = 60000; /**< @brief Intervalo en milisegundos (60 segundo)*/
    unsigned long temBuf = 0;                   // bufer guarda tiempo

    // implementar logica de envio y recepcion datos

    void handleAnnounce(uint8_t *buf, uint8_t len, uint8_t from);

    /**
     * @brief Envía un mensaje HELLO al Gateway.
     *
     * Este método es llamado por el nodo sensor al iniciar para anunciarse
     * a la red y al Gateway.
     */
    void sendHello();

    void sendAtmosphericData();
    void sendGroungGpsData();

    // envia daros fragmentados por ende necesito vector string y ademas da tiempo bloqueante (delay()) al gatway para procesado de datos
    bool sendFragmentData(const std::vector<String> &stringList);

    void changeID(uint8_t *buf, uint8_t len);

public:
    /**
     * @brief Constructor de la clase AppLogic para un nodo sensor.
     *
     * @param identity Referencia al objeto NodeIdentity para la gestión de la identidad del nodo.
     * @param radioMgr Referencia al objeto RadioManager para la comunicación por radio.
     * @param data Referencia al objeto SensorManager para la adquisición de datos de sensores.
     * @param gwAddress La dirección de red predefinida o conocida del Gateway.
     */
    AppLogic(SensorManager data, NodeIdentity identity, RadioManager radioMgr);

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
