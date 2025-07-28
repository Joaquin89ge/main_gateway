/**
 * @file app_logic.h
 * @brief Lógica de aplicación del Gateway Agrícola
 * @author Joaquin89ge
 * @date 2025-01-20
 * @version 1.0
 * 
 * @details Esta clase implementa la lógica principal del Gateway Agrícola,
 * orquestando la comunicación con nodos sensores, gestión de datos atmosféricos
 * y de suelo/GPS, y coordinación temporal mediante RTC.
 * 
 * @see NodeIdentity, RadioManager, RtcManager, Protocol
 * @see https://docs.platformio.org/en/latest/frameworks/arduino.html
 * @see https://www.arduino.cc/reference/en/
 */

#ifndef APP_LOGIC_H
#define APP_LOGIC_H

#include <map>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "node_identity.h" // Para NodeIdentity (dirección MAC, clave)
#include "radio_manager.h" // Para RadioManager (gestión de radio LoRa)
#include "protocol.h"      // Para Protocol (serialización/deserialización de mensajes)
#include "rtc_manager.h"
#include "config.h"

/**
 * @class AppLogic
 * @brief Clase principal que orquesta la lógica del Gateway Agrícola
 * 
 * @details Esta clase implementa el comportamiento del Gateway en la red mesh LoRa.
 * Sus responsabilidades principales incluyen:
 * - Gestión de nodos sensores registrados
 * - Solicitud y almacenamiento de datos atmosféricos
 * - Solicitud y almacenamiento de datos de suelo/GPS
 * - Coordinación temporal mediante RTC
 * - Manejo de errores y nodos inactivos
 * 
 * @note Esta clase es específica para ESP8266 y requiere inicialización
 *       de SPI para comunicación LoRa
 * @warning No exceder MAX_NODES (250) para evitar sobrecarga de memoria
 * 
 * @example
 * ```cpp
 * #include "app_logic.h"
 * 
 * NodeIdentity identity;
 * RadioManager radio(identity.getNodeID());
 * RtcManager rtc(RTC_DAT, RTC_CLK, RTC_RST);
 * 
 * AppLogic logic(identity, radio, rtc);
 * logic.begin();
 * 
 * void loop() {
 *     logic.update();
 *     delay(100);
 * }
 * ```
 * 
 * @see https://github.com/Joaquin89ge/main_gateway
 * @see https://www.arduino.cc/reference/en/language/functions/time/delay/
 */
class AppLogic
{
private:
    NodeIdentity nodeIdentity; /**< @brief Gestor de identidad del nodo basado en MAC */
    RadioManager radio;        /**< @brief Gestor de comunicación LoRa */
    RtcManager& rtc;          /**< @brief Referencia al gestor de tiempo real */
    uint8_t gatewayAddress;   /**< @brief Dirección de red del Gateway */

    // Variables WiFi y MQTT
    WiFiClient wifiClient;    /**< @brief Cliente WiFi */
    PubSubClient mqttClient;  /**< @brief Cliente MQTT */
    bool wifiConnected;       /**< @brief Estado de conexión WiFi */
    bool mqttConnected;       /**< @brief Estado de conexión MQTT */

    std::map<uint8_t, String> mapNodesIDsMac; /**< @brief Mapeo de IDs de nodos a direcciones MAC */

    /**
     * @brief Intervalos de solicitud de datos de suelo/GPS (en horas)
     * @details Los datos de suelo se solicitan a las 12:00 y 24:00 horas
     * @see requestGroundGpsData()
     */
    const int intervaloHorasSuelo[CANTIDAD_MUESTRAS_SUELO] = {12, 24};
    
    unsigned long temBuf = 0;  /**< @brief Buffer temporal para gestión de tiempo */
    unsigned long temBuf1 = 0; /**< @brief Buffer temporal secundario */

    /**
     * @brief Número máximo de reintentos para solicitudes de datos
     * @details Después de este número de reintentos, el nodo se marca como inactivo
     * @see nodesDown
     */
    const uint8_t connectionRetries = 2;

    /**
     * @brief Flag que indica si los datos han sido actualizados
     * @details Se activa después de requestAtmosphericData() o requestGroundGpsData()
     */
    bool updatedData = false;

    /**
     * @brief Contador de muestras de suelo almacenadas
     * @details Controla la posición en el array de muestras diarias
     * @see groundGpsSamplesNodes
     */
    uint8_t countGroundSamples = 0;
    
    /**
     * @brief Envía mensaje ANNOUNCE a la red
     * @details Broadcast que anuncia la presencia del Gateway
     * @see Protocol::ANNOUNCE
     */
    void sendAnnounce();

    /**
     * @brief Procesa mensajes HELLO de nodos sensores
     * @details Registra nuevos nodos en la red
     * @see Protocol::HELLO
     */
    void handleHello();
    
    /**
     * @brief Registra un nuevo nodo en la red
     * @param receivedMac Dirección MAC del nodo
     * @param from ID del nodo
     * @return true si el registro fue exitoso, false en caso contrario
     */
    bool registerNewNode(char receivedMac, uint8_t from);
    
    /**
     * @brief Solicita datos atmosféricos a todos los nodos registrados
     * @details Itera sobre todos los nodos conocidos y envía REQUEST_DATA_ATMOSPHERIC
     * @note Los datos se solicitan cada INTERVALOATMOSPHERIC milisegundos
     * @warning No exceder MAX_NODES para evitar sobrecarga de memoria
     * @return true si al menos un nodo respondió, false en caso contrario
     * 
     * @example
     * ```cpp
     * if (logic.requestAtmosphericData()) {
     *     Serial.println("Datos atmosféricos recibidos");
     * }
     * ```
     * 
     * @see AtmosphericSampleNodes, Protocol::REQUEST_DATA_ATMOSPHERIC
     */
    void requestAtmosphericData();
    
    /**
     * @brief Solicita datos de suelo/GPS a todos los nodos registrados
     * @details Se ejecuta en horarios específicos (12:00 y 24:00)
     * @note Verifica compareHsAndMs() antes de ejecutar
     * @see groundGpsSamplesNodes, Protocol::REQUEST_DATA_GPS_GROUND
     */
    void requestGroundGpsData();
    
    /**
     * @brief Procesa solicitudes UART externas
     * @details Permite comunicación serial para debugging y control
     */
    void handleUartRequest();
    
    /**
     * @brief Envía comando de cambio de ID a un nodo
     * @param from ID del nodo destino
     */
    void sendChangeID(uint8_t from);
    
    /**
     * @brief Gestión temporal del sistema
     * @details Controla intervalos de solicitudes y anuncios
     */
    void timer();
    
    /**
     * @brief Compara hora actual con horarios configurados
     * @details Verifica si es momento de solicitar datos de suelo/GPS
     * @return true si la hora actual coincide con algún intervalo configurado
     * @see intervaloHorasSuelo
     */
    bool compareHsAndMs();

    /**
     * @brief Conecta a WiFi
     * @details Intenta conectar a la red WiFi configurada
     * @return true si la conexión fue exitosa
     */
    bool connectWiFi();

    /**
     * @brief Conecta a MQTT
     * @details Intenta conectar al broker MQTT configurado
     * @return true si la conexión fue exitosa
     */
    bool connectMQTT();

    /**
     * @brief Publica datos atmosféricos por MQTT
     * @param nodeId ID del nodo
     * @param data Datos atmosféricos a publicar
     */
    void publishAtmosphericData(uint8_t nodeId, const Protocol::AtmosphericSample& data);

    /**
     * @brief Publica datos de suelo por MQTT
     * @param nodeId ID del nodo
     * @param data Datos de suelo a publicar
     */
    void publishGroundData(uint8_t nodeId, const Protocol::GroundGpsPacket& data);

public:
    /**
     * @brief Almacenamiento de datos de suelo/GPS por nodo
     * @details Mapa donde cada elemento es un array de GroundGpsPacket
     * @note CANTIDAD_MUESTRAS_SUELO muestras por día por nodo
     * @warning No exceder MAX_NODES para evitar sobrecarga de memoria
     * 
     * @example
     * ```cpp
     * // Acceder a datos de suelo del nodo 0x42
     * auto& nodeData = logic.groundGpsSamplesNodes[0x42];
     * GroundGpsPacket sample = nodeData[0]; // Primera muestra del día
     * ```
     */
    std::map<std::uint8_t, std::array<Protocol::GroundGpsPacket, CANTIDAD_MUESTRAS_SUELO>> groundGpsSamplesNodes;

    /**
     * @brief Almacenamiento de datos atmosféricos por nodo
     * @details Mapa donde cada elemento es un array de AtmosphericSample
     * @note NUMERO_MUESTRAS_ATMOSFERICAS muestras por nodo
     * @warning No exceder 200-250 nodos para evitar sobrecarga de memoria
     * 
     * @example
     * ```cpp
     * // Acceder a datos atmosféricos del nodo 0x42
     * auto& nodeData = logic.AtmosphericSampleNodes[0x42];
     * AtmosphericSample sample = nodeData[0]; // Primera muestra
     * ```
     */
    std::map<std::uint8_t, std::array<Protocol::AtmosphericSample, NUMERO_MUESTRAS_ATMOSFERICAS>> AtmosphericSampleNodes;

    /**
     * @brief Lista de nodos inactivos o con fallos
     * @details Array que almacena IDs de nodos que no responden
     * @note Se llena cuando se alcanza connectionRetries sin respuesta
     * @see connectionRetries
     */
    uint8_t nodesDown[MAX_NODES];

    /**
     * @brief Constructor de AppLogic
     * @param identity Gestor de identidad del nodo
     * @param radioMgr Gestor de comunicación LoRa
     * @param rtcMgr Gestor de tiempo real
     * 
     * @example
     * ```cpp
     * NodeIdentity identity;
     * RadioManager radio(identity.getNodeID());
     * RtcManager rtc(RTC_DAT, RTC_CLK, RTC_RST);
     * AppLogic logic(identity, radio, rtc);
     * ```
     */
    AppLogic(NodeIdentity identity, RadioManager radioMgr, RtcManager& rtcMgr);

    /**
     * @brief Inicializa la lógica de aplicación
     * @details Configuración inicial y envío de primer ANNOUNCE
     * @note Debe llamarse una vez en setup()
     * 
     * @example
     * ```cpp
     * void setup() {
     *     logic.begin();
     * }
     * ```
     */
    void begin();

    /**
     * @brief Actualiza la lógica de aplicación
     * @details Procesa mensajes entrantes y gestiona temporizadores
     * @note Debe llamarse repetidamente en loop()
     * 
     * @example
     * ```cpp
     * void loop() {
     *     logic.update();
     *     delay(100);
     * }
     * ```
     * 
     * @see https://www.arduino.cc/reference/en/language/functions/time/delay/
     */
    void update();
};

#endif // APP_LOGIC_H
