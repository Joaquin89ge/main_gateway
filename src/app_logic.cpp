// 03:00 16/6/2025
//  AppLogic.cpp (Lógica para un nodo sensor)
#include "app_logic.h" // Incluye la definición de la clase AppLogic.

// TODO:queda implementar logica  errores y posible reinicio si se acomulan
/**
 * @file app_logic.cpp
 * @brief Implementación de la clase AppLogic para un nodo sensor.
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
AppLogic::AppLogic(SensorManager data,NodeIdentity identity, RadioManager radioMgr )
: getData(data),
      nodeIdentity(identity),
      radio(radioMgr){
    nodeID = nodeIdentity.getNodeID();
    gatwayRegistred = nodeIdentity.getGetway(gatewayAddress);
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

    uint8_t buf[RH_MESH_MAX_MESSAGE_LEN]; // Búfer para el mensaje recibido
    uint8_t len = sizeof(buf);            // Longitud máxima del búfer
    uint8_t from;                         // Dirección del remitente
    uint8_t flag;                         // FLAG de detecccion protocolo

    getData.update();
    // Intenta recibir un mensaje.
    if (radio.recvMessage(buf, &len, &from, &flag))
    {
        Serial.print(F("[AppLogic] Mensaje recibido de 0x"));
        Serial.print(from, HEX);
        Serial.print(F(" con longitud "));
        Serial.println(len);

        if (static_cast<Protocol::MessageType>(flag) == Protocol::MessageType::ANNOUNCE)
        {
            //   handleAnnounce  lista creo
            handleAnnounce(buf, len, from);
        }
        else if (gatwayRegistred == true && from == gatewayAddress)
        {

            switch (static_cast<Protocol::MessageType>(flag))
            {

            case Protocol::MessageType::REQUEST_DATA_ATMOSPHERIC:
                // listo creo
                sendAtmosphericData();
                break;
            case Protocol::MessageType::REQUEST_DATA_GPC_GROUND:
                // listo creo
                sendGroungGpsData();
                break;
            case Protocol::MessageType::ERROR_DIRECCION:

                changeID(buf, len);

                break;
            default:
                Serial.print("Tipo de mensaje desconocido o no relevante para este nodo: 0x");
                Serial.println(flag, HEX);
                break;
            }
        }
    }

    unsigned long tiempoActual = millis();

    if (tiempoActual - temBuf >= intervaloHello)
    {
        temBuf = tiempoActual;
        sendHello();
    }
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
void AppLogic::sendHello()
{
    uint8_t buffer[0];
    if (radio.sendMessage(gatewayAddress, buffer, sizeof(buffer), static_cast<uint8_t>(Protocol::MessageType::HELLO)))
    {
        Serial.println(F("[AppLogic] HELLO enviado exitosamente."));
    }
    else
    {
        Serial.println(F("[AppLogic] Fallo al enviar HELLO."));
    }
}

/**
 * @brief Maneja un mensaje ANNOUNCE.
 *
 */

void AppLogic::handleAnnounce(uint8_t *buf, uint8_t len, uint8_t from)
{
    if (gatwayRegistred == true && gatewayAddress == from)
        return;

    if (buf[0] == Protocol::KEY)
    {
        gatewayAddress = from;
        nodeIdentity.saveGetway(gatewayAddress);
        gatwayRegistred = true;
    }
    return;
}

/**
 * @brief Envía los datos actuales del sensor al Gateway.

 */
void AppLogic::sendAtmosphericData()
{
    getData.readSensorsAtmospheric();

    if (radio.sendMessage(gatewayAddress, reinterpret_cast<uint8_t *>(&getData.atmosSamples), sizeof(getData.atmosSamples), Protocol::MessageType::DATA_ATMOSPHERIC))
    {
        Serial.println(F("[AppLogic] DATA atmosferica enviado exitosamente."));
    }
    else
    {
        Serial.println(F("[AppLogic] Falló el envío de DATA atmosferica al Gateway."));
    }
    /*idea sensilla para recepcion
    void processReceivedPacket(uint8_t* data, uint8_t len) {
    // 1. Verificar tamaño
    if(len != sizeof(atmosSamples)) {
        Serial.print("Tamaño incorrecto: ");
        Serial.print(len);
        Serial.print(" vs ");
        Serial.println(sizeof(atmosSamples));
        return;
    }

    // 2. Convertir a nuestra estructura
    atmosSamples* packet = reinterpret_cast<atmosSamples*>(data);
    */
}

/**
 * @brief Envía los datos actuales del sensor al Gateway.

 */
void AppLogic::sendGroungGpsData()
{
    getData.readGroundGpsSensors();
    GroundGpsPacket packet;
    packet.ground = getData.groundData;
    packet.gps = getData.gpsData;
    if (radio.sendMessage(gatewayAddress, reinterpret_cast<uint8_t *>(&packet), sizeof(packet), Protocol::MessageType::DATA_GPS_CROUND))
    {
        Serial.println(F("[AppLogic] DATA gps y ground enviado exitosamente."));
    }
    else
    {
        Serial.println(F("[AppLogic] Falló el envío de DATA  gps y ground al Gateway."));
    }
    /*idea sensilla para recepcion
    void processReceivedPacket(uint8_t* data, uint16_t len) {
    if(len != sizeof(GroundGpsPacket)) {
        Serial.println("Error: Tamaño incorrecto");
        return;
    }

    GroundGpsPacket* packet = reinterpret_cast<GroundGpsPacket*>(data);

    // Verificar checksum
    if(packet->checksum != calculateChecksum(*packet)) {
        Serial.println("Error en checksum");
        return;
    }

    // Procesar datos de suelo
    float soilTemp = packet->ground.temp / 10.0f;
    float soilHumidity = packet->ground.moisture / 10.0f;
    float ph = packet->ground.PH / 10.0f;

    // Procesar GPS
    double latitude = static_cast<double>(packet->gps.latitude) / 10000000.0;
    double longitude = static_cast<double>(packet->gps.longitude) / 10000000.0;

    // Verificar flags
    bool validLocation = (packet->gps.flags & 0x01);
    bool validAltitude = (packet->gps.flags & 0x02);
    bool validTime = (packet->gps.flags & 0x04);
}
    */
}

void AppLogic::changeID(uint8_t *buf, uint8_t len)
{
    Serial.println("Error de dirección detectado.");
    if (len != 0)
    {
        nodeID = nodeIdentity.changeNodeID(len, buf);
    }
    else
    {
        Serial.print("no llego data de balores repetidos asumo sera mi direccion la repetida");
        uint8_t blacklist[1] = {nodeID};
        nodeID = nodeIdentity.changeNodeID(1, blacklist);
    }
}