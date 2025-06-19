// 03:00 16/6/2025
//  AppLogic.h
#ifndef APP_LOGIC_H
#define APP_LOGIC_H
// hoola
#include "node_identity.h"  // Para NodeIdentity (dirección MAC, clave)
#include "radio_manager.h"  // Para RadioManager (gestión de radio LoRa)
#include "protocol.h"       // Para Protocol (serialización/deserialización de mensajes)
#include "sensor_manager.h" // Para GetData (obtención de datos de sensores)
#include <vector>
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
#define CANTIDAD_PAQUETES_ALMACENAR
#define CANTIDAD_MUESTRAS_SUELO 2
class AppLogic
{
private:
    NodeIdentity nodeIdentity; /**< @brief Referencia al objeto NodeIdentity para gestionar la identidad del nodo. */
    RadioManager radio;        /**< @brief Referencia al objeto RadioManager para la comunicación por radio. */

    uint8_t gatewayAddress; /**< @brief Dirección de red del Gateway con el que este sensor se comunica. */

    uint8_t nodeIDs[MAX_NODES]; /**< @brief Direcciónes de red de nodos conocidos. */
    bool nodesRegistred = false;
    uint8_t counterNodes = 0; /**< @brief contador de red de nodos conocidos. */

    const unsigned long intervaloAnnounce = 120000;               /**< @brief Intervalo en milisegundos (2 min) para announce*/
    const unsigned long intervaloAtmospheric = 480000;            /**< @brief Intervalo en milisegundos (8 minutos) para request atmospheric*/
    const int horasGroundGps[CANTIDAD_MUESTRAS_SUELO] = {12, 24}; /**< @brief horas para request data ground y gps*/
    unsigned long temBuf = 0;                                     /**< @brief bufer guarda tiempo */
    unsigned long temBuf1 = 0;                                    /**< @brief  bufer guarda tiempo1 */

    const uint16_t timeoutGral = 1500;   /**< @brief timeout para espera recepcion de datos se usa en requestAmospheric y grondGps entre otros */
    const uint8_t connectionRetries = 2; /**< @brief reintentos para pedir nuevamente datos, alcanzaaod el maximo se agrega la direccion del nodo a nodesDown*/

    bool updatedData = false; /**< @brief  flag de datos actualizados, se pone en true luego de un requestAtmosphericData o requestGroundGpsData */

    // implementar logica de envio y recepcion datos

    void sendAnnounce();

    /**
     * @brief Envía un mensaje HELLO al Gateway.
     *
     * Este método es llamado por el nodo sensor al iniciar para anunciarse
     * a la red y al Gateway.
     */
    void handleHello();

    void requestAtmosphericData();
    void requestGroundGpsData();
    void handleUartRequest();
    void sendChangeID(uint8_t from);
    void timer();
    bool compareHsAndMs();

public:
    GroundGpsPacket groundGpsPacket[CANTIDAD_MUESTRAS_SUELO];
    // Un vector donde cada elemento es un std::array que contiene CANTIDAD_MUESTRAS_SUELO de AtmosphericSample
    std::vector<std::array<AtmosphericSample, CANTIDAD_MUESTRAS_SUELO>> atmosSamplesNodes; // contiene las medicioens ground de cada nodo

    // Un vector donde cada elemento es un std::array que contiene NUMERO_MUESTRAS_ATMOSFERICAS de AtmosphericSample
    std::vector<std::array<AtmosphericSample, NUMERO_MUESTRAS_ATMOSFERICAS>> atmosSamplesNodes; // contiene las medicioens atmosfericas de cada nodo @warning no pasar los 200 0 250 unidades por sobrecargar el almacenamiento

    uint8_t nodesDown[MAX_NODES]; /**< @brief  recolecta los nodos que no contesten o tuvieron fallos  */

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
