Configuración del Sistema Gateway
================================

Esta guía detalla la configuración completa del Sistema Gateway Agrícola, incluyendo parámetros de red, hardware y software.

Configuración de Hardware
------------------------

Pines de Conexión
~~~~~~~~~~~~~~~~~

ESP8266 - SX1278 LoRa
^^^^^^^^^^^^^^^^^^^^^

.. code-block:: cpp

   // Configuración de pines LoRa
   #define RFM95_CS 2   // Pin Chip Select (D4 en ESP8266)
   #define RFM95_INT 5  // Pin Interrupción (D1 en ESP8266)

ESP8266 - DS1302 RTC
^^^^^^^^^^^^^^^^^^^^

.. code-block:: cpp

   // Configuración de pines RTC (3-wire interface)
   #define RTC_CLK 18   // Pin CLK del DS1302 (D3 en ESP8266)
   #define RTC_DAT 19   // Pin DAT del DS1302 (D2 en ESP8266) 
   #define RTC_RST 4    // Pin RST del DS1302 (D0 en ESP8266)

Configuración de Red
-------------------

Parámetros LoRa
~~~~~~~~~~~~~~~

.. code-block:: cpp

   // Configuración de frecuencia y potencia
   #define LORA_FREQUENCY 433.0    // Frecuencia en MHz
   #define LORA_TX_POWER 20        // Potencia en dBm (0-20)
   #define LORA_SPREADING_FACTOR 7 // SF7-SF12
   #define LORA_BANDWIDTH 125000   // Ancho de banda en Hz
   #define LORA_CODING_RATE 5      // Coding rate (4/5, 4/6, 4/7, 4/8)

Parámetros de Red Mesh
~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: cpp

   // Configuración de red
   #define MAX_NODES 250                    // Número máximo de nodos
   #define RH_MESH_MAX_MESSAGE_LEN 50       // Longitud máxima de mensaje
   #define MAC_STR_LEN_WITH_NULL 18         // Longitud de MAC con null

Timeouts y Delays
~~~~~~~~~~~~~~~~~

.. code-block:: cpp

   // Modo Debug
   #define TIMEOUTGRAL 2000                 // Timeout general (2 segundos)
   #define INTERVALOANNOUNCE 5000           // Intervalo de announce (5 segundos)
   #define INTERVALOATMOSPHERIC 30000       // Intervalo atmosférico (30 segundos)

   // Modo Producción
   #define TIMEOUTGRAL 1500                 // Timeout general (1.5 segundos)
   #define INTERVALOANNOUNCE 120000         // Intervalo de announce (2 minutos)
   #define INTERVALOATMOSPHERIC 480000      // Intervalo atmosférico (8 minutos)

Configuración de Datos
---------------------

Muestras por Nodo
~~~~~~~~~~~~~~~~~

.. code-block:: cpp

   // Configuración de muestras
   #define NUMERO_MUESTRAS_ATMOSFERICAS 8   // Muestras atmosféricas por nodo
   #define CANTIDAD_MUESTRAS_SUELO 2        // Muestras de suelo por nodo

Delays de Procesamiento
~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: cpp

   // Delays entre operaciones
   #define DELAY_BETWEEN_NODES 200          // Delay entre nodos (200ms)
   #define DELAY_BEFORE_RETRY_ATMOSPHERIC 2000  // Retry atmosférico (2 segundos)
   #define DELAY_BEFORE_RETRY_GROUND 4000   // Retry suelo/GPS (4 segundos)

Configuración de Debugging
-------------------------

Modo Debug
~~~~~~~~~~

.. code-block:: cpp

   #define DEBUG_MODE 

   #ifdef DEBUG_MODE
       #define DEBUG_PRINT(x) Serial.print(x)
       #define DEBUG_PRINTLN(x) Serial.println(x)
       // Configuraciones optimizadas para desarrollo
   #else
       #define DEBUG_PRINT(x)
       #define DEBUG_PRINTLN(x)
       // Configuraciones optimizadas para producción
   #endif

Configuración de NodeIdentity
----------------------------

Archivos de Persistencia
~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: cpp

   // Nombres de archivos para LittleFS
   #define NODE_ID_FILE "/node_id.json"      // Archivo para ID del nodo
   #define GATEWAY_ADDR_FILE "/gateway.json" // Archivo para dirección del gateway

Valores por Defecto
~~~~~~~~~~~~~~~~~~~

.. code-block:: cpp

   #define HASH_NOT_SET 255   // Valor no inicializado
   #define GETWAY_NOT_SET 255 // Gateway no configurado

   // Lista negra por defecto
   extern const uint8_t defaultBlacklist[2]; // [0x00, 0xFF]

Configuración de RadioManager
----------------------------

Parámetros de Inicialización
~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: cpp

   // Configuración del driver RFM95
   driver.setFrequency(433.0);           // Frecuencia en MHz
   driver.setTxPower(20, false);         // Potencia en dBm
   driver.setSignalBandwidth(125E3);     // Ancho de banda
   driver.setCodingRate4(5);             // Coding rate
   driver.setSpreadingFactor(7);         // Spreading factor

Configuración de RtcManager
--------------------------

Parámetros de Inicialización
~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: cpp

   // Configuración del RTC DS1302
   rtc.Begin();                          // Inicializar RTC
   rtc.SetIsWriteProtected(false);       // Habilitar escritura
   rtc.SetIsRunning(true);               // Iniciar oscilador

Configuración de AppLogic
------------------------

Horarios de Muestreo
~~~~~~~~~~~~~~~~~~~

.. code-block:: cpp

   // Horarios para muestreo de suelo
   const int intervaloHorasSuelo[CANTIDAD_MUESTRAS_SUELO] = {12, 24};

Parámetros de Reintentos
~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: cpp

   const uint8_t connectionRetries = 2;  // Reintentos para pedir datos

Configuración Avanzada
---------------------

Optimización de Memoria
~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: cpp

   // Configuración para optimizar uso de RAM
   #define BUFFER_SIZE 50                 // Tamaño de buffer
   #define MAX_MESSAGE_LENGTH 50          // Longitud máxima de mensaje
   #define MAX_RETRIES 3                  // Máximo número de reintentos

Optimización de Energía
~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: cpp

   // Configuración para ahorro de energía
   #define SLEEP_DURATION 1000            // Duración de sleep en ms
   #define WAKEUP_INTERVAL 5000           // Intervalo de wakeup en ms
   #define LOW_POWER_MODE true            // Habilitar modo de baja potencia

Configuración de Seguridad
~~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: cpp

   // Configuración de seguridad
   #define ENABLE_ENCRYPTION false        // Habilitar cifrado
   #define ENABLE_AUTHENTICATION true     // Habilitar autenticación
   #define MAX_FAILED_ATTEMPTS 3          // Máximo intentos fallidos

Configuración por Entorno
------------------------

Desarrollo
~~~~~~~~~~

.. code-block:: cpp

   // Configuración para desarrollo
   #define DEBUG_MODE true
   #define LOG_LEVEL DEBUG
   #define ENABLE_SERIAL_OUTPUT true
   #define SHORT_TIMEOUTS true

Producción
~~~~~~~~~~

.. code-block:: cpp

   // Configuración para producción
   #define DEBUG_MODE false
   #define LOG_LEVEL INFO
   #define ENABLE_SERIAL_OUTPUT false
   #define SHORT_TIMEOUTS false

Testing
~~~~~~~

.. code-block:: cpp

   // Configuración para testing
   #define DEBUG_MODE true
   #define LOG_LEVEL DEBUG
   #define ENABLE_SERIAL_OUTPUT true
   #define SHORT_TIMEOUTS true
   #define MOCK_HARDWARE true

Configuración de Monitoreo
-------------------------

Logging
~~~~~~~

.. code-block:: cpp

   // Configuración de logging
   #define LOG_LEVEL DEBUG                // Nivel de logging
   #define LOG_TO_SERIAL true             // Logging a Serial
   #define LOG_TO_FILE false              // Logging a archivo
   #define LOG_BUFFER_SIZE 100            // Tamaño de buffer de log

Métricas
~~~~~~~~

.. code-block:: cpp

   // Configuración de métricas
   #define ENABLE_METRICS true            // Habilitar métricas
   #define METRICS_INTERVAL 60000         // Intervalo de métricas (1 min)
   #define METRICS_RETENTION 24           // Retención de métricas (horas)

Alertas
~~~~~~~

.. code-block:: cpp

   // Configuración de alertas
   #define ENABLE_ALERTS true             // Habilitar alertas
   #define ALERT_TEMPERATURE_HIGH 35      // Temperatura alta (°C)
   #define ALERT_TEMPERATURE_LOW 5        // Temperatura baja (°C)
   #define ALERT_HUMIDITY_HIGH 90         // Humedad alta (%)
   #define ALERT_HUMIDITY_LOW 20          // Humedad baja (%)

Configuración de Red Mesh
------------------------

Topología
~~~~~~~~~

.. code-block:: cpp

   // Configuración de topología
   #define MESH_MAX_HOPS 5                // Máximo número de saltos
   #define MESH_ROUTE_TIMEOUT 30000       // Timeout de ruta (30s)
   #define MESH_BROADCAST_INTERVAL 60000  // Intervalo de broadcast (1min)

Enrutamiento
~~~~~~~~~~~

.. code-block:: cpp

   // Configuración de enrutamiento
   #define ROUTING_ALGORITHM AODV         // Algoritmo de enrutamiento
   #define ROUTE_DISCOVERY_TIMEOUT 10000  // Timeout de descubrimiento
   #define ROUTE_MAINTENANCE_INTERVAL 30000 // Intervalo de mantenimiento

Configuración de Protocolo
-------------------------

Tipos de Mensaje
~~~~~~~~~~~~~~~~

.. code-block:: cpp

   // Tipos de mensaje del protocolo
   #define MSG_TYPE_HELLO 0x01            // Mensaje HELLO
   #define MSG_TYPE_ANNOUNCE 0x02         // Mensaje ANNOUNCE
   #define MSG_TYPE_DATA_REQUEST 0x03     // Solicitud de datos
   #define MSG_TYPE_DATA_RESPONSE 0x04    // Respuesta de datos
   #define MSG_TYPE_CHANGE_ID 0x05        // Cambio de ID

Estructura de Mensaje
~~~~~~~~~~~~~~~~~~~~~

.. code-block:: cpp

   // Estructura de mensaje
   struct MessageHeader {
       uint8_t type;                      // Tipo de mensaje
       uint8_t source;                    // Dirección origen
       uint8_t destination;               // Dirección destino
       uint8_t length;                    // Longitud de datos
       uint32_t timestamp;                // Timestamp
   };

Validación
~~~~~~~~~~

.. code-block:: cpp

   // Configuración de validación
   #define ENABLE_CRC_CHECK true          // Habilitar verificación CRC
   #define ENABLE_TIMESTAMP_CHECK true    // Habilitar verificación timestamp
   #define MAX_MESSAGE_AGE 300000         // Edad máxima de mensaje (5min)

Configuración de Persistencia
----------------------------

LittleFS
~~~~~~~~

.. code-block:: cpp

   // Configuración de LittleFS
   #define LITTLEFS_MAX_FILES 10          // Máximo número de archivos
   #define LITTLEFS_MAX_FILE_SIZE 1024    // Tamaño máximo de archivo
   #define LITTLEFS_ENABLE_FORMAT false   // Habilitar formateo automático

Backup
~~~~~~

.. code-block:: cpp

   // Configuración de backup
   #define ENABLE_AUTO_BACKUP true        // Habilitar backup automático
   #define BACKUP_INTERVAL 3600000        // Intervalo de backup (1h)
   #define BACKUP_RETENTION 7             // Retención de backups (días)

Configuración de Actualizaciones
------------------------------

OTA (Over-The-Air)
~~~~~~~~~~~~~~~~~~

.. code-block:: cpp

   // Configuración de actualizaciones OTA
   #define ENABLE_OTA true                // Habilitar actualizaciones OTA
   #define OTA_PORT 8266                  // Puerto para OTA
   #define OTA_PASSWORD "gateway123"      // Contraseña OTA
   #define OTA_HOSTNAME "gateway-agricola" // Hostname OTA

Verificación
~~~~~~~~~~~

.. code-block:: cpp

   // Configuración de verificación
   #define ENABLE_FIRMWARE_VERIFICATION true  // Verificación de firmware
   #define FIRMWARE_VERSION "1.0.0"           // Versión de firmware
   #define FIRMWARE_CHECKSUM_SIZE 32          // Tamaño de checksum

Configuración de Integración
---------------------------

APIs Externas
~~~~~~~~~~~~~

.. code-block:: cpp

   // Configuración de APIs
   #define ENABLE_CLOUD_SYNC false        // Habilitar sincronización con nube
   #define CLOUD_API_URL "https://api.agro-iot.com"  // URL de API
   #define CLOUD_API_KEY "your-api-key"   // Clave de API
   #define CLOUD_SYNC_INTERVAL 300000     // Intervalo de sincronización (5min)

Webhooks
~~~~~~~~

.. code-block:: cpp

   // Configuración de webhooks
   #define ENABLE_WEBHOOKS false          // Habilitar webhooks
   #define WEBHOOK_URL "https://webhook.site/your-url"  // URL de webhook
   #define WEBHOOK_TIMEOUT 5000           // Timeout de webhook (5s)

Configuración de Diagnóstico
---------------------------

Diagnóstico de Hardware
~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: cpp

   // Configuración de diagnóstico
   #define ENABLE_HARDWARE_DIAGNOSTIC true    // Habilitar diagnóstico
   #define DIAGNOSTIC_INTERVAL 60000          // Intervalo de diagnóstico (1min)
   #define DIAGNOSTIC_TIMEOUT 5000            // Timeout de diagnóstico (5s)

Métricas de Rendimiento
~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: cpp

   // Configuración de métricas
   #define ENABLE_PERFORMANCE_METRICS true    // Habilitar métricas
   #define METRICS_SAMPLE_INTERVAL 1000       // Intervalo de muestreo (1s)
   #define METRICS_RETENTION_PERIOD 3600      // Período de retención (1h)

Para aplicar cambios en la configuración, edita los archivos correspondientes y recompila el proyecto:

.. code-block:: bash

   # Recompilar después de cambios
   pio run --target clean
   pio run --target build
   pio run --target upload --upload-port COM7 