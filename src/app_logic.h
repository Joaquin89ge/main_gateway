// 03:00 16/6/2025
//  AppLogic.h
#ifndef APP_LOGIC_H
#define APP_LOGIC_H

#include <map>
#include "node_identity.h" // Para NodeIdentity (dirección MAC, clave)
#include "radio_manager.h" // Para RadioManager (gestión de radio LoRa)
#include "protocol.h"      // Para Protocol (serialización/deserialización de mensajes)
#include "rtc_manager.h"
#include "config.h"

/**
 * @class AppLogic
 * @brief Clase que encapsula la lógica de aplicación para un nodo sensor.
 *
 * Esta clase orquesta la interacción entre los diferentes módulos (identidad del nodo,
 * gestión de radio, protocolo de comunicación y adquisición de datos) para implementar
 * el comportamiento de un nodo sensor. Su rol principal es gateway por ende envia announchers
 * solicita data y escucha hello.
 */

class AppLogic
{
private:
    NodeIdentity nodeIdentity; /**< @brief Referencia al objeto NodeIdentity para gestionar la identidad del nodo. */
    RadioManager radio;        /**< @brief Referencia al objeto RadioManager para la comunicación por radio. */
    RtcManager& rtc;          /**< @brief Referencia al objeto RtcManager para gestión del tiempo. */
    uint8_t gatewayAddress; /**< @brief Dirección de red del Gateway con el que este sensor se comunica. */

    std::map<uint8_t, String> mapNodesIDsMac;

    const int intervaloHorasSuelo[CANTIDAD_MUESTRAS_SUELO] = {12, 24}; /**< @brief horas para request data ground y gps*/
    unsigned long temBuf = 0;                                          /**< @brief bufer guarda tiempo */
    unsigned long temBuf1 = 0;                                         /**< @brief  bufer guarda tiempo1 */

    const uint8_t connectionRetries = 2; /**< @brief reintentos para pedir nuevamente datos, alcanzaaod el maximo se agrega la direccion del nodo a nodesDown*/

    bool updatedData = false; /**< @brief  flag de datos actualizados, se pone en true luego de un requestAtmosphericData o requestGroundGpsData */

   uint8_t countGroundSamples = 0 ;/**< @brief  cuenta en que posicion del array meter la medicion del modulo lora ya que el poquete de datos es por dia CANTIDAD_MUESTRAS_SUELO x DIA */
    
    void sendAnnounce();

    /**
     * @brief Envía un mensaje HELLO al Gateway.
     *
     * Este método es llamado por el nodo sensor al iniciar para anunciarse
     * a la red y al Gateway.
     */
    void handleHello();
    bool registerNewNode(char receivedMac, uint8_t from);
    void requestAtmosphericData();
    void requestGroundGpsData();
    void handleUartRequest();
    void sendChangeID(uint8_t from);
    void timer();
    bool compareHsAndMs();

public:
    // Un mapa donde cada elemento es un std::array que contiene CANTIDAD_MUESTRAS_SUELO de AtmosphericSample
    std::map<std::uint8_t, std::array<GroundGpsPacket, CANTIDAD_MUESTRAS_SUELO>> groundGpsSamplesNodes; // contiene las medicioens ground de cada nodo

    // Un mapa donde cada elemento es un std::array que contiene NUMERO_MUESTRAS_ATMOSFERICAS de AtmosphericSample
    std::map<std::uint8_t, std::array<AtmosphericSample, NUMERO_MUESTRAS_ATMOSFERICAS>> AtmosphericSampleNodes; // contiene las medicioens atmosfericas de cada nodo @warning no pasar los 200 0 250 unidades por sobrecargar el almacenamiento

    /* *@warning se espera que se recolecten los datos por medio de uart en maximo INTERVALOATMOSPHERIC*2 ya que antes de volver a pedir los datos tiene que esta vacios lo datos para evitar sobrecarga de memoria  */

    uint8_t nodesDown[MAX_NODES]; /**< @brief  recolecta los nodos que no contesten o tuvieron fallos  */

    /**
     * @brief Constructor de la clase AppLogic para un nodo sensor.
     *
     * @param identity Referencia al objeto NodeIdentity para la gestión de la identidad del nodo.
     * @param radioMgr Referencia al objeto RadioManager para la comunicación por radio.
     * @param rtcMgr Referencia al objeto RtcManager para la gestión del tiempo.
     */
    AppLogic(NodeIdentity identity, RadioManager radioMgr, RtcManager& rtcMgr);

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
