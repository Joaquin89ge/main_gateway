// 03:00 16/6/2025
//  AppLogic.cpp (Lógica para un nodo sensor)
#include "app_logic.h" // Incluye la definición de la clase AppLogic.

// TODO:queda implementar logica  errores y posible reinicio si se acomulan
// TODO: poner funciones y completar todo
/**
 * @file app_logic.cpp
 * @brief Implementación de la clase AppLogic para un gateway
 */

/**
 * @brief Constructor de la clase AppLogic para un nodo sensor.
 *
 * Inicializa las referencias a los objetos `NodeIdentity`, `SensorManager` (formalmente GetData) y `RadioManager`
 * que se utilizarán para la gestión de la identidad, la adquisición de datos
 * y la comunicación por radio. También inicializa la dirección del Gateway.
 *
 * @param identity Referencia al objeto NodeIdentity.
 * @param radioMgr Referencia al objeto RadioManager.
 * @param data Referencia al objeto SensorManager (formalmente GetData).
 * @note La declaración en el .h es `AppLogic(NodeIdentity &identity, RadioManager &radioMgr, SensorManager &data, uint8_t gwAddress);`
 * La implementación aquí es `AppLogic(NodeIdentity &identity, RadioManager &radioMgr, SensorManager &data)`.
 * Para que coincida, esta implementación debería aceptar y usar `gwAddress`.
 * Voy a asumir que el constructor del .h es el correcto y que esta implementación
 * debería ser actualizada para tomar `gwAddress`. Por ahora, documento la versión actual del .cpp
 * e inicializo `gatewayAddress` a un valor por defecto o inválido si no se proporciona.
 */
AppLogic::AppLogic(NodeIdentity identity, RadioManager radioMgr)
    : nodeIdentity(identity),
      radio(radioMgr)
{
    gatewayAddress = nodeIdentity.getNodeID();
    // begin();
}

/**
 * @brief Inicia la lógica de la aplicación del nodo sensor.
 *
 * Este método se llama una vez al inicio del programa (normalmente en `setup()`).
 * Su propósito es realizar configuraciones iniciales. Actualmente, envía un mensaje HELLO
 * al `gatewayAddress` para anunciar la presencia de este nodo sensor en la red.
 */
void AppLogic::begin()
{
}

/**
 * @brief Actualiza la lógica de la aplicación en cada ciclo del bucle principal.
 *
 * Este método debe ser llamado repetidamente (en `loop()`).
 * Comprueba si hay mensajes pendientes en la radio utilizando `radio.recvMessage()`.
 * Si se recibe un mensaje, intenta parsearlo con `Protocol::parse()`.
 * Los mensajes parseados correctamente se procesan según su `msg.type`:
 * - `Protocol::ANNOUNCE`: Llama a `handleAnnounce()`.
 * - `Protocol::REQUEST_DATA`: Llama a `handleRequestData()`.
 * - Otros tipos: Se ignora y se imprime un mensaje si no son esperados por un sensor.
 */
void AppLogic::update()
{

    handleHello();
    handleUartRequest();
    if (nodesRegistred)
        timer();
}
/**
 * @brief Envía un mensaje HELLO al Gateway.
 *
 * Crea un mensaje de tipo `Protocol::HELLO`.
 * Establece el `nodeID` del mensaje con la identidad de este nodo.
 * El `jsonPayload` para un mensaje HELLO es actualmente una cadena JSON vacía "{}".
 * Serializa el mensaje y lo envía a través de `radio.sendMessage()` al `gatewayAddress`.
 * Registra el envío en la consola serial.
 */
void AppLogic::handleHello()
{
    uint8_t buf[RH_MESH_MAX_MESSAGE_LEN]; // Búfer para el mensaje recibido
    uint8_t len = sizeof(buf);            // Longitud máxima del búfer
    uint8_t from;                         // Dirección del remitente
    uint8_t flag;                         // FLAG de detecccion protocolo

    // Intenta recibir un mensaje.
    if (radio.recvMessage(buf, &len, &from, &flag))
    {
        if (static_cast<Protocol::MessageType>(flag) == Protocol::MessageType::HELLO)
        {
            if (counterNodes == 0)
            {
                nodeIDs[counterNodes] = from;
                nodesRegistred = true;
                return;
            }

            for (uint8_t i = 0; i < counterNodes; i++)
            {
                if (nodeIDs[i] == from)
                {

                    sendChangeID(from);
                }
            }
            nodeIDs[counterNodes - 1] = from;
            nodesRegistred = true;
        }
    }
}

void AppLogic::sendChangeID(uint8_t from)
{
    uint8_t IDsAcotados[RH_MAX_MESSAGE_LEN]; /*copia lo maximo posile de ids
     guardados para limitar error de cambio de id del nodo y a suves simplifica
     el envio de mensaje maximo manda RH_MAX_LEN del array (solo un mensaje) */
    memcpy(IDsAcotados, nodeIDs, RH_MAX_MESSAGE_LEN);
    IDsAcotados[0] = from;
    radio.sendMessage(from, IDsAcotados, sizeof(IDsAcotados), Protocol::MessageType::ERROR_DIRECCION);
}

void AppLogic::timer()
{
    unsigned long tiempoActual = millis();

    if (tiempoActual - temBuf >= INTERVALOANNOUNCE)
    {
        temBuf = tiempoActual;
        sendAnnounce();
    }
    else if (tiempoActual - temBuf1 >= INTERVALOATMOSPHERIC)
    {
        temBuf1 = tiempoActual;
        requestAtmosphericData();
    }
    if (compareHsAndMs())
    {
        requestGroundGpsData();
    }
}

/**
 * @brief Maneja un mensaje ANNOUNCE.
 *
 */
void AppLogic::sendAnnounce()
{
    uint8_t key = Protocol::KEY;
    radio.sendMessage(255, &key, sizeof(key), static_cast<uint8_t>(Protocol::MessageType::ANNOUNCE));
}

/**
 * @brief solicita los datos actuales de atmospheic a los nodos.

 */
void AppLogic::requestAtmosphericData()
{

    std::array<AtmosphericSample, NUMERO_MUESTRAS_ATMOSFERICAS> atmosSamples;
    uint8_t buf[RH_MESH_MAX_MESSAGE_LEN] = {0}; // Búfer para el mensaje recibido
    uint8_t len = sizeof(buf);                  // Longitud máxima del búfer
    uint8_t from;                               // Dirección del remitente
    uint8_t flag;                               // FLAG de detecccion protocolo
    uint8_t nodeId = 0;
    for (uint8_t i = 0; i < counterNodes; i++)
    {
        nodeId = nodeIDs[i];

        radio.sendMessage(nodeId, buf, len, static_cast<uint8_t>(Protocol::MessageType::REQUEST_DATA_ATMOSPHERIC));

        bool t = false;
        uint8_t intentos = 0;
        while (t == false || intentos <= connectionRetries)
        {

            intentos++;
            // Intenta recibir un mensaje.
            if (radio.recvMessageTimeout(buf, &len, &from, &flag, TIMEOUTGRAL))
            {

                if (static_cast<Protocol::MessageType>(flag) == Protocol::MessageType::REQUEST_DATA_ATMOSPHERIC && from == nodeId)
                {
                    // 1. Verificar tamaño
                    if (len != (sizeof(atmosSamples)))
                    {
                        if (DEBUGPRINTS == false)
                        {
                            Serial.print("Tamaño incorrecto: ");
                            Serial.print(len);
                            Serial.print(" vs ");
                            Serial.println(sizeof(AtmosphericSample) * NUMERO_MUESTRAS_ATMOSFERICAS);
                        }
                        t = false;
                        continue;
                    }
                    // 2. Convertir a nuestra estructura
                    // *** ESTA ES LA FORMA CORRECTA ***
                    memcpy(atmosSamples.data(), buf, len);
                    AtmosphericSampleNodes[nodeId] = atmosSamples;
                    t = true;
                    break;
                }
            }
        }
    }
}

/**
 * @brief solicita los datos actuales de Ground y Gps a los nodos.

 */
void AppLogic::requestGroundGpsData()
{
    if (DEBUGPRINTS == false)
        Serial.println("requestGroundGpsData");
    std::array<GroundGpsPacket, CANTIDAD_MUESTRAS_SUELO> groundSamples;
    uint8_t buf[RH_MESH_MAX_MESSAGE_LEN] = {0}; // Búfer para el mensaje recibido
    uint8_t len = sizeof(buf);                  // Longitud máxima del búfer
    uint8_t from;                               // Dirección del remitente
    uint8_t flag;
    uint8_t nodeId; // FLAG de detecccion protocolo
    for (uint8_t i = 0; i < counterNodes; i++)
    {
        nodeId = nodeIDs[i];
        if (DEBUGPRINTS == false)
        {
            Serial.println("enviando REQUEST_DATA_GPC_GROUND a");
            Serial.println(nodeId);
        }
        radio.sendMessage(nodeId, buf, len, static_cast<uint8_t>(Protocol::MessageType::REQUEST_DATA_GPC_GROUND));

        bool t = false;
        uint8_t intentos = 0;
        while (t == false || intentos <= connectionRetries)
        {

            intentos++;
            // Intenta recibir un mensaje.
            if (radio.recvMessageTimeout(buf, &len, &from, &flag, TIMEOUTGRAL))
            {

                if (static_cast<Protocol::MessageType>(flag) == Protocol::MessageType::REQUEST_DATA_GPC_GROUND && from == nodeId)
                {
                    // 1. Verificar tamaño
                    if (len != (sizeof(groundSamples)))
                    {
                        if (DEBUGPRINTS == true)
                        {
                            Serial.print("Tamaño incorrecto: ");
                            Serial.print(len);
                            Serial.print(" vs ");
                            Serial.println(sizeof(groundSamples));
                        }
                        t = false;
                        continue;
                    }
                    // 2. Convertir a nuestra estructura
                    // *** ESTA ES LA FORMA CORRECTA ***
                    memcpy(groundSamples.data(), buf, len);
                    groundGpsSamplesNodes[nodeId] = groundSamples;
                    if (DEBUGPRINTS == false)
                        Serial.println("recepcion exitosa de REQUEST_DATA_GPC_GROUND");
                    t = true;
                    break;
                }
            }
        }
    }
}

bool AppLogic::compareHsAndMs()
{
    if (clockRtc.getMinutos() == 00)
    {
        for (uint8_t i = 0; i < CANTIDAD_MUESTRAS_SUELO; i++)
        {
            if (clockRtc.getHora() == intervaloHorasSuelo[i])
                return true;
        }
    }
    return false;
}

// TODO: falta implementar
void AppLogic::handleUartRequest()
{

    return;
}