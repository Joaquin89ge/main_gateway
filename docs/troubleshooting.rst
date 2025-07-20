Solución de Problemas
====================

Esta guía te ayudará a identificar y resolver problemas comunes del Sistema Gateway Agrícola.

Problemas de Compilación
-----------------------

Error: "RadioHead library not found"
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

**Síntomas:**
- Error de compilación: `fatal error: 'RHMesh.h' file not found`
- Error: `RadioHead library not found`

**Solución:**
.. code-block:: bash

   # Instalar librería RadioHead
   pio lib install "RadioHead"

   # O instalar manualmente
   pio lib install "https://github.com/airspayce/RadioHead.git"

Error: "RtcDS1302 library not found"
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

**Síntomas:**
- Error de compilación: `fatal error: 'RtcDS1302.h' file not found`
- Error: `RtcDS1302 library not found`

**Solución:**
.. code-block:: bash

   # Instalar librería RtcDS1302
   pio lib install "andrewrapp/RtcDS1302"

Error: "LittleFS library not found"
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

**Síntomas:**
- Error de compilación: `fatal error: 'LittleFS.h' file not found`
- Error: `LittleFS library not found`

**Solución:**
.. code-block:: bash

   # Instalar librería LittleFS
   pio lib install "LittleFS"

Error de Memoria Insuficiente
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

**Síntomas:**
- Error: `section '.text' will not fit in region 'iram0_0'`
- Error: `section '.data' will not fit in region 'dram0_0'`

**Solución:**
.. code-block:: cpp

   // Reducir buffers en config.h
   #define MAX_NODES 100                    // Reducir de 250 a 100
   #define NUMERO_MUESTRAS_ATMOSFERICAS 4   // Reducir de 8 a 4
   #define RH_MESH_MAX_MESSAGE_LEN 30       // Reducir de 50 a 30

Problemas de Subida
------------------

Error: "Failed to connect to ESP8266"
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

**Síntomas:**
- Error: `Failed to connect to ESP8266: Timed out waiting for packet header`
- Error: `Failed to connect to ESP8266: Invalid head of packet`

**Solución:**
1. **Verificar Conexión USB:**
   - Cambiar cable USB
   - Probar puerto USB diferente
   - Verificar drivers instalados

2. **Modo de Descarga:**
   - Mantener botón FLASH presionado
   - Presionar botón RESET
   - Soltar botón RESET
   - Soltar botón FLASH después de 2 segundos

3. **Verificar Puerto:**
.. code-block:: bash

   # Listar puertos disponibles
   pio device list

   # Usar puerto correcto
   pio run --target upload --upload-port COM7

Error: "Upload failed"
~~~~~~~~~~~~~~~~~~~~~~

**Síntomas:**
- Error: `Upload failed: Timed out waiting for packet header`
- Error: `Upload failed: Invalid head of packet`

**Solución:**
1. **Verificar Velocidad de Subida:**
.. code-block:: ini

   # En platformio.ini
   upload_speed = 921600

2. **Verificar Configuración de Board:**
.. code-block:: ini

   # En platformio.ini
   board = esp12e
   platform = espressif8266

3. **Limpiar y Recompilar:**
.. code-block:: bash

   pio run --target clean
   pio run --target build
   pio run --target upload

Problemas de Hardware
--------------------

Error de Comunicación LoRa
~~~~~~~~~~~~~~~~~~~~~~~~~~

**Síntomas:**
- Mensaje: `RF95 MESH init failed`
- No hay comunicación entre nodos
- Errores de timeout en transmisión

**Solución:**
1. **Verificar Conexiones:**
   - Confirmar pines SPI conectados correctamente
   - Verificar alimentación 3.3V estable
   - Confirmar antena conectada

2. **Verificar Configuración:**
.. code-block:: cpp

   // En config.h
   #define RFM95_CS 2   // Verificar pin CS
   #define RFM95_INT 5  // Verificar pin INT

3. **Diagnóstico de Hardware:**
.. code-block:: bash

   # Verificar comunicación SPI
   pio device monitor --port COM7
   # Buscar mensajes de inicialización

Error de RTC
~~~~~~~~~~~

**Síntomas:**
- Mensaje: `RTC initialization failed`
- Fecha/hora incorrecta
- RTC no responde

**Solución:**
1. **Verificar Conexiones 3-wire:**
   - Confirmar pines CLK, DAT, RST conectados
   - Verificar niveles de voltaje 3.3V
   - Confirmar batería de backup

2. **Verificar Configuración:**
.. code-block:: cpp

   // En config.h
   #define RTC_CLK 18   // Verificar pin CLK
   #define RTC_DAT 19   // Verificar pin DAT
   #define RTC_RST 4    // Verificar pin RST

3. **Diagnóstico de RTC:**
.. code-block:: bash

   # Verificar estado del RTC
   pio device monitor --port COM7
   # Buscar mensajes de RTC

Problemas de Red
---------------

Nodos No Detectados
~~~~~~~~~~~~~~~~~~~

**Síntomas:**
- No se reciben mensajes HELLO
- Nodos no aparecen en la red
- Timeouts de comunicación

**Solución:**
1. **Verificar Configuración de Frecuencia:**
.. code-block:: cpp

   // En radio_manager.cpp
   driver.setFrequency(433.0);  // Verificar frecuencia

2. **Verificar Potencia de Transmisión:**
.. code-block:: cpp

   // En radio_manager.cpp
   driver.setTxPower(20, false);  // Aumentar potencia si es necesario

3. **Verificar Distancia:**
   - Reducir distancia entre nodos
   - Verificar obstáculos
   - Probar con antenas de mayor ganancia

Pérdida de Paquetes
~~~~~~~~~~~~~~~~~~~

**Síntomas:**
- Mensajes perdidos ocasionalmente
- Timeouts frecuentes
- Datos incompletos

**Solución:**
1. **Ajustar Parámetros LoRa:**
.. code-block:: cpp

   // Aumentar spreading factor para mayor alcance
   driver.setSpreadingFactor(9);

   // Aumentar coding rate para mayor robustez
   driver.setCodingRate4(6);

2. **Ajustar Timeouts:**
.. code-block:: cpp

   // En config.h
   #define TIMEOUTGRAL 3000        // Aumentar timeout
   #define INTERVALOANNOUNCE 10000 // Aumentar intervalo

3. **Verificar Interferencias:**
   - Cambiar frecuencia si es necesario
   - Verificar fuentes de interferencia
   - Usar filtros si es necesario

Problemas de Memoria
-------------------

Desbordamiento de Memoria
~~~~~~~~~~~~~~~~~~~~~~~~~

**Síntomas:**
- Reinicios inesperados
- Comportamiento errático
- Errores de heap

**Solución:**
1. **Reducir Buffers:**
.. code-block:: cpp

   // En config.h
   #define MAX_NODES 100                    // Reducir número de nodos
   #define NUMERO_MUESTRAS_ATMOSFERICAS 4   // Reducir muestras
   #define RH_MESH_MAX_MESSAGE_LEN 30       // Reducir tamaño de mensaje

2. **Optimizar Almacenamiento:**
.. code-block:: cpp

   // Usar tipos de datos más pequeños
   uint8_t nodeID;  // En lugar de uint16_t
   uint16_t timestamp;  // En lugar de uint32_t

3. **Liberar Memoria:**
.. code-block:: cpp

   // Limpiar buffers periódicamente
   void cleanupMemory() {
       // Limpiar buffers no utilizados
   }

Problemas de Energía
-------------------

Consumo Excesivo
~~~~~~~~~~~~~~~~

**Síntomas:**
- Batería se agota rápidamente
- Voltaje inestable
- Reinicios por bajo voltaje

**Solución:**
1. **Optimizar Sleep:**
.. code-block:: cpp

   // Implementar sleep profundo
   ESP.deepSleep(60000000);  // Sleep por 1 minuto

2. **Reducir Transmisiones:**
.. code-block:: cpp

   // En config.h
   #define INTERVALOANNOUNCE 300000    // Aumentar intervalo (5 min)
   #define INTERVALOATMOSPHERIC 900000 // Aumentar intervalo (15 min)

3. **Optimizar Hardware:**
   - Usar regulador de voltaje eficiente
   - Verificar consumo de componentes
   - Implementar modo de baja potencia

Problemas de Datos
-----------------

Datos Corruptos
~~~~~~~~~~~~~~~

**Síntomas:**
- Valores de sensores incorrectos
- Datos fuera de rango
- Checksums fallidos

**Solución:**
1. **Verificar Validación:**
.. code-block:: cpp

   // Implementar validación de rangos
   if (temperature < -40 || temperature > 80) {
       // Datos inválidos
       return false;
   }

2. **Implementar Checksums:**
.. code-block:: cpp

   // Verificar integridad de datos
   uint8_t checksum = calculateChecksum(data, length);
   if (checksum != receivedChecksum) {
       // Datos corruptos
       return false;
   }

3. **Reintentos:**
.. code-block:: cpp

   // Implementar reintentos para datos críticos
   for (int i = 0; i < MAX_RETRIES; i++) {
       if (requestData()) {
           break;
       }
       delay(RETRY_DELAY);
   }

Problemas de Configuración
-------------------------

Configuración No Persistente
~~~~~~~~~~~~~~~~~~~~~~~~~~~

**Síntomas:**
- Configuración se pierde al reiniciar
- Archivos no se guardan en LittleFS
- Errores de escritura

**Solución:**
1. **Verificar LittleFS:**
.. code-block:: cpp

   // Inicializar LittleFS correctamente
   if (!LittleFS.begin()) {
       Serial.println("Error al inicializar LittleFS");
       return false;
   }

2. **Verificar Escritura:**
.. code-block:: cpp

   // Verificar escritura exitosa
   File file = LittleFS.open("/config.json", "w");
   if (!file) {
       Serial.println("Error al abrir archivo para escritura");
       return false;
   }

3. **Backup de Configuración:**
.. code-block:: cpp

   // Implementar backup automático
   void backupConfiguration() {
       // Crear backup de configuración
   }

Comandos de Diagnóstico
---------------------

Verificar Estado del Sistema
~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: bash

   # Verificar comunicación serial
   pio device monitor --port COM7 --baud 115200

   # Buscar mensajes de estado:
   # - "RF95 MESH init okay"
   # - "RTC inicializado correctamente"
   # - "NodeIdentity::getNodeID()"

Verificar Memoria
~~~~~~~~~~~~~~~~

.. code-block:: cpp

   // Función para verificar memoria disponible
   void checkMemory() {
       Serial.printf("Free heap: %d bytes\n", ESP.getFreeHeap());
       Serial.printf("Free stack: %d bytes\n", ESP.getFreeContStack());
   }

Verificar Red
~~~~~~~~~~~~

.. code-block:: cpp

   // Función para verificar estado de red
   void checkNetwork() {
       Serial.printf("Nodos activos: %d\n", activeNodes);
       Serial.printf("Paquetes enviados: %d\n", packetsSent);
       Serial.printf("Paquetes recibidos: %d\n", packetsReceived);
   }

Verificar RTC
~~~~~~~~~~~~

.. code-block:: cpp

   // Función para verificar RTC
   void checkRTC() {
       RtcDateTime now = rtc.getDateTime();
       Serial.printf("Fecha: %04d/%02d/%02d\n", 
                    now.Year(), now.Month(), now.Day());
       Serial.printf("Hora: %02d:%02d:%02d\n", 
                    now.Hour(), now.Minute(), now.Second());
   }

Logs de Diagnóstico
------------------

Habilitar Logs Detallados
~~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: cpp

   // En config.h
   #define DEBUG_MODE true
   #define LOG_LEVEL DEBUG
   #define ENABLE_SERIAL_OUTPUT true

   // En código
   #ifdef DEBUG_MODE
       Serial.printf("[DEBUG] %s: %s\n", __FUNCTION__, message);
   #endif

Interpretar Logs
~~~~~~~~~~~~~~~

**Logs de Inicialización:**
.. code-block:: text

   Inicializando LittleFS en sketch principal...
   RF95 MESH init okay
   RTC inicializado correctamente
   NodeIdentity::getNodeID() - ID: 0x42
   AppLogic::begin() - Gateway iniciado

**Logs de Comunicación:**
.. code-block:: text

   RadioManager::sendMessage() - Enviando a 0x10
   RadioManager::recvMessage() - Recibido de 0x10
   AppLogic::handleHello() - Nodo 0x10 registrado

**Logs de Error:**
.. code-block:: text

   ERROR: RF95 MESH init failed
   ERROR: RTC initialization failed
   ERROR: Memory allocation failed

Contacto y Soporte
-----------------

Si los problemas persisten:

1. **Recopilar Información:**
   - Logs completos del sistema
   - Configuración actual
   - Descripción detallada del problema

2. **Verificar Documentación:**
   - Revisar guías de instalación
   - Consultar configuración avanzada
   - Verificar requisitos del sistema

3. **Contactar Soporte:**
   - Email: soporte@agro-iot.com
   - GitHub: https://github.com/agro-iot/gateway/issues
   - Foro: https://forum.agro-iot.com

Para más información, consulta la documentación completa en las siguientes secciones. 