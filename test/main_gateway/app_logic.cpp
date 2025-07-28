// 03:00 16/6/2025
//  AppLogic.cpp (Lógica para un nodo sensor)
#include "app_logic.h"  // Incluye la definición de la clase AppLogic.

// TODO:queda implementar logica  errores y posible reinicio si se acomulan
// TODO: poner funcion RTC y terminar tests
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
AppLogic::AppLogic(NodeIdentity identity, RadioManager radioMgr, RtcManager& rtcMgr)
  : nodeIdentity(identity),
    radio(radioMgr),
    rtc(rtcMgr) {
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
void AppLogic::begin() {
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
void AppLogic::update() {

  handleHello();
  handleUartRequest();

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

void AppLogic::handleHello() {
  uint8_t buf[RH_MESH_MAX_MESSAGE_LEN];  // Buffer for the received message
  uint8_t len;                           // Maximum buffer length
  uint8_t from;                          // Sender address
  uint8_t flag;                          // Protocol detection FLAG

  bool triSend = radio.recvMessage(buf, &len, &from, &flag);
  if (triSend) {
    DEBUG_PRINTLN("AppLogic::handleHello(): Entering.");
    // Message successfully received
    DEBUG_PRINT("AppLogic::handleHello(): Message received. Sender: 0x");
    DEBUG_PRINT(String(from, HEX));
    DEBUG_PRINT(", Length: ");
    DEBUG_PRINT(String(len));
    DEBUG_PRINT(", Flag: 0x");
    DEBUG_PRINTLN(String(flag, HEX));

    // Check if the FLAG corresponds to the HELLO message type
            if (static_cast<Protocol::MessageType>(flag) == Protocol::MessageType::HELLO && len == MAC_STR_LEN_WITH_NULL) {
      DEBUG_PRINTLN("AppLogic::handleHello(): Message is of type HELLO.");
              char receivedMac[MAC_STR_LEN_WITH_NULL];
      // Copiar los bytes recibidos al buffer de la MAC
              memcpy(receivedMac, buf, MAC_STR_LEN_WITH_NULL);
      // Check if it's the very first node being registered
      DEBUG_PRINTLN("AppLogic::handleHello(): start the proses to registerNewNode.");
      registerNewNode(*receivedMac, from);
      DEBUG_PRINTLN("AppLogic::handleHello(): Exiting.");
      return;
    }
      else  // The flag is not HELLO
    {
    DEBUG_PRINT("AppLogic::handleHello(): Message received, but FLAG (0x");
    DEBUG_PRINT(String(flag, HEX));
    DEBUG_PRINT(" or  Length: ");
    DEBUG_PRINT(len);
  
    DEBUG_PRINTLN(") is not HELLO. Ignoring message.");
    return;
  }
  
}
}

bool AppLogic::registerNewNode(char receivedMac, uint8_t from) {
  // This 'if' block handles the initial state where no nodes are recorded yet.
  if (mapNodesIDsMac.empty())  // registro de una
  {

    DEBUG_PRINT("AppLogic::handleHello(): First node detected. Registering 0x");
    DEBUG_PRINT(String(from, HEX));
    DEBUG_PRINTLN(" as node 0.");
    mapNodesIDsMac[from] = String(receivedMac);
    DEBUG_PRINTLN("AppLogic::handleHello(): First node registered. Exiting.");
    //return true;  // Exit the function as the first node has been processed succesful.
  }

  // If not the first node, check if it's already registered
  DEBUG_PRINT("AppLogic::handleHello(): Searching if node 0x");
  DEBUG_PRINT(String(from, HEX));
  DEBUG_PRINT(" is already registered among ");
  DEBUG_PRINT(String(mapNodesIDsMac.size()));
  DEBUG_PRINTLN(" existing nodes.");

  // 'it' será un iterador al elemento si se encuentra
  auto it1 = mapNodesIDsMac.find(from);

  // Comprobar si el iterador no es el final del mapa (es decir, la clave se encontró)
  if (it1 != mapNodesIDsMac.end()) {
    DEBUG_PRINT("  ¡Clave encontrada!");
    if (it1->second == String(receivedMac)) {
      DEBUG_PRINT("nodo ya registrado");
      //return true;  // Exit the function as the first node has been processed succesful.
    } else if (it1->second != String(receivedMac)) {
      DEBUG_PRINT("nodo ya registrado con mismo key pero diferente se prosede a envio changeID, MAC: ");
      DEBUG_PRINT(it1->second);  // it1->second es el valor (la String de la MAC)
      DEBUG_PRINTLN("AppLogic::handleHello(): envio send change ID.");
      sendChangeID(from);
      return false;  // Exit the function as the first node hasn't been processed .
    }
  } else {
    DEBUG_PRINT("AppLogic::handleHello(): Nuevo Nodo ya que no esta registrado su key. Registering 0x");
    DEBUG_PRINT(String(from, HEX));
    DEBUG_PRINTLN(" as node 0.");
    mapNodesIDsMac[from] = String(receivedMac);
    //return true;  // Exit the function as the first node has been processed succesful.
  }
  return true;
}
void AppLogic::sendChangeID(uint8_t from) {
  // Declara IDsAcotados con el tamaño máximo de mensaje que puede enviar el radio.
  // Esto asegura que el búfer local es lo suficientemente grande.
  uint8_t IDsAcotados[RH_MESH_MAX_MESSAGE_LEN];
  uint8_t counterNodes = mapNodesIDsMac.size();
  DEBUG_PRINT("AppLogic::sendChangeID(): Enviando solicitud de cambio de ID a 0x");
  DEBUG_PRINTLN(String(from, HEX));
  DEBUG_PRINT("AppLogic::sendChangeID(): counterNodes actual: ");
  DEBUG_PRINTLN(String(counterNodes));
  DEBUG_PRINT("AppLogic::sendChangeID(): RH_MESH_MAX_MESSAGE_LEN: ");
  DEBUG_PRINTLN(String(RH_MESH_MAX_MESSAGE_LEN));

  // Calcula la cantidad de bytes que realmente necesitamos copiar de nodeIDs.
  // Esto debe ser el menor entre:
  // 1. La cantidad real de nodos registrados (counterNodes)
  // 2. El tamaño máximo que puede caber en IDsAcotados (RH_MESH_MAX_MESSAGE_LEN)
  size_t bytesToCopy = (counterNodes < RH_MESH_MAX_MESSAGE_LEN) ? counterNodes : RH_MESH_MAX_MESSAGE_LEN;

  // Asegurarse de que no copiamos de una posición inválida si counterNodes es 0.
  // Aunque si counterNodes es 0, bytesToCopy sería 0 y memcpy no haría nada.
  // Es buena práctica verificar siempre.
  if (bytesToCopy > 0) {
    // Copia los IDs registrados (hasta el límite del búfer de mensaje)
    size_t i = 0;
    for (const auto &pair : mapNodesIDsMac) {
      if ((i + 1) <= bytesToCopy) {
        IDsAcotados[i] = pair.first;
        i++;
      }
    }
    DEBUG_PRINT("AppLogic::sendChangeID(): Se copiaron ");
    DEBUG_PRINT(String(bytesToCopy));
    DEBUG_PRINTLN(" bytes de nodeIDs a IDsAcotados.");
  } else {
    DEBUG_PRINTLN("AppLogic::sendChangeID(): No hay IDs de nodos para copiar (counterNodes es 0).");
    // Si no hay nodos, el mensaje puede ser solo el 'from' o un mensaje vacío si tiene sentido.
    // En este caso, IDsAcotados ya está "vacío" o con basura, así que solo pondremos 'from'.
  }

  // El primer byte del mensaje será la dirección 'from' que se está contactando.
  // Esto sobrescribe el primer byte si se copió algo de nodeIDs.
  // Asegúrate de que esto sea el comportamiento deseado.
  IDsAcotados[0] = from;
  DEBUG_PRINT("AppLogic::sendChangeID(): Primer byte de IDsAcotados establecido a 0x");
  DEBUG_PRINTLN(String(IDsAcotados[0], HEX));

  // Envía el mensaje. La longitud del mensaje es bytesToCopy, ya que eso es lo que copiamos de nodeIDs
  // y lo que queremos enviar (más el ajuste del primer byte).
  // Si tu mensaje siempre incluye 'from' como parte de los datos, la longitud puede variar.
  // Aquí usamos bytesToCopy como la longitud del contenido significativo del array.
  // Si el mensaje es solo el 'from' y los IDs, esto tiene sentido.
  // Si el mensaje es "cambia tu ID a X", el contenido y la longitud serían diferentes.
  // Asumiendo que el mensaje es una lista de IDs de nodos registrados para que 'from' sepa con quién no colisionar.
  radio.sendMessage(from, IDsAcotados, bytesToCopy, Protocol::MessageType::ERROR_DIRECCION);
  DEBUG_PRINTLN("AppLogic::sendChangeID(): Mensaje de cambio de ID enviado.");
}

void AppLogic::timer() {
  // DEBUG_PRINTLN("entro timer ");
  unsigned long tiempoActual = millis();

  if (tiempoActual - temBuf >= INTERVALOANNOUNCE) {
    temBuf = tiempoActual;
    sendAnnounce();
  } else if (tiempoActual - temBuf1 >= INTERVALOATMOSPHERIC && mapNodesIDsMac.empty() == false) {
    temBuf1 = tiempoActual;
    DEBUG_PRINTLN("salto timer requestAtmosphericData");
    requestAtmosphericData();
  }
  if(compareHsAndMs() && mapNodesIDsMac.empty() == false) {
    DEBUG_PRINTLN("salto hora requestGroundGpsData");
    requestGroundGpsData();
  }
}

/**
 * @brief Maneja un mensaje ANNOUNCE.
 *
 */
void AppLogic::sendAnnounce() {
  DEBUG_PRINTLN("enviando announce KEY:");
  uint8_t key = Protocol::KEY;
  DEBUG_PRINTLN(String(key));
  DEBUG_PRINTLN(String(radio.sendMessage(RH_BROADCAST_ADDRESS, &key, sizeof(key), static_cast<uint8_t>(Protocol::MessageType::ANNOUNCE))));
  return;
}

/**
 * @brief solicita los datos actuales de atmospheic a los nodos.

 */
void AppLogic::requestAtmosphericData() {

  DEBUG_PRINTLN("DEBUG: [requestAtmosphericData] Iniciando ciclo de solicitud a nodos.");

      std::array<Protocol::AtmosphericSample, NUMERO_MUESTRAS_ATMOSFERICAS> atmosSamples;
  uint8_t buf[RH_MESH_MAX_MESSAGE_LEN];  // Búfer para el mensaje recibido
  uint8_t key = Protocol::KEY;
  uint8_t len = 0;                                                                       // Longitud máxima del búfer
  uint8_t from;                                                                          // Dirección del remitente
  uint8_t flag = static_cast<uint8_t>(Protocol::MessageType::REQUEST_DATA_ATMOSPHERIC);  // FLAG de detecccion protocolo
  uint8_t nodeId = 0;

  // Calcula el tamaño esperado de los datos atmosféricos para validación
      size_t expectedAtmosphericDataSize = sizeof(Protocol::AtmosphericSample) * NUMERO_MUESTRAS_ATMOSFERICAS;
  DEBUG_PRINT("DEBUG: Tamano de buffer para recv: ");
  DEBUG_PRINT(RH_MESH_MAX_MESSAGE_LEN);
  DEBUG_PRINT(", Tamano esperado de muestras: ");
  DEBUG_PRINTLN(expectedAtmosphericDataSize);


  for (const auto &pair : mapNodesIDsMac) {
    nodeId = pair.first;
    DEBUG_PRINT("DEBUG: [requestAtmosphericData] Procesando nodo ID: 0x");
    DEBUG_PRINTLN(String(nodeId, HEX));

    DEBUG_PRINT("DEBUG: Enviando REQUEST_DATA_ATMOSPHERIC a 0x");
    // Envio de la solicitud
    DEBUG_PRINT(String(nodeId, HEX));
    DEBUG_PRINT(" con payload de ");
    DEBUG_PRINT(len);  // Aquí 'len' es el tamaño completo del buffer de recepción, no el payload del REQUEST
    DEBUG_PRINTLN(" bytes (manager.sendMessage).");

    // NOTA: Si el mensaje REQUEST_DATA_ATMOSPHERIC no lleva payload,
    // es posible que quieras usar `0` para `len` o pasar un puntero nulo como `data`.
    // Tu lógica actual pasa `buf` y `len` completos, lo cual está bien si se espera un payload.

    radio.sendMessage(nodeId, &key, sizeof(key), flag);

    // Delay antes de esperar respuesta
    delay(DELAY_BEFORE_RETRY_ATMOSPHERIC);

    bool t = false;
    t = false;
    uint8_t intentos = 0;
    while (t == false && intentos <= connectionRetries) {
      intentos++;
      DEBUG_PRINT("DEBUG: Intento ");
      DEBUG_PRINT(intentos);
      DEBUG_PRINT(" de recepcion para nodo 0x");
      DEBUG_PRINT(String(nodeId, HEX));
      DEBUG_PRINT(" (Timeout: ");
      DEBUG_PRINT(TIMEOUTGRAL);
      DEBUG_PRINTLN("ms).");

      uint8_t current_recv_len = sizeof(buf);  // Reiniciar len para cada llamada a recvMessageTimeout

      // Intenta recibir un mensaje.
      if (radio.recvMessageTimeout(buf, &current_recv_len, &from, &flag, TIMEOUTGRAL)) {
        DEBUG_PRINT("DEBUG: Mensaje recibido. Remitente: 0x");
        DEBUG_PRINT(String(from, HEX));
        DEBUG_PRINT(", Tipo: 0x");
        DEBUG_PRINT(String(flag, HEX));
        DEBUG_PRINT(", Longitud: ");
        DEBUG_PRINTLN(current_recv_len);

        // Verifica tipo de mensaje y remitente
        if (static_cast<Protocol::MessageType>(flag) == Protocol::MessageType::DATA_ATMOSPHERIC && from == nodeId) {
          DEBUG_PRINTLN("DEBUG: Tipo de mensaje y remitente coinciden.");

          // 1. Verificar tamaño del payload recibido
          if (current_recv_len != expectedAtmosphericDataSize) {
            DEBUG_PRINT("DEBUG: ERROR: Tamano de payload INCORRECTO. Recibido: ");
            DEBUG_PRINT(current_recv_len);
            DEBUG_PRINT(" bytes, Esperado: ");
            DEBUG_PRINT(expectedAtmosphericDataSize);
            DEBUG_PRINTLN(" bytes.");


            // No se modifica 't', el bucle continuará si hay reintentos.
          } else {
            DEBUG_PRINTLN("DEBUG: Tamano de payload CORRECTO. Realizando memcpy.");
            // 2. Convertir a nuestra estructura
            memcpy(atmosSamples.data(), buf, current_recv_len);
            AtmosphericSampleNodes[nodeId] = atmosSamples;
            DEBUG_PRINT("DEBUG: Datos de nodo 0x");
            DEBUG_PRINT(String(nodeId, HEX));
            DEBUG_PRINTLN(" almacenados.");
            t = true;  // Marca como exitoso



            //_--------------------------------borrar
            if (1 == 1) {
              DEBUG_PRINTLN("--- DEBUG: Imprimiendo muestras atmosfericas almacenadas ---");

              if (AtmosphericSampleNodes.empty()) {
                DEBUG_PRINTLN("DEBUG: El mapa AtmosphericSampleNodes esta vacio.");
                return;
              }

              // Itera sobre cada entrada (par clave-valor) en el mapa
              for (const auto &pair : AtmosphericSampleNodes) {
                uint8_t nodeId = pair.first;  // La clave es el ID del nodo
                            // Accedemos al std::array de AtmosphericSample usando .second
            const std::array<Protocol::AtmosphericSample, NUMERO_MUESTRAS_ATMOSFERICAS> &samples = pair.second;

                DEBUG_PRINT("DEBUG: Nodo ID: 0x");
                DEBUG_PRINT(String(nodeId, HEX));  // Convierte el ID a String hexadecimal para imprimir
                DEBUG_PRINTLN(" ----");

                // Itera sobre cada AtmosphericSample dentro del array para este nodo
                for (size_t i = 0; i < samples.size(); ++i) {
                  const Protocol::AtmosphericSample &sample = samples[i];  // Obtén una referencia a la muestra actual

                  DEBUG_PRINT("DEBUG:   Muestra ");
                  DEBUG_PRINT(i + 1);  // Para empezar a contar desde 1
                  DEBUG_PRINT(": ");

                  // Temperatura: Se divide por 10.0 para obtener el valor flotante
                  DEBUG_PRINT("Temp=");
                  DEBUG_PRINT((float)sample.temp / 10.0);  // Imprimir float
                  DEBUG_PRINT("C, ");

                  // Humedad: Se divide por 10.0 para obtener el valor flotante
                  DEBUG_PRINT("Hum=");
                  DEBUG_PRINT((float)sample.moisture / 10.0);  // Imprimir float
                  DEBUG_PRINT("%");

                  // Hora y Minuto: Requiere lógica de formato manual para el cero inicial
                  DEBUG_PRINT(", Hora=");
                  if (sample.hour < 10) {
                    DEBUG_PRINT("0");  // Rellena con cero si es necesario
                  }
                  DEBUG_PRINT(sample.hour);
                  DEBUG_PRINT(":");
                  if (sample.minute < 10) {
                    DEBUG_PRINT("0");  // Rellena con cero si es necesario
                  }
                  DEBUG_PRINT(sample.minute);

                  DEBUG_PRINTLN("");  // Nueva línea para la siguiente muestra
                }
                DEBUG_PRINT("DEBUG: ---- Fin de muestras para Nodo ID: 0x");
                DEBUG_PRINT(String(nodeId, HEX));
                DEBUG_PRINTLN(" ----");
              }
              DEBUG_PRINTLN("--- DEBUG: Fin de impresion de muestras atmosfericas ---");
            }




            break;  // Salir del bucle while
          }
        } else {
          DEBUG_PRINT("DEBUG: Mensaje recibido pero NO ES LA RESPUESTA ESPERADA (tipo o remitente incorrecto).");
        }
      }
      if (t == false) {
        // Delay antes de reintentar
        delay(DELAY_BEFORE_RETRY_ATMOSPHERIC);
        radio.sendMessage(nodeId, &key, sizeof(key), flag);
      }

    }  // Fin del while de reintentos

    if (!t) {
      DEBUG_PRINT("DEBUG: Fallo al obtener datos de nodo 0x");
      DEBUG_PRINT(String(nodeId, HEX));
      DEBUG_PRINTLN(" despues de todos los intentos.");
    }
    DEBUG_PRINTLN("DEBUG: ---");  // Separador entre nodos
    
    // Delay entre procesamiento de nodos
    delay(DELAY_BETWEEN_NODES);
  }                               // Fin del for de nodos
  DEBUG_PRINTLN("DEBUG: [requestAtmosphericData] Finalizado el ciclo de solicitud.");
}


/**
 * @brief solicita los datos actuales de Ground y Gps a los nodos.
 */
void AppLogic::requestGroundGpsData() {

  // DEBUG: Inicio de la función principal de solicitud de datos de suelo y GPS.
  DEBUG_PRINTLN("DEBUG: [requestGroundGpsData] -- INICIO --");

  // std::array<GroundGpsPacket, CANTIDAD_MUESTRAS_SUELO> groundSamples; // Ya está definido como miembro del mapa
  uint8_t buf[RH_MESH_MAX_MESSAGE_LEN] = { 0 };  // Búfer para el mensaje recibido
  uint8_t len = sizeof(buf);                     // Longitud máxima del búfer
  uint8_t from;                                  // Dirección del remitente
  uint8_t flag = static_cast<uint8_t>(Protocol::MessageType::REQUEST_DATA_GPC_GROUND);
  uint8_t nodeId;  // FLAG de detección protocolo
  uint8_t key = Protocol::KEY;

      size_t expectedGroundcDataSize = sizeof(Protocol::GroundGpsPacket);

  // DEBUG: Mostrar tamaño esperado de los datos.
  DEBUG_PRINT("DEBUG: Tamano esperado de datos: ");
  DEBUG_PRINT(String(expectedGroundcDataSize));
  DEBUG_PRINTLN(" bytes.");


  for (const auto &pair : mapNodesIDsMac) {
    nodeId = pair.first;

    // Inicio del contador de tiempo para este nodo
    unsigned long tiempoInicioNodo = millis();

    // DEBUG: Procesando un nuevo nodo.
    DEBUG_PRINT("DEBUG: Procesando nodo ID: 0x");
    DEBUG_PRINT(String(nodeId, HEX));
    DEBUG_PRINTLN("");

    DEBUG_PRINT("DEBUG: Enviando REQUEST_DATA_GPC_GROUND a 0x");
    DEBUG_PRINT(String(nodeId, HEX));
    DEBUG_PRINTLN(".");

    radio.sendMessage(nodeId, &key, sizeof(key), flag);

    // Delay antes de esperar respuesta
    delay(DELAY_BEFORE_RETRY_GROUND);

    bool t = false;
    uint8_t intentos = 0;
    while (t == false && intentos <= connectionRetries) {

      intentos++;
      // DEBUG: Mostrar el número de intento actual.
      DEBUG_PRINT("DEBUG: Intento de recepcion #");
      DEBUG_PRINT(String(intentos));
      DEBUG_PRINT(" para nodo 0x");
      DEBUG_PRINT(String(nodeId, HEX));
      DEBUG_PRINTLN(".");

      // Inicio del contador de tiempo para este intento
      unsigned long tiempoInicio = millis();
      DEBUG_PRINT("DEBUG: Iniciando espera de respuesta... ");

      // Intenta recibir un mensaje.
      if (radio.recvMessageTimeout(buf, &len, &from, &flag, TIMEOUTGRAL)) {

        // Calcular tiempo transcurrido
        unsigned long tiempoTranscurrido = millis() - tiempoInicio;
        DEBUG_PRINT("DEBUG: Respuesta recibida en ");
        DEBUG_PRINT(String(tiempoTranscurrido));
        DEBUG_PRINTLN(" ms.");

        // DEBUG: Mensaje recibido. Mostrar remitente, tipo y longitud.
        DEBUG_PRINT("DEBUG: Mensaje recibido de 0x");
        DEBUG_PRINT(String(from, HEX));
        DEBUG_PRINT(", Tipo: 0x");
        DEBUG_PRINT(String(flag, HEX));
        DEBUG_PRINT(", Longitud: ");
        DEBUG_PRINTLN(String(len));

        if (static_cast<Protocol::MessageType>(flag) == Protocol::MessageType::DATA_GPS_CROUND && from == nodeId) {
          // DEBUG: El mensaje es el esperado.
          DEBUG_PRINTLN("DEBUG: Mensaje es DATA_GPS_CROUND del nodo correcto.");

          // 1. Verificar tamaño
          if (len != expectedGroundcDataSize) {

            // DEBUG: Mensaje de error por tamaño incorrecto.
            DEBUG_PRINT("DEBUG: ERROR: Tamano incorrecto. Recibido: ");
            DEBUG_PRINT(String(len));
            DEBUG_PRINT(", Esperado: ");
            DEBUG_PRINT(String(expectedGroundcDataSize));
            DEBUG_PRINTLN(".");

            t = false;
            continue;  // Continúa al siguiente intento
          } else {
                  // *** LA LÓGICA CLAVE: Insertar un solo GroundGpsPacket en el array del mapa ***

                // Paso 1: Asegurarse de que el nodo ya existe en el mapa o crearlo con un array por defecto.
                // Si el nodo no existe, groundGpsSamplesNodes[nodeId] lo creará con un array inicializado.
                // Si ya existe, obtendrá una referencia a ese array.
                std::array<Protocol::GroundGpsPacket, CANTIDAD_MUESTRAS_SUELO>& nodeSamples = groundGpsSamplesNodes[nodeId];

                // Paso 2: Copia los bytes del búfer 'buf' directamente a una variable temporal de GroundGpsPacket.
                // Esto es necesario para asegurar una copia limpia y manejable.
                Protocol::GroundGpsPacket receivedPacket;
                memcpy(&receivedPacket, buf, expectedGroundcDataSize); // Copia el contenido del búfer al paquete temporal

                // Paso 3: Define la 'count' (posición) donde quieres almacenar este paquete.

                // Verificar que la posición 'count' sea válida
                if (countGroundSamples < CANTIDAD_MUESTRAS_SUELO) {
                    nodeSamples[countGroundSamples] = receivedPacket; // Asigna el paquete recibido a la posición 'count'
                    DEBUG_PRINT("DEBUG: Paquete de nodo 0x");
                    DEBUG_PRINT(String(nodeId, HEX));
                    DEBUG_PRINT(" almacenado en posicion ");
                    DEBUG_PRINT(String(countGroundSamples));
                    DEBUG_PRINTLN(".");
                    countGroundSamples ++;
                    t = true; // Marca como exitoso
                    
                } else {
                    DEBUG_PRINT("DEBUG: ERROR: Posicion 'count' (");
                    DEBUG_PRINT(String(countGroundSamples));
                    DEBUG_PRINT(") excede el tamano del array (");
                    DEBUG_PRINT(String(CANTIDAD_MUESTRAS_SUELO));
                    DEBUG_PRINTLN("). Paquete no almacenado.");
                    break;
                }

           
            //----------------------------borrar -----------------
            // Este bloque de debug está aquí en tu código original. No lo muevo para no alterar la lógica.
            if (1 == 1) {  // Esto es siempre verdadero, por lo que siempre se ejecuta.
              DEBUG_PRINTLN("--- DEBUG: Imprimiendo muestras de suelo y GPS almacenadas (DEBUG INTERNO) ---");

              if (groundGpsSamplesNodes.empty()) {
                DEBUG_PRINTLN("DEBUG: El mapa groundGpsSamplesNodes esta vacio.");
                // return; // Esta línea podría salir de la función principal, lo cual afectaría la lógica.
                // Mantengo tu código, pero si se llama aquí, no debería salir de la función requestGroundGpsData.
              }

              for (const auto &pair_inner : groundGpsSamplesNodes) {  // Usar 'pair_inner' para evitar conflicto de nombre
                uint8_t nodeId_inner = pair_inner.first;
                const std::array<Protocol::GroundGpsPacket, CANTIDAD_MUESTRAS_SUELO> &packets_inner = pair_inner.second;

                DEBUG_PRINT("DEBUG: Nodo ID (interno): 0x");
                DEBUG_PRINT(String(nodeId_inner, HEX));
                DEBUG_PRINTLN(" ----");

                for (size_t i_inner = 0; i_inner < packets_inner.size(); ++i_inner) {
                                  const Protocol::GroundGpsPacket &packet_inner = packets_inner[i_inner];
                const Protocol::GroundSensor &ground_inner = packet_inner.ground;
                const Protocol::GpsSensor &gps_inner = packet_inner.gps;

                  DEBUG_PRINT("DEBUG:   Paquete ");
                  DEBUG_PRINT(String(i_inner + 1));
                  DEBUG_PRINT(" (Suelo + GPS): ");
                  DEBUG_PRINTLN("");

                  DEBUG_PRINT("DEBUG:     [SUELO] Temp=");
                  DEBUG_PRINT(String((float)ground_inner.temp / 10.0, 1));
                  DEBUG_PRINT("C, Hum=");
                  DEBUG_PRINT(String((float)ground_inner.moisture / 10.0, 1));
                  DEBUG_PRINT("%, N=");
                  DEBUG_PRINT(String(ground_inner.n));
                  DEBUG_PRINT(", P=");
                  DEBUG_PRINT(String(ground_inner.p));
                  DEBUG_PRINT(", K=");
                  DEBUG_PRINT(String(ground_inner.k));
                  DEBUG_PRINT(", EC=");
                  DEBUG_PRINT(String(ground_inner.EC));
                  DEBUG_PRINT(", PH=");
                  DEBUG_PRINT(String((float)ground_inner.PH / 10.0, 1));
                  DEBUG_PRINTLN("");

                  DEBUG_PRINT("DEBUG:     [GPS] Lat=");
                  DEBUG_PRINT(String((float)gps_inner.latitude / 10000000.0, 7));
                  DEBUG_PRINT(", Lon=");
                  DEBUG_PRINT(String((float)gps_inner.longitude / 10000000.0, 7));
                  DEBUG_PRINT(", Alt=");
                  DEBUG_PRINT(String(gps_inner.altitude));
                  DEBUG_PRINT("m, Hora=");

                  if (gps_inner.hour < 10) { DEBUG_PRINT("0"); }
                  DEBUG_PRINT(String(gps_inner.hour));
                  DEBUG_PRINT(":");
                  if (gps_inner.minute < 10) { DEBUG_PRINT("0"); }
                  DEBUG_PRINT(String(gps_inner.minute));

                  DEBUG_PRINT(", Flags=0x");
                  DEBUG_PRINT(String(gps_inner.flags, HEX));

                  if (gps_inner.flags & 0x01) { DEBUG_PRINT(" (Ubicacion Valida)"); }
                  if (gps_inner.flags & 0x02) { DEBUG_PRINT(" (Altitud Valida)"); }
                  if (gps_inner.flags & 0x04) { DEBUG_PRINT(" (Hora Valida)"); }

                  DEBUG_PRINTLN("");
                  DEBUG_PRINTLN("DEBUG:   --------------------");
                }
                DEBUG_PRINT("DEBUG: ---- Fin de muestras para Nodo ID (interno): 0x");
                DEBUG_PRINT(String(nodeId_inner, HEX));
                DEBUG_PRINTLN(" ----");
              }
              DEBUG_PRINTLN("--- DEBUG: Fin de impresion de muestras de suelo y GPS (DEBUG INTERNO) ---");
            }  // Fin del if (1 == 1)
            //----------------------------borrar -----------------

            break;  // Salir del bucle while (reintentos)
          }
        } else {
          // DEBUG: Mensaje recibido pero no coincide con el tipo o remitente esperado.
          DEBUG_PRINT("DEBUG: Mensaje inesperado. Tipo recibido: 0x");
          DEBUG_PRINT(String(flag, HEX));
          DEBUG_PRINT(", Remitente recibido: 0x");
          DEBUG_PRINT(String(from, HEX));
          DEBUG_PRINTLN(".");
        }
      } else {
        // Calcular tiempo transcurrido en caso de timeout
        unsigned long tiempoTranscurrido = millis() - tiempoInicio;
        DEBUG_PRINT("DEBUG: Timeout despues de ");
        DEBUG_PRINT(String(tiempoTranscurrido));
        DEBUG_PRINT(" ms. No se recibio respuesta de 0x");
        DEBUG_PRINT(String(nodeId, HEX));
        DEBUG_PRINTLN(".");
      }
      if (t == false) {
        // DEBUG: Se reenvía el mensaje si no hubo éxito.
        DEBUG_PRINT("DEBUG: No exitoso. Reenviando solicitud a 0x");
        DEBUG_PRINT(String(nodeId, HEX));
        DEBUG_PRINTLN(".");
        // Delay antes de reintentar
        delay(DELAY_BEFORE_RETRY_GROUND);
        radio.sendMessage(nodeId, &key, sizeof(key), flag);
      }
    }  // Fin del while de reintentos

    if (!t) {
      // DEBUG: Fallo final al obtener datos.
      DEBUG_PRINT("DEBUG: Fallo definitivo para nodo 0x");
      DEBUG_PRINT(String(nodeId, HEX));
      DEBUG_PRINTLN(".");
    }
    
    // Resumen del tiempo total para este nodo
    unsigned long tiempoTotalNodo = millis() - tiempoInicioNodo;
    DEBUG_PRINT("DEBUG: Tiempo total para nodo 0x");
    DEBUG_PRINT(String(nodeId, HEX));
    DEBUG_PRINT(": ");
    DEBUG_PRINT(String(tiempoTotalNodo));
    DEBUG_PRINTLN(" ms.");
    
    DEBUG_PRINTLN("DEBUG: --- Fin de procesamiento para nodo ---");
    
    // Delay entre procesamiento de nodos
    delay(DELAY_BETWEEN_NODES);
  }  // Fin del for de nodos

  // DEBUG: Fin de la función principal.
  DEBUG_PRINTLN("DEBUG: [requestGroundGpsData] -- FIN --");
}
bool AppLogic::compareHsAndMs() {
    //DEBUG_PRINTLN("compareHsAndMs: Iniciando función");
    
    // Verificar si el RTC está funcionando correctamente
    if (!rtc.isDateTimeValid()) {
        //DEBUG_PRINTLN("compareHsAndMs: RTC no válido, retornando false");
        return false;
    }
    
    if (!rtc.isRunning()) {
        //DEBUG_PRINTLN("compareHsAndMs: RTC no está funcionando, retornando false");
        return false;
    }
    
    // Obtener la hora actual del RTC
    DEBUG_PRINTLN("compareHsAndMs: Obteniendo hora actual");
    String currentTime = rtc.getTimeString();
    DEBUG_PRINT("compareHsAndMs: Hora actual obtenida: ");
    DEBUG_PRINTLN(currentTime);
    
    // Verificar que la hora obtenida sea válida (no "00:00" que indica error)
    if (currentTime == "00:00") {
        DEBUG_PRINTLN("compareHsAndMs: Hora inválida obtenida del RTC, retornando false");
        return false;
    }
    
    // Comparar con cada intervalo de horas configurado
    DEBUG_PRINT("compareHsAndMs: Comparando con ");
    DEBUG_PRINT(CANTIDAD_MUESTRAS_SUELO);
    DEBUG_PRINTLN(" intervalos");
    
    for (int i = 0; i < CANTIDAD_MUESTRAS_SUELO; i++) {
        DEBUG_PRINT("compareHsAndMs: Procesando intervalo ");
        DEBUG_PRINTLN(i);
        
        String targetTime = String(intervaloHorasSuelo[i]) + ":00";
        DEBUG_PRINT("compareHsAndMs: Hora objetivo: ");
        DEBUG_PRINTLN(targetTime);
        
        DEBUG_PRINTLN("compareHsAndMs: Llamando a compareHsAndMs del RTC");
        if (rtc.compareHsAndMs(currentTime, targetTime)) {
            DEBUG_PRINT("compareHsAndMs: Coincidencia encontrada - Hora actual: ");
            DEBUG_PRINT(currentTime);
            DEBUG_PRINT(" con intervalo: ");
            DEBUG_PRINTLN(targetTime);
            return true;
        }
        DEBUG_PRINTLN("compareHsAndMs: No coincidencia para este intervalo");
    }
    
    DEBUG_PRINTLN("compareHsAndMs: No se encontraron coincidencias");
    return false;
}

// TODO: falta implementar
void AppLogic::handleUartRequest() {

  return;
}