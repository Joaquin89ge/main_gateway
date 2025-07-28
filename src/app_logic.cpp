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
    rtc(rtcMgr),
    mqttClient(wifiClient) {
  gatewayAddress = nodeIdentity.getNodeID();
  wifiConnected = false;
  mqttConnected = false;
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

  // Mantener conexión MQTT activa
  if (wifiConnected && mqttConnected) {
    mqttClient.loop();
  }

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
    Serial.printf("AppLogic::handleHello(): Message received. Sender: 0x%02X, Length: %d, Flag: 0x%02X\n", from, len, flag);

    // Check if the FLAG corresponds to the HELLO message type
            if (static_cast<Protocol::MessageType>(flag) == Protocol::MessageType::HELLO && len == MAC_STR_LEN_WITH_NULL) {
      Serial.printf("AppLogic::handleHello(): Message is of type HELLO.\n");
              char receivedMac[MAC_STR_LEN_WITH_NULL];
      // Copiar los bytes recibidos al buffer de la MAC
              memcpy(receivedMac, buf, MAC_STR_LEN_WITH_NULL);
      // Check if it's the very first node being registered
      Serial.printf("AppLogic::handleHello(): start the proses to registerNewNode.\n");
      registerNewNode(*receivedMac, from);
      Serial.printf("AppLogic::handleHello(): Exiting.\n");
      return;
    }
      else  // The flag is not HELLO
    {
    Serial.printf("AppLogic::handleHello(): Message received, but FLAG (0x%02X or Length: %d", flag, len);
  
    Serial.printf(") is not HELLO. Ignoring message.\n");
    return;
  }
  
}
}

bool AppLogic::registerNewNode(char receivedMac, uint8_t from) {
  // This 'if' block handles the initial state where no nodes are recorded yet.
  if (mapNodesIDsMac.empty())  // registro de una
  {

    Serial.printf("AppLogic::handleHello(): First node detected. Registering 0x%02X as node 0.\n", from);
    mapNodesIDsMac[from] = String(receivedMac);
    Serial.printf("AppLogic::handleHello(): First node registered. Exiting.\n");
    return true;  // Exit the function as the first node has been processed succesful.
  }

  // If not the first node, check if it's already registered
  Serial.printf("AppLogic::handleHello(): Searching if node 0x%02X is already registered among %d existing nodes.\n", from, mapNodesIDsMac.size());

  // 'it' será un iterador al elemento si se encuentra
  auto it1 = mapNodesIDsMac.find(from);

  // Comprobar si el iterador no es el final del mapa (es decir, la clave se encontró)
  if (it1 != mapNodesIDsMac.end()) {
    Serial.printf("  ¡Clave encontrada!\n");
            if (it1->second == String(receivedMac, MAC_STR_LEN_WITH_NULL)) {
      Serial.printf("nodo ya registrado\n");
      return true;  // Exit the function as the first node has been processed succesful.
            } else if (it1->second != String(receivedMac, MAC_STR_LEN_WITH_NULL)) {
      Serial.printf("nodo ya registrado con mismo key pero diferente se prosede a envio changeID, MAC: ");
      Serial.printf(it1->second.c_str());  // it1->second es el valor (la String de la MAC)
      Serial.printf("AppLogic::handleHello(): envio send change ID.\n");
      //sendChangeID(from);
      return false;  // Exit the function as the first node hasn't been processed .
    }
  } else {
    Serial.printf("AppLogic::handleHello(): Nuevo Nodo ya que no esta registrado su key. Registering 0x%02X as node 0.\n", from);
    mapNodesIDsMac[from] = String(receivedMac);
    return true;  // Exit the function as the first node has been processed succesful.
  }
  return true;
}
void AppLogic::sendChangeID(uint8_t from) {
  // Declara IDsAcotados con el tamaño máximo de mensaje que puede enviar el radio.
  // Esto asegura que el búfer local es lo suficientemente grande.
  uint8_t IDsAcotados[RH_MESH_MAX_MESSAGE_LEN];
  uint8_t counterNodes = mapNodesIDsMac.size();
  Serial.printf("AppLogic::sendChangeID(): Enviando solicitud de cambio de ID a 0x%02X\n", from);
  Serial.printf("AppLogic::sendChangeID(): counterNodes actual: %d\n", counterNodes);
  Serial.printf("AppLogic::sendChangeID(): RH_MESH_MAX_MESSAGE_LEN: %d\n", RH_MESH_MAX_MESSAGE_LEN);

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
    Serial.printf("AppLogic::sendChangeID(): Se copiaron ");
    Serial.printf("Se copiaron %d bytes de nodeIDs a IDsAcotados.\n", bytesToCopy);
  } else {
    Serial.printf("AppLogic::sendChangeID(): No hay IDs de nodos para copiar (counterNodes es 0).\n");
    // Si no hay nodos, el mensaje puede ser solo el 'from' o un mensaje vacío si tiene sentido.
    // En este caso, IDsAcotados ya está "vacío" o con basura, así que solo pondremos 'from'.
  }

  // El primer byte del mensaje será la dirección 'from' que se está contactando.
  // Esto sobrescribe el primer byte si se copió algo de nodeIDs.
  // Asegúrate de que esto sea el comportamiento deseado.
  IDsAcotados[0] = from;
      Serial.printf("AppLogic::sendChangeID(): Primer byte de IDsAcotados establecido a 0x%02X\n", IDsAcotados[0]);

  // Envía el mensaje. La longitud del mensaje es bytesToCopy, ya que eso es lo que copiamos de nodeIDs
  // y lo que queremos enviar (más el ajuste del primer byte).
  // Si tu mensaje siempre incluye 'from' como parte de los datos, la longitud puede variar.
  // Aquí usamos bytesToCopy como la longitud del contenido significativo del array.
  // Si el mensaje es solo el 'from' y los IDs, esto tiene sentido.
  // Si el mensaje es "cambia tu ID a X", el contenido y la longitud serían diferentes.
  // Asumiendo que el mensaje es una lista de IDs de nodos registrados para que 'from' sepa con quién no colisionar.
  radio.sendMessage(from, IDsAcotados, bytesToCopy, Protocol::MessageType::ERROR_DIRECCION);
  Serial.printf("AppLogic::sendChangeID(): Mensaje de cambio de ID enviado.\n");
}

void AppLogic::timer() {
  // DEBUG_PRINTLN("entro timer ");
  unsigned long tiempoActual = millis();

  if (tiempoActual - temBuf >= INTERVALOANNOUNCE) {
    temBuf = tiempoActual;
    sendAnnounce();
  } else if (tiempoActual - temBuf1 >= INTERVALOATMOSPHERIC && mapNodesIDsMac.empty() == false) {
    temBuf1 = tiempoActual;
    Serial.printf("salto timer requestAtmosphericData\n");
    requestAtmosphericData();
  }
  
  // Lógica condicional para requestGroundGpsData basada en USE_TIMER_FOR_GROUND_REQUEST
  if (USE_TIMER_FOR_GROUND_REQUEST == 1) {
    // Modo temporizador: usar millis() como atmospheric
    static unsigned long temBufGround = 0;
    if (tiempoActual - temBufGround >= INTERVALO_GROUND_REQUEST && mapNodesIDsMac.empty() == false) {
      temBufGround = tiempoActual;
      Serial.printf("salto timer requestGroundGpsData (modo temporizador)\n");
      requestGroundGpsData();
    }
  } else {
    // Modo comparación de horas: usar compareHsAndMs() como antes
    if(compareHsAndMs() && mapNodesIDsMac.empty() == false) {
      Serial.printf("salto hora requestGroundGpsData (modo comparación de horas)\n");
      requestGroundGpsData();
    }
  }
}

/**
 * @brief Maneja un mensaje ANNOUNCE.
 *
 */
void AppLogic::sendAnnounce() {
  Serial.printf("enviando announce KEY:\n");
  uint8_t key = Protocol::KEY;
      Serial.printf("enviando announce KEY: %d\n", key);
    Serial.printf("Resultado envío: %d\n", radio.sendMessage(RH_BROADCAST_ADDRESS, &key, sizeof(key), static_cast<uint8_t>(Protocol::MessageType::ANNOUNCE)));
  return;
}

/**
 * @brief solicita los datos actuales de atmospheic a los nodos.

 */
void AppLogic::requestAtmosphericData() {

  Serial.printf("DEBUG: [requestAtmosphericData] Iniciando ciclo de solicitud a nodos.\n");

  // Verificar memoria disponible antes de crear arrays grandes
  if (ESP.getFreeHeap() < 2048) {
    Serial.printf("DEBUG: ADVERTENCIA: Memoria baja antes de procesar datos atmosféricos\n");
    return; // Salir si la memoria es muy baja
  }

  std::array<Protocol::AtmosphericSample, NUMERO_MUESTRAS_ATMOSFERICAS> atmosSamples;
  uint8_t buf[RH_MESH_MAX_MESSAGE_LEN];  // Búfer para el mensaje recibido
  uint8_t key = Protocol::KEY;
  uint8_t len = 0;                                                                       // Longitud máxima del búfer
  uint8_t from;                                                                          // Dirección del remitente
  uint8_t flag = static_cast<uint8_t>(Protocol::MessageType::REQUEST_DATA_ATMOSPHERIC);  // FLAG de detecccion protocolo
  uint8_t nodeId = 0;

  // Calcula el tamaño esperado de los datos atmosféricos para validación
      size_t expectedAtmosphericDataSize = sizeof(Protocol::AtmosphericSample) * NUMERO_MUESTRAS_ATMOSFERICAS;
  Serial.printf("DEBUG: Tamano de buffer para recv: ");
  Serial.printf("%d", RH_MESH_MAX_MESSAGE_LEN);
  Serial.printf(", Tamano esperado de muestras: ");
  Serial.printf("%d\n", expectedAtmosphericDataSize);


  for (const auto &pair : mapNodesIDsMac) {
    nodeId = pair.first;
    Serial.printf("DEBUG: [requestAtmosphericData] Procesando nodo ID: 0x");
    Serial.printf("%02X\n", nodeId);

    Serial.printf("DEBUG: Enviando REQUEST_DATA_ATMOSPHERIC a 0x");
    // Envio de la solicitud
    Serial.printf("%02X", nodeId);
    Serial.printf(" con payload de ");
    Serial.printf("%d", len);  // Aquí 'len' es el tamaño completo del buffer de recepción, no el payload del REQUEST
    Serial.printf(" bytes (manager.sendMessage).\n");

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
      Serial.printf("DEBUG: Intento %d de recepcion para nodo 0x%02X (Timeout: %d ms).\n", intentos, nodeId, TIMEOUTGRAL);

      uint8_t current_recv_len = sizeof(buf);  // Reiniciar len para cada llamada a recvMessageTimeout

      // Intenta recibir un mensaje.
      if (radio.recvMessageTimeout(buf, &current_recv_len, &from, &flag, TIMEOUTGRAL)) {
        Serial.printf("DEBUG: Mensaje recibido. Remitente: 0x%02X, Tipo: 0x%02X, Longitud: %d\n", from, flag, current_recv_len);

        // Verifica tipo de mensaje y remitente
        if (static_cast<Protocol::MessageType>(flag) == Protocol::MessageType::DATA_ATMOSPHERIC && from == nodeId) {
          Serial.printf("DEBUG: Tipo de mensaje y remitente coinciden.\n");

          // 1. Verificar tamaño del payload recibido
          if (current_recv_len != expectedAtmosphericDataSize) {
            Serial.printf("DEBUG: ERROR: Tamano de payload INCORRECTO. Recibido: ");
            Serial.printf("%d", current_recv_len);
            Serial.printf(" bytes, Esperado: ");
            Serial.printf("%d", expectedAtmosphericDataSize);
            Serial.printf(" bytes.\n");


            // No se modifica 't', el bucle continuará si hay reintentos.
          } else {
            Serial.printf("DEBUG: Tamano de payload CORRECTO. Realizando memcpy.\n");
            // 2. Convertir a nuestra estructura
            memcpy(atmosSamples.data(), buf, current_recv_len);
            AtmosphericSampleNodes[nodeId] = atmosSamples;
            Serial.printf("DEBUG: Datos de nodo ");
            Serial.printf("%02X", nodeId);
            Serial.printf(" almacenados.\n");
            
            // Publicar datos por MQTT
            for (const auto& sample : atmosSamples) {
                publishAtmosphericData(nodeId, sample);
            }
            
            t = true;  // Marca como exitoso



            //_--------------------------------borrar
            if (1 == 1) {
              Serial.printf("--- DEBUG: Imprimiendo muestras atmosfericas almacenadas ---\n");

              if (AtmosphericSampleNodes.empty()) {
                Serial.printf("DEBUG: El mapa AtmosphericSampleNodes esta vacio.\n");
                return;
              }

              // Itera sobre cada entrada (par clave-valor) en el mapa
              for (const auto &pair : AtmosphericSampleNodes) {
                uint8_t nodeId = pair.first;  // La clave es el ID del nodo
                            // Accedemos al std::array de AtmosphericSample usando .second
            const std::array<Protocol::AtmosphericSample, NUMERO_MUESTRAS_ATMOSFERICAS> &samples = pair.second;

                Serial.printf("DEBUG: Nodo ID: ");
                Serial.printf("%02X", nodeId);  // Convierte el ID a String hexadecimal para imprimir
                Serial.printf(" ----\n");

                // Itera sobre cada AtmosphericSample dentro del array para este nodo
                for (size_t i = 0; i < samples.size(); ++i) {
                  const Protocol::AtmosphericSample &sample = samples[i];  // Obtén una referencia a la muestra actual

                  Serial.printf("DEBUG:   Muestra ");
                  Serial.printf("%d", i + 1);  // Para empezar a contar desde 1
                  Serial.printf(": ");

                  // Temperatura: Se divide por 10.0 para obtener el valor flotante
                  Serial.printf("Temp=");
                  Serial.printf("%.1f", (float)sample.temp / 10.0);  // Imprimir float
                  Serial.printf("C, ");

                  // Humedad: Se divide por 10.0 para obtener el valor flotante
                  Serial.printf("Hum=");
                  Serial.printf("%.1f", (float)sample.moisture / 10.0);  // Imprimir float
                  Serial.printf("%%");

                  // Hora y Minuto: Requiere lógica de formato manual para el cero inicial
                  Serial.printf(", Hora=");
                  if (sample.hour < 10) {
                    Serial.printf("0");  // Rellena con cero si es necesario
                  }
                  Serial.printf("%d", sample.hour);
                  Serial.printf(":");
                  if (sample.minute < 10) {
                    Serial.printf("0");  // Rellena con cero si es necesario
                  }
                  Serial.printf("%d", sample.minute);

                  Serial.printf("\n");  // Nueva línea para la siguiente muestra
                }
                Serial.printf("DEBUG: ---- Fin de muestras para Nodo ID: 0x");
                Serial.printf("%02X", nodeId);
                Serial.printf(" ----\n");
              }
              Serial.printf("--- DEBUG: Fin de impresion de muestras atmosfericas ---\n");
            }




            break;  // Salir del bucle while
          }
        } else {
          Serial.printf("DEBUG: Mensaje recibido pero NO ES LA RESPUESTA ESPERADA (tipo o remitente incorrecto).\n");
        }
      }
      if (t == false) {
        // Delay antes de reintentar
        delay(DELAY_BEFORE_RETRY_ATMOSPHERIC);
        radio.sendMessage(nodeId, &key, sizeof(key), flag);
      }

    }  // Fin del while de reintentos

    if (!t) {
      Serial.printf("DEBUG: Fallo al obtener datos de nodo 0x");
      Serial.printf("%02X", nodeId);
      Serial.printf(" despues de todos los intentos.\n");
    }
    Serial.printf("DEBUG: ---\n");  // Separador entre nodos
    
    // Delay entre procesamiento de nodos
    delay(DELAY_BETWEEN_NODES);
  }                               // Fin del for de nodos
  Serial.printf("DEBUG: [requestAtmosphericData] Finalizado el ciclo de solicitud.\n");
}


/**
 * @brief solicita los datos actuales de Ground y Gps a los nodos.
 */
void AppLogic::requestGroundGpsData() {

  // DEBUG: Inicio de la función principal de solicitud de datos de suelo y GPS.
  Serial.printf("DEBUG: [requestGroundGpsData] -- INICIO --\n");

  // std::array<GroundGpsPacket, CANTIDAD_MUESTRAS_SUELO> groundSamples; // Ya está definido como miembro del mapa
  uint8_t buf[RH_MESH_MAX_MESSAGE_LEN] = { 0 };  // Búfer para el mensaje recibido
  uint8_t len = sizeof(buf);                     // Longitud máxima del búfer
  uint8_t from;                                  // Dirección del remitente
  uint8_t flag = static_cast<uint8_t>(Protocol::MessageType::REQUEST_DATA_GPC_GROUND);
  uint8_t nodeId;  // FLAG de detección protocolo
  uint8_t key = Protocol::KEY;

      size_t expectedGroundcDataSize = sizeof(Protocol::GroundGpsPacket);

  // DEBUG: Mostrar tamaño esperado de los datos.
  Serial.printf("DEBUG: Tamano esperado de datos: ");
  Serial.printf("%d", expectedGroundcDataSize);
  Serial.printf(" bytes.\n");


  for (const auto &pair : mapNodesIDsMac) {
    nodeId = pair.first;

    // Inicio del contador de tiempo para este nodo
    unsigned long tiempoInicioNodo = millis();

    // DEBUG: Procesando un nuevo nodo.
    Serial.printf("DEBUG: Procesando nodo ID: 0x");
    Serial.printf("%02X\n", nodeId);
    Serial.printf("\n");

    Serial.printf("DEBUG: Enviando REQUEST_DATA_GPC_GROUND a 0x");
    Serial.printf("%02X", nodeId);
    Serial.printf(".\n");

    radio.sendMessage(nodeId, &key, sizeof(key), flag);

    // Delay antes de esperar respuesta
    delay(DELAY_BEFORE_RETRY_GROUND);

    bool t = false;
    uint8_t intentos = 0;
    while (t == false && intentos <= connectionRetries) {

      intentos++;
      // DEBUG: Mostrar el número de intento actual.
      Serial.printf("DEBUG: Intento de recepcion #");
      Serial.printf("%d", intentos);
      Serial.printf(" para nodo 0x");
      Serial.printf("%02X", nodeId);
      Serial.printf(".\n");

      // Inicio del contador de tiempo para este intento
      unsigned long tiempoInicio = millis();
      Serial.printf("DEBUG: Iniciando espera de respuesta... \n");

      // Intenta recibir un mensaje.
      if (radio.recvMessageTimeout(buf, &len, &from, &flag, TIMEOUTGRAL)) {

        // Calcular tiempo transcurrido
        unsigned long tiempoTranscurrido = millis() - tiempoInicio;
        Serial.printf("DEBUG: Respuesta recibida en ");
        Serial.printf("%lu", tiempoTranscurrido);
        Serial.printf(" ms.\n");

        // DEBUG: Mensaje recibido. Mostrar remitente, tipo y longitud.
        Serial.printf("DEBUG: Mensaje recibido de 0x");
        Serial.printf("%02X", from);
        Serial.printf(", Tipo: 0x");
        Serial.printf("%02X", flag);
        Serial.printf(", Longitud: ");
        Serial.printf("%d\n", len);

        if (static_cast<Protocol::MessageType>(flag) == Protocol::MessageType::DATA_GPS_CROUND && from == nodeId) {
          // DEBUG: El mensaje es el esperado.
          Serial.printf("DEBUG: Mensaje es DATA_GPS_CROUND del nodo correcto.\n");

          // 1. Verificar tamaño
          if (len != expectedGroundcDataSize) {

            // DEBUG: Mensaje de error por tamaño incorrecto.
            Serial.printf("DEBUG: ERROR: Tamano incorrecto. Recibido: ");
            Serial.printf("%d", len);
            Serial.printf(", Esperado: ");
            Serial.printf("%d", expectedGroundcDataSize);
            Serial.printf(".\n");

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
                    Serial.printf("DEBUG: Paquete de nodo 0x");
                    Serial.printf("%02X", nodeId);
                    Serial.printf(" almacenado en posicion ");
                    Serial.printf("%d", countGroundSamples);
                    Serial.printf(".\n");
                    
                    // Publicar datos por MQTT
                    publishGroundData(nodeId, receivedPacket);
                    
                    countGroundSamples ++;
                    t = true; // Marca como exitoso
                    
                } else {
                    Serial.printf("DEBUG: ERROR: Posicion 'count' (");
                    Serial.printf("%d", countGroundSamples);
                    Serial.printf(") excede el tamano del array (");
                    Serial.printf("%d", CANTIDAD_MUESTRAS_SUELO);
                    Serial.printf("). Paquete no almacenado. reinicio contador.\n");
                    countGroundSamples = 0;
                    break;
                }

           
            //----------------------------borrar -----------------
            // Este bloque de debug está aquí en tu código original. No lo muevo para no alterar la lógica.
            if (1 == 1) {  // Esto es siempre verdadero, por lo que siempre se ejecuta.
              Serial.printf("--- DEBUG: Imprimiendo muestras de suelo y GPS almacenadas (DEBUG INTERNO) ---\n");

              if (groundGpsSamplesNodes.empty()) {
                Serial.printf("DEBUG: El mapa groundGpsSamplesNodes esta vacio.\n");
                // return; // Esta línea podría salir de la función principal, lo cual afectaría la lógica.
                // Mantengo tu código, pero si se llama aquí, no debería salir de la función requestGroundGpsData.
              }

              for (const auto &pair_inner : groundGpsSamplesNodes) {  // Usar 'pair_inner' para evitar conflicto de nombre
                uint8_t nodeId_inner = pair_inner.first;
                const std::array<Protocol::GroundGpsPacket, CANTIDAD_MUESTRAS_SUELO> &packets_inner = pair_inner.second;

                Serial.printf("DEBUG: Nodo ID (interno): 0x");
                Serial.printf("%02X", nodeId_inner);
                Serial.printf(" ----\n");

                for (size_t i_inner = 0; i_inner < packets_inner.size(); ++i_inner) {
                                  const Protocol::GroundGpsPacket &packet_inner = packets_inner[i_inner];
                const Protocol::GroundSensor &ground_inner = packet_inner.ground;
                const Protocol::GpsSensor &gps_inner = packet_inner.gps;

                  Serial.printf("DEBUG:   Paquete ");
                  Serial.printf("%d", i_inner + 1);
                  Serial.printf(" (Suelo + GPS): ");
                  Serial.printf("\n");

                  Serial.printf("DEBUG:     [SUELO] Temp=");
                  Serial.printf("%.1f", (float)ground_inner.temp / 10.0);
                  Serial.printf("C, Hum=");
                  Serial.printf("%.1f", (float)ground_inner.moisture / 10.0);
                  Serial.printf("%%, N=");
                  Serial.printf("%d", ground_inner.n);
                  Serial.printf(", P=");
                  Serial.printf("%d", ground_inner.p);
                  Serial.printf(", K=");
                  Serial.printf("%d", ground_inner.k);
                  Serial.printf(", EC=");
                  Serial.printf("%d", ground_inner.EC);
                  Serial.printf(", PH=");
                  Serial.printf("%.1f", (float)ground_inner.PH / 10.0);
                  Serial.printf("\n");

                  Serial.printf("DEBUG:     [GPS] Lat=");
                  Serial.printf("%.7f", (float)gps_inner.latitude / 10000000.0);
                  Serial.printf(", Lon=");
                  Serial.printf("%.7f", (float)gps_inner.longitude / 10000000.0);
                  Serial.printf(", Alt=");
                  Serial.printf("%d", gps_inner.altitude);
                  Serial.printf("m, Hora=");

                  if (gps_inner.hour < 10) { Serial.printf("0"); }
                  Serial.printf("%d", gps_inner.hour);
                  Serial.printf(":");
                  if (gps_inner.minute < 10) { Serial.printf("0"); }
                  Serial.printf("%d", gps_inner.minute);

                  Serial.printf(", Flags=");
                  Serial.printf("%02X", gps_inner.flags);

                  if (gps_inner.flags & 0x01) { Serial.printf(" (Ubicacion Valida)"); }
                  if (gps_inner.flags & 0x02) { Serial.printf(" (Altitud Valida)"); }
                  if (gps_inner.flags & 0x04) { Serial.printf(" (Hora Valida)"); }

                  Serial.printf("\n");
                  Serial.printf("DEBUG:   --------------------\n");
                }
                Serial.printf("DEBUG: ---- Fin de muestras para Nodo ID (interno): ");
                Serial.printf("%02X", nodeId_inner);
                Serial.printf(" ----\n");
              }
              Serial.printf("--- DEBUG: Fin de impresion de muestras de suelo y GPS (DEBUG INTERNO) ---\n");
            }  // Fin del if (1 == 1)
            //----------------------------borrar -----------------

            break;  // Salir del bucle while (reintentos)
          }
        } else {
          // DEBUG: Mensaje recibido pero no coincide con el tipo o remitente esperado.
          Serial.printf("DEBUG: Mensaje inesperado. Tipo recibido: 0x");
          Serial.printf("%02X", flag);
          Serial.printf(", Remitente recibido: 0x");
          Serial.printf("%02X", from);
          Serial.printf(".\n");
        }
      } else {
        // Calcular tiempo transcurrido en caso de timeout
        unsigned long tiempoTranscurrido = millis() - tiempoInicio;
        Serial.printf("DEBUG: Timeout despues de ");
        Serial.printf("%lu", tiempoTranscurrido);
        Serial.printf(" ms. No se recibio respuesta de ");
        Serial.printf("%02X", nodeId);
        Serial.printf(".\n");
      }
      if (t == false) {
        // DEBUG: Se reenvía el mensaje si no hubo éxito.
        Serial.printf("DEBUG: No exitoso. Reenviando solicitud a ");
        Serial.printf("%02X", nodeId);
        Serial.printf(".\n");
        // Delay antes de reintentar
        delay(DELAY_BEFORE_RETRY_GROUND);
        radio.sendMessage(nodeId, &key, sizeof(key), flag);
      }
    }  // Fin del while de reintentos

    if (!t) {
      // DEBUG: Fallo final al obtener datos.
      Serial.printf("DEBUG: Fallo definitivo para nodo ");
      Serial.printf("%02X", nodeId);
      Serial.printf(".\n");
    }
    
    // Resumen del tiempo total para este nodo
    unsigned long tiempoTotalNodo = millis() - tiempoInicioNodo;
    Serial.printf("DEBUG: Tiempo total para nodo ");
    Serial.printf("%02X", nodeId);
    Serial.printf(": ");
    Serial.printf("%lu", tiempoTotalNodo);
    Serial.printf(" ms.\n");
    
    Serial.printf("DEBUG: --- Fin de procesamiento para nodo ---\n");
    
    // Delay entre procesamiento de nodos
    delay(DELAY_BETWEEN_NODES);
  }  // Fin del for de nodos

  // DEBUG: Fin de la función principal.
  Serial.printf("DEBUG: [requestGroundGpsData] -- FIN --\n");
}
bool AppLogic::compareHsAndMs() {
    //DEBUG_PRINTLN("compareHsAndMs: Iniciando función");
    
    // Verificar si el RTC está funcionando correctamente
    DateTime now = rtc.getDateTime();
    if (!rtc.isDateTimeValid(now)) {
        //DEBUG_PRINTLN("compareHsAndMs: RTC no válido, retornando false");
        return false;
    }
    
    if (!rtc.isRunning()) {
        //DEBUG_PRINTLN("compareHsAndMs: RTC no está funcionando, retornando false");
        return false;
    }
    
    // Obtener la hora actual del RTC
    Serial.printf("compareHsAndMs: Obteniendo hora actual\n");
    String currentTime = rtc.getTimeString();
    Serial.printf("compareHsAndMs: Hora actual obtenida: ");
    Serial.printf("%s\n", currentTime.c_str());
    
    // Verificar que la hora obtenida sea válida (no "00:00" que indica error)
    if (currentTime == "00:00") {
        Serial.printf("compareHsAndMs: Hora inválida obtenida del RTC, retornando false\n");
        return false;
    }
    
    // Comparar con cada intervalo de horas configurado
    Serial.printf("compareHsAndMs: Comparando con ");
    Serial.printf("%d", CANTIDAD_MUESTRAS_SUELO);
    Serial.printf(" intervalos\n");
    
    for (int i = 0; i < CANTIDAD_MUESTRAS_SUELO; i++) {
        Serial.printf("compareHsAndMs: Procesando intervalo ");
        Serial.printf("%d\n", i);
        
        String targetTime = String(intervaloHorasSuelo[i]) + ":00";
        Serial.printf("compareHsAndMs: Hora objetivo: ");
        Serial.printf("%s\n", targetTime.c_str());
        
        Serial.printf("compareHsAndMs: Llamando a compareHsAndMs del RTC\n");
        if (rtc.compareHsAndMs(currentTime, targetTime)) {
            Serial.printf("compareHsAndMs: Coincidencia encontrada - Hora actual: ");
            Serial.printf("%s", currentTime.c_str());
            Serial.printf(" con intervalo: ");
            Serial.printf("%s\n", targetTime.c_str());
            return true;
        }
        Serial.printf("compareHsAndMs: No coincidencia para este intervalo\n");
    }
    
    Serial.printf("compareHsAndMs: No se encontraron coincidencias\n");
    return false;
}

// TODO: falta implementar
void AppLogic::handleUartRequest() {

  return;
}

// ===== MÉTODOS MQTT =====

bool AppLogic::connectWiFi() {
    if (WiFi.status() == WL_CONNECTED) {
        return true;
    }
    
    Serial.printf("Conectando a WiFi: %s\n", WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        Serial.printf(".");
        attempts++;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        Serial.printf("\nConectado a WiFi! IP: %s\n", WiFi.localIP().toString().c_str());
        wifiConnected = true;
        return true;
    } else {
        Serial.printf("\nError al conectar WiFi\n");
        wifiConnected = false;
        return false;
    }
}

bool AppLogic::connectMQTT() {
    if (!wifiConnected) {
        if (!connectWiFi()) {
            return false;
        }
    }
    
    if (mqttClient.connected()) {
        return true;
    }
    
    mqttClient.setClient(wifiClient);
    mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
    
    Serial.printf("Conectando a MQTT: %s:%d\n", MQTT_SERVER, MQTT_PORT);
    
    if (mqttClient.connect(MQTT_CLIENT_ID)) {
        Serial.printf("Conectado a MQTT!\n");
        mqttConnected = true;
        return true;
    } else {
        Serial.printf("Error al conectar MQTT\n");
        mqttConnected = false;
        return false;
    }
}

void AppLogic::publishAtmosphericData(uint8_t nodeId, const Protocol::AtmosphericSample& data) {
    if (!connectMQTT()) {
        Serial.printf("No se pudo conectar MQTT para publicar datos atmosféricos\n");
        return;
    }
    
    String payload = "{";
    payload += "\"nodeId\":" + String(nodeId);
    payload += ",\"temperature\":" + String(data.temp / 10.0, 1);
    payload += ",\"moisture\":" + String(data.moisture / 10.0, 1);
    payload += "}";
    
    if (mqttClient.publish(MQTT_TOPIC_ATMOSPHERIC, payload.c_str())) {
        Serial.printf("Datos atmosféricos publicados para nodo 0x%02X\n", nodeId);
    } else {
        Serial.printf("Error al publicar datos atmosféricos para nodo 0x%02X\n", nodeId);
    }
}

void AppLogic::publishGroundData(uint8_t nodeId, const Protocol::GroundGpsPacket& data) {
    if (!connectMQTT()) {
        Serial.printf("No se pudo conectar MQTT para publicar datos de suelo\n");
        return;
    }
    
    String payload = "{";
    payload += "\"nodeId\":" + String(nodeId);
    payload += ",\"temperature\":" + String(data.ground.temp / 10.0, 1);
    payload += ",\"moisture\":" + String(data.ground.moisture / 10.0, 1);
    payload += ",\"n\":" + String(data.ground.n);
    payload += ",\"p\":" + String(data.ground.p);
    payload += ",\"k\":" + String(data.ground.k);
    payload += ",\"ec\":" + String(data.ground.EC);
    payload += ",\"ph\":" + String(data.ground.PH / 10.0, 1);
    payload += ",\"volt\":3.33";
    payload += ",\"latitude\":" + String(data.gps.latitude, 7);
    payload += ",\"longitude\":" + String(data.gps.longitude, 7);
    payload += "}";
    
    if (mqttClient.publish(MQTT_TOPIC_GROUND, payload.c_str())) {
        Serial.printf("Datos de suelo publicados para nodo 0x%02X\n", nodeId);
    } else {
        Serial.printf("Error al publicar datos de suelo para nodo 0x%02X\n", nodeId);
        
    }
}