Referencia de API
================

Esta sección proporciona la referencia completa de la API del Sistema Gateway Agrícola.

NodeIdentity API
---------------

Constructor
~~~~~~~~~~

.. code-block:: cpp

   NodeIdentity();

**Descripción:** Constructor de la clase NodeIdentity.

**Parámetros:** Ninguno

**Retorno:** Instancia de NodeIdentity

getNodeID()
~~~~~~~~~~

.. code-block:: cpp

   uint8_t getNodeID(size_t blacklist_len = 2, const uint8_t *blacklist = defaultBlacklist);

**Descripción:** Obtiene el identificador único del nodo.

**Parámetros:**
- `blacklist_len`: Tamaño de la lista negra (default: 2)
- `blacklist`: Array de valores prohibidos (default: [0x00, 0xFF])

**Retorno:** ID único entre 1-254

**Ejemplo:**
.. code-block:: cpp

   NodeIdentity identity;
   uint8_t nodeID = identity.getNodeID();

getDeviceMAC()
~~~~~~~~~~~~~

.. code-block:: cpp

   String getDeviceMAC();

**Descripción:** Recupera la dirección MAC del hardware.

**Parámetros:** Ninguno

**Retorno:** MAC address en formato "XX:XX:XX:XX:XX:XX"

**Ejemplo:**
.. code-block:: cpp

   String mac = identity.getDeviceMAC();

saveGetway()
~~~~~~~~~~~

.. code-block:: cpp

   void saveGetway(const uint8_t getwayAdress);

**Descripción:** Guarda la dirección del gateway asociado.

**Parámetros:**
- `getwayAdress`: Dirección del gateway a guardar

**Retorno:** void

**Ejemplo:**
.. code-block:: cpp

   identity.saveGetway(0x10);

getGetway()
~~~~~~~~~~

.. code-block:: cpp

   bool getGetway(uint8_t &stored_getway);

**Descripción:** Recupera la dirección del gateway guardada.

**Parámetros:**
- `stored_getway`: Referencia donde se guardará la dirección

**Retorno:** true si existe gateway guardado, false en caso contrario

**Ejemplo:**
.. code-block:: cpp

   uint8_t gatewayAddr;
   if (identity.getGetway(gatewayAddr)) {
       Serial.print("Gateway: 0x");
       Serial.println(gatewayAddr, HEX);
   }

RadioManager API
---------------

Constructor
~~~~~~~~~~

.. code-block:: cpp

   RadioManager(uint8_t address);

**Descripción:** Constructor de la clase RadioManager.

**Parámetros:**
- `address`: Dirección única del nodo en la red

**Retorno:** Instancia de RadioManager

**Ejemplo:**
.. code-block:: cpp

   RadioManager radio(0x01);

init()
~~~~~

.. code-block:: cpp

   bool init();

**Descripción:** Inicializa el módulo de radio RFM95.

**Parámetros:** Ninguno

**Retorno:** true si la inicialización fue exitosa

**Ejemplo:**
.. code-block:: cpp

   if (!radio.init()) {
       Serial.println("Error al inicializar radio");
   }

sendMessage()
~~~~~~~~~~~

.. code-block:: cpp

   bool sendMessage(uint8_t to, uint8_t *data, uint8_t len, uint8_t flag);

**Descripción:** Envía mensaje a dirección específica.

**Parámetros:**
- `to`: Dirección de destino
- `data`: Puntero a datos a enviar
- `len`: Longitud del mensaje
- `flag`: Tipo de mensaje (protocolo)

**Retorno:** true si el envío fue exitoso

**Ejemplo:**
.. code-block:: cpp

   uint8_t data[] = {0x01, 0x02, 0x03};
   if (radio.sendMessage(0x10, data, 3, 0x01)) {
       Serial.println("Mensaje enviado exitosamente");
   }

recvMessage()
~~~~~~~~~~~~

.. code-block:: cpp

   bool recvMessage(uint8_t *buf, uint8_t *len, uint8_t *from, uint8_t *flag);

**Descripción:** Recibe mensaje de la red.

**Parámetros:**
- `buf`: Buffer para datos recibidos
- `len`: Puntero a longitud (entrada: máximo, salida: real)
- `from`: Puntero a dirección del remitente
- `flag`: Puntero a tipo de mensaje

**Retorno:** true si se recibió mensaje

**Ejemplo:**
.. code-block:: cpp

   uint8_t buffer[50];
   uint8_t len = 50;
   uint8_t from, flag;
   if (radio.recvMessage(buffer, &len, &from, &flag)) {
       Serial.print("Mensaje de 0x");
       Serial.print(from, HEX);
   }

recvMessageTimeout()
~~~~~~~~~~~~~~~~~~

.. code-block:: cpp

   bool recvMessageTimeout(uint8_t *buf, uint8_t *len, uint8_t *from, uint8_t *flag, uint16_t timeout);

**Descripción:** Recibe mensaje con timeout específico.

**Parámetros:**
- `buf`: Buffer para datos recibidos
- `len`: Puntero a longitud
- `from`: Puntero a dirección del remitente
- `flag`: Puntero a tipo de mensaje
- `timeout`: Tiempo máximo de espera en ms

**Retorno:** true si se recibió mensaje dentro del timeout

**Ejemplo:**
.. code-block:: cpp

   uint8_t buffer[50];
   uint8_t len = 50;
   uint8_t from, flag;
   if (radio.recvMessageTimeout(buffer, &len, &from, &flag, 5000)) {
       Serial.println("Mensaje recibido en 5 segundos");
   }

update()
~~~~~~~

.. code-block:: cpp

   void update();

**Descripción:** Actualiza estado interno del gestor.

**Parámetros:** Ninguno

**Retorno:** void

**Ejemplo:**
.. code-block:: cpp

   radio.update();

RtcManager API
-------------

Constructor
~~~~~~~~~~

.. code-block:: cpp

   RtcManager(int ioPin, int sclkPin, int cePin);

**Descripción:** Constructor de la clase RtcManager.

**Parámetros:**
- `ioPin`: Pin de datos bidireccional
- `sclkPin`: Pin de reloj
- `cePin`: Pin de habilitación

**Retorno:** Instancia de RtcManager

**Ejemplo:**
.. code-block:: cpp

   RtcManager rtc(RTC_DAT, RTC_CLK, RTC_RST);

begin()
~~~~~~

.. code-block:: cpp

   bool begin();

**Descripción:** Inicializa el módulo RTC DS1302.

**Parámetros:** Ninguno

**Retorno:** true si la inicialización fue exitosa

**Ejemplo:**
.. code-block:: cpp

   if (rtc.begin()) {
       Serial.println("RTC inicializado correctamente");
   }

getDateTime()
~~~~~~~~~~~~

.. code-block:: cpp

   RtcDateTime getDateTime();

**Descripción:** Obtiene fecha y hora actual del RTC.

**Parámetros:** Ninguno

**Retorno:** Fecha y hora actual como RtcDateTime

**Ejemplo:**
.. code-block:: cpp

   RtcDateTime now = rtc.getDateTime();
   Serial.print("Fecha: ");
   Serial.print(now.Year());
   Serial.print("/");
   Serial.print(now.Month());
   Serial.print("/");
   Serial.println(now.Day());

setDateTime()
~~~~~~~~~~~

.. code-block:: cpp

   bool setDateTime(const RtcDateTime& dateTime);

**Descripción:** Establece fecha y hora en el RTC.

**Parámetros:**
- `dateTime`: Nueva fecha/hora a establecer

**Retorno:** true si se estableció correctamente

**Ejemplo:**
.. code-block:: cpp

   RtcDateTime newTime(2025, 7, 20, 14, 30, 0);
   if (rtc.setDateTime(newTime)) {
       Serial.println("Fecha/hora establecida correctamente");
   }

isDateTimeValid()
~~~~~~~~~~~~~~~

.. code-block:: cpp

   bool isDateTimeValid();

**Descripción:** Verifica si la fecha/hora del RTC es válida.

**Parámetros:** Ninguno

**Retorno:** true si la fecha/hora es válida

**Ejemplo:**
.. code-block:: cpp

   if (rtc.isDateTimeValid()) {
       Serial.println("RTC funcionando correctamente");
   }

getTimeString()
~~~~~~~~~~~~~

.. code-block:: cpp

   String getTimeString();

**Descripción:** Obtiene hora actual en formato string.

**Parámetros:** Ninguno

**Retorno:** Hora en formato "HH:MM"

**Ejemplo:**
.. code-block:: cpp

   String currentTime = rtc.getTimeString();
   Serial.print("Hora actual: ");
   Serial.println(currentTime);

compareCurrentTimeWith()
~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: cpp

   bool compareCurrentTimeWith(const String& targetTime);

**Descripción:** Compara hora actual con horario objetivo.

**Parámetros:**
- `targetTime`: Horario objetivo ("HH:MM")

**Retorno:** true si coincide con hora actual

**Ejemplo:**
.. code-block:: cpp

   if (rtc.compareCurrentTimeWith("12:00")) {
       Serial.println("Es mediodía");
   }

isRunning()
~~~~~~~~~~

.. code-block:: cpp

   bool isRunning();

**Descripción:** Verifica si el RTC está funcionando.

**Parámetros:** Ninguno

**Retorno:** true si el RTC está funcionando

**Ejemplo:**
.. code-block:: cpp

   if (rtc.isRunning()) {
       Serial.println("RTC funcionando correctamente");
   }

AppLogic API
-----------

Constructor
~~~~~~~~~~

.. code-block:: cpp

   AppLogic(NodeIdentity identity, RadioManager radioMgr, RtcManager& rtcMgr);

**Descripción:** Constructor de la clase AppLogic.

**Parámetros:**
- `identity`: Gestor de identidad del nodo
- `radioMgr`: Gestor de comunicación LoRa
- `rtcMgr`: Gestor de tiempo real

**Retorno:** Instancia de AppLogic

**Ejemplo:**
.. code-block:: cpp

   AppLogic logic(identity, radio, rtc);

begin()
~~~~~~

.. code-block:: cpp

   void begin();

**Descripción:** Inicializa la lógica de aplicación.

**Parámetros:** Ninguno

**Retorno:** void

**Ejemplo:**
.. code-block:: cpp

   logic.begin();

update()
~~~~~~~

.. code-block:: cpp

   void update();

**Descripción:** Actualiza la lógica de aplicación.

**Parámetros:** Ninguno

**Retorno:** void

**Ejemplo:**
.. code-block:: cpp

   void loop() {
       logic.update();
   }

Estructuras de Datos
-------------------

AtmosphericSample
~~~~~~~~~~~~~~~~

.. code-block:: cpp

   struct AtmosphericSample {
       float temperature;    // Temperatura en °C
       float humidity;       // Humedad en %
       float pressure;       // Presión en hPa
       uint32_t timestamp;  // Timestamp de la muestra
   };

GroundGpsPacket
~~~~~~~~~~~~~~

.. code-block:: cpp

   struct GroundGpsPacket {
       float soilTemperature;  // Temperatura del suelo en °C
       float soilHumidity;     // Humedad del suelo en %
       float pH;               // pH del suelo
       float conductivity;     // Conductividad en μS/cm
       float latitude;         // Latitud GPS
       float longitude;        // Longitud GPS
       uint32_t timestamp;     // Timestamp de la muestra
   };

MessageHeader
~~~~~~~~~~~~

.. code-block:: cpp

   struct MessageHeader {
       uint8_t type;          // Tipo de mensaje
       uint8_t source;        // Dirección origen
       uint8_t destination;   // Dirección destino
       uint8_t length;        // Longitud de datos
       uint32_t timestamp;    // Timestamp
   };

Constantes del Sistema
---------------------

Tipos de Mensaje
~~~~~~~~~~~~~~~

.. code-block:: cpp

   #define MSG_TYPE_HELLO 0x01            // Mensaje HELLO
   #define MSG_TYPE_ANNOUNCE 0x02         // Mensaje ANNOUNCE
   #define MSG_TYPE_DATA_REQUEST 0x03     // Solicitud de datos
   #define MSG_TYPE_DATA_RESPONSE 0x04    // Respuesta de datos
   #define MSG_TYPE_CHANGE_ID 0x05        // Cambio de ID

Configuración de Red
~~~~~~~~~~~~~~~~~~~

.. code-block:: cpp

   #define MAX_NODES 250                    // Número máximo de nodos
   #define NUMERO_MUESTRAS_ATMOSFERICAS 8   // Muestras atmosféricas por nodo
   #define CANTIDAD_MUESTRAS_SUELO 2        // Muestras de suelo por nodo
   #define RH_MESH_MAX_MESSAGE_LEN 50       // Longitud máxima de mensaje

Timeouts y Delays
~~~~~~~~~~~~~~~~

.. code-block:: cpp

   #define TIMEOUTGRAL 2000                 // Timeout general (2 segundos)
   #define INTERVALOANNOUNCE 5000           // Intervalo de announce (5 segundos)
   #define INTERVALOATMOSPHERIC 30000       // Intervalo atmosférico (30 segundos)
   #define DELAY_BETWEEN_NODES 200          // Delay entre nodos (200ms)

Pines de Hardware
~~~~~~~~~~~~~~~~

.. code-block:: cpp

   #define RFM95_CS 2   // Pin Chip Select (D4 en ESP8266)
   #define RFM95_INT 5  // Pin Interrupción (D1 en ESP8266)
   #define RTC_CLK 18   // Pin CLK del DS1302 (D3 en ESP8266)
   #define RTC_DAT 19   // Pin DAT del DS1302 (D2 en ESP8266) 
   #define RTC_RST 4    // Pin RST del DS1302 (D0 en ESP8266)

Ejemplos de Uso
--------------

Inicialización Completa
~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: cpp

   #include "node_identity.h"
   #include "radio_manager.h"
   #include "rtc_manager.h"
   #include "app_logic.h"

   NodeIdentity identity;
   RadioManager radio(identity.getNodeID());
   RtcManager rtc(RTC_DAT, RTC_CLK, RTC_RST);
   AppLogic logic(identity, radio, rtc);

   void setup() {
       Serial.begin(115200);
       
       if (!radio.init()) {
           Serial.println("Error al inicializar radio");
           return;
       }
       
       if (!rtc.begin()) {
           Serial.println("Error al inicializar RTC");
           return;
       }
       
       logic.begin();
   }

   void loop() {
       logic.update();
   }

Comunicación Básica
~~~~~~~~~~~~~~~~~~

.. code-block:: cpp

   // Enviar mensaje
   uint8_t data[] = {0x01, 0x02, 0x03};
   if (radio.sendMessage(0x10, data, 3, MSG_TYPE_DATA_REQUEST)) {
       Serial.println("Mensaje enviado exitosamente");
   }

   // Recibir mensaje
   uint8_t buffer[50];
   uint8_t len = 50;
   uint8_t from, flag;
   if (radio.recvMessage(buffer, &len, &from, &flag)) {
       Serial.print("Mensaje recibido de 0x");
       Serial.print(from, HEX);
       Serial.print(" con flag 0x");
       Serial.println(flag, HEX);
   }

Programación Temporal
~~~~~~~~~~~~~~~~~~~

.. code-block:: cpp

   // Verificar si es hora de muestreo
   if (rtc.compareCurrentTimeWith("08:00")) {
       // Solicitar datos atmosféricos
       logic.requestAtmosphericData();
   }
   
   if (rtc.compareCurrentTimeWith("12:00")) {
       // Solicitar datos de suelo
       logic.requestGroundGpsData();
   }

Gestión de Datos
~~~~~~~~~~~~~~~

.. code-block:: cpp

   // Acceder a datos almacenados
   for (auto& node : logic.AtmosphericSampleNodes) {
       uint8_t nodeID = node.first;
       auto& samples = node.second;
       
       Serial.print("Datos del nodo 0x");
       Serial.print(nodeID, HEX);
       Serial.println(":");
       
       for (int i = 0; i < NUMERO_MUESTRAS_ATMOSFERICAS; i++) {
           auto& sample = samples[i];
           Serial.printf("Muestra %d: Temp=%.1f°C, Hum=%.1f%%\n", 
                        i, sample.temperature, sample.humidity);
       }
   }

Manejo de Errores
~~~~~~~~~~~~~~~~~

.. code-block:: cpp

   // Verificar estado del sistema
   if (!rtc.isDateTimeValid()) {
       Serial.println("RTC con problemas de integridad");
   }
   
   if (!rtc.isRunning()) {
       Serial.println("RTC detenido");
   }
   
   if (!radio.init()) {
       Serial.println("Error al inicializar radio");
   }

Para más información sobre el uso específico de cada API, consulta la documentación detallada de cada clase en las secciones correspondientes. 