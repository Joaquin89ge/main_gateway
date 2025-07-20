Instalación del Sistema Gateway
===============================

Esta guía te ayudará a instalar y configurar el Sistema Gateway Agrícola en tu entorno de desarrollo.

Requisitos del Sistema
----------------------

Hardware Requerido
~~~~~~~~~~~~~~~~~~

* **ESP8266** (NodeMCU, Wemos D1 Mini, o similar)
* **Módulo LoRa SX1278** (RFM95W)
* **RTC DS1302** con interfaz 3-wire
* **Antena LoRa** (433MHz)
* **Cables de conexión**
* **Fuente de alimentación** (3.3V, 500mA mínimo)

Software Requerido
~~~~~~~~~~~~~~~~~~

* **PlatformIO** o **Arduino IDE**
* **Python 3.7+** (para documentación)
* **Git** (para control de versiones)

Instalación del Entorno de Desarrollo
------------------------------------

1. Instalar PlatformIO
~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: bash

   # Instalar PlatformIO Core
   pip install platformio

   # Verificar instalación
   pio --version

2. Clonar el Repositorio
~~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: bash

   # Clonar el repositorio
   git clone https://github.com/agro-iot/gateway.git
   cd gateway

   # Cambiar a la rama de desarrollo
   git checkout doc_generate

3. Instalar Dependencias
~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: bash

   # Instalar dependencias de Python para documentación
   pip install sphinx sphinx-rtd-theme myst-parser

   # Instalar dependencias del proyecto
   pio lib install "RadioHead"
   pio lib install "CRC32"
   pio lib install "LittleFS"
   pio lib install "andrewrapp/RtcDS1302"

Configuración del Hardware
-------------------------

Diagrama de Conexiones
~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: text

   ESP8266          SX1278 LoRa
   --------         -----------
   D2 (GPIO4)  -->  NSS (CS)
   D1 (GPIO5)  -->  DIO0 (INT)
   D5 (GPIO14) -->  SCK
   D7 (GPIO13) -->  MOSI
   D6 (GPIO12) -->  MISO
   GND         -->  GND
   3.3V        -->  VCC

   ESP8266          DS1302 RTC
   --------         -----------
   D0 (GPIO16) -->  RST
   D2 (GPIO4)  -->  DAT
   D3 (GPIO0)  -->  CLK
   GND         -->  GND
   3.3V        -->  VCC

Pasos de Conexión
~~~~~~~~~~~~~~~~~

1. **Conectar SX1278 LoRa:**
   - Conectar pines SPI según diagrama
   - Asegurar conexión de antena
   - Verificar alimentación 3.3V

2. **Conectar DS1302 RTC:**
   - Conectar pines 3-wire según diagrama
   - Verificar batería de backup
   - Confirmar niveles de voltaje

3. **Verificar Conexiones:**
   - Usar multímetro para verificar continuidad
   - Confirmar que no hay cortocircuitos
   - Verificar polaridad de componentes

Configuración del Software
-------------------------

1. Configurar PlatformIO
~~~~~~~~~~~~~~~~~~~~~~~~~

Editar el archivo ``platformio.ini``:

.. code-block:: ini

   [env:esp12e]
   platform = espressif8266
   board = esp12e
   framework = arduino
   monitor_speed = 115200
   upload_speed = 921600
   lib_deps = 
       RadioHead
       CRC32
       LittleFS
       andrewrapp/RtcDS1302

2. Configurar Parámetros
~~~~~~~~~~~~~~~~~~~~~~~~

Editar el archivo ``src/config.h`` según tus necesidades:

.. code-block:: cpp

   // Configuración de pines
   #define RFM95_CS 2
   #define RFM95_INT 5
   #define RTC_CLK 18
   #define RTC_DAT 19
   #define RTC_RST 4

   // Configuración de red
   #define MAX_NODES 250
   #define TIMEOUTGRAL 2000
   #define INTERVALOANNOUNCE 5000

Compilación y Subida
--------------------

1. Compilar el Proyecto
~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: bash

   # Compilar para ESP8266
   pio run --target build

2. Subir Firmware
~~~~~~~~~~~~~~~~~

.. code-block:: bash

   # Subir firmware (reemplazar COM7 con tu puerto)
   pio run --target upload --upload-port COM7

3. Verificar Instalación
~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: bash

   # Abrir monitor serial
   pio device monitor --port COM7

   # Deberías ver mensajes como:
   # "Inicializando LittleFS en sketch principal..."
   # "RF95 MESH init okay"
   # "RTC inicializado correctamente"

Configuración de Red
-------------------

1. Configurar Frecuencia LoRa
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Editar en ``src/radio_manager.cpp``:

.. code-block:: cpp

   // Configurar frecuencia (433MHz para Europa)
   driver.setFrequency(433.0);

2. Configurar Potencia de Transmisión
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: cpp

   // Configurar potencia (hasta 20dBm)
   driver.setTxPower(20, false);

3. Configurar Parámetros de Red
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: cpp

   // Configurar spreading factor (SF7-SF12)
   driver.setSignalBandwidth(125E3);
   driver.setCodingRate4(5);
   driver.setSpreadingFactor(7);

Verificación del Sistema
-----------------------

1. Pruebas Básicas
~~~~~~~~~~~~~~~~~~

.. code-block:: bash

   # Verificar comunicación LoRa
   pio device monitor --port COM7
   # Buscar mensajes: "RF95 MESH init okay"

   # Verificar RTC
   # Buscar mensajes: "RTC inicializado correctamente"

2. Pruebas de Red
~~~~~~~~~~~~~~~~~

.. code-block:: bash

   # Verificar anuncios de red
   # Buscar mensajes: "enviando announce KEY"

   # Verificar recepción de HELLO
   # Buscar mensajes: "AppLogic::handleHello()"

3. Pruebas de Datos
~~~~~~~~~~~~~~~~~~~

.. code-block:: bash

   # Verificar solicitud de datos
   # Buscar mensajes: "requestAtmosphericData()"

   # Verificar almacenamiento
   # Buscar mensajes: "Datos almacenados correctamente"

Solución de Problemas
--------------------

Problemas Comunes
~~~~~~~~~~~~~~~~~

1. **Error de Compilación:**
   - Verificar dependencias instaladas
   - Verificar versión de PlatformIO
   - Limpiar cache: ``pio run --target clean``

2. **Error de Subida:**
   - Verificar puerto COM correcto
   - Mantener botón FLASH presionado durante subida
   - Verificar drivers de USB

3. **Error de Comunicación LoRa:**
   - Verificar conexiones de pines
   - Verificar antena conectada
   - Verificar alimentación estable

4. **Error de RTC:**
   - Verificar conexiones 3-wire
   - Verificar batería de backup
   - Verificar niveles de voltaje

Comandos de Diagnóstico
~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: bash

   # Verificar estado del sistema
   pio device monitor --port COM7 --baud 115200

   # Limpiar y recompilar
   pio run --target clean
   pio run --target build

   # Verificar dependencias
   pio lib list

   # Actualizar dependencias
   pio lib update

Próximos Pasos
--------------

Una vez que el sistema esté instalado y funcionando:

1. **Configurar Nodos Sensores:** Conectar nodos a la red
2. **Configurar Monitoreo:** Implementar sistema de logging
3. **Configurar Alertas:** Implementar notificaciones
4. **Optimizar Parámetros:** Ajustar según condiciones específicas

Para más información, consulta la documentación completa en las siguientes secciones. 