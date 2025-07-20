# 📚 Ejemplos y Tutoriales - Sistema Gateway Agrícola

## 🎯 Enfoque de esta Guía

Esta sección proporciona ejemplos mínimos pero funcionales, junto con enlaces a recursos externos para tutoriales más detallados.

## 🚀 Ejemplos de Código

### 📡 Configuración Básica del Gateway

```cpp
#include "app_logic.h"
#include "node_identity.h"
#include "radio_manager.h"
#include "rtc_manager.h"

// Configuración básica
NodeIdentity identity;
RadioManager radio(identity.getNodeID());
RtcManager rtc(RTC_DAT, RTC_CLK, RTC_RST);
AppLogic logic(identity, radio, rtc);

void setup() {
    Serial.begin(115200);

    // Inicializar componentes
    if (!radio.init()) {
        Serial.println("Error: Radio no inicializada");
        return;
    }

    if (!rtc.begin()) {
        Serial.println("Error: RTC no inicializado");
        return;
    }

    logic.begin();
    Serial.println("Gateway iniciado correctamente");
}

void loop() {
    logic.update();
    delay(100);
}
```

### 🔧 Configuración de Hardware

```cpp
// config.h - Configuración de pines
#define RFM95_CS 2    // D4
#define RFM95_INT 14  // D5
#define RFM95_RST 12  // D6

#define RTC_DAT 4     // D2
#define RTC_CLK 5     // D1
#define RTC_RST 0     // D3
```

### 📊 Acceso a Datos

```cpp
// Obtener datos atmosféricos de un nodo
auto& nodeData = logic.AtmosphericSampleNodes[0x42];
AtmosphericSample sample = nodeData[0];
Serial.printf("Temp: %.1f°C, Hum: %.1f%%\n",
    sample.temperature, sample.humidity);

// Obtener datos de suelo/GPS
auto& groundData = logic.groundGpsSamplesNodes[0x42];
GroundGpsPacket ground = groundData[0];
Serial.printf("Suelo: %.1f%%, pH: %.1f\n",
    ground.soilMoisture, ground.pH);
```

## 🔗 Tutoriales Externos

### 📚 Arduino y ESP8266

#### **Tutoriales Básicos**

- **Arduino Getting Started**: https://www.arduino.cc/en/Guide
- **ESP8266 Quick Start**: https://docs.espressif.com/projects/esp8266-rtos-sdk/en/latest/get-started/
- **PlatformIO Guide**: https://docs.platformio.org/en/latest/tutorials/

#### **Comunicación LoRa**

- **RadioHead Tutorial**: https://www.airspayce.com/mikem/arduino/RadioHead/
- **LoRa Basics**: https://lora-alliance.org/resource_hub/what-is-lora/
- **RFM95 Datasheet**: https://www.hoperf.com/rf_transceiver/rfm95.html

#### **RTC y Tiempo**

- **DS1302 Library**: https://github.com/andrewrapp/RtcDS1302
- **Time Management**: https://www.arduino.cc/reference/en/language/functions/time/

### 🛠️ Desarrollo y Debugging

#### **PlatformIO**

- **PlatformIO IDE**: https://docs.platformio.org/en/latest/integration/ide/
- **Debugging Guide**: https://docs.platformio.org/en/latest/advanced/debugging/
- **Library Management**: https://docs.platformio.org/en/latest/librarymanager/

#### **Git y Versionado**

- **Git Tutorial**: https://git-scm.com/doc
- **GitHub Guides**: https://guides.github.com/
- **Submodules**: https://git-scm.com/book/en/v2/Git-Tools-Submodules

### 📊 Documentación Técnica

#### **Sphinx y Autodoc**

- **Sphinx Documentation**: https://www.sphinx-doc.org/
- **Doxygen Manual**: https://www.doxygen.nl/manual/
- **Mermaid Diagrams**: https://mermaid-js.github.io/mermaid/

#### **Markdown y reStructuredText**

- **Markdown Guide**: https://www.markdownguide.org/
- **reStructuredText**: https://docutils.sourceforge.io/rst.html

## 🎯 Casos de Uso Comunes

### 🔍 Debugging de Comunicación

```cpp
// Habilitar debug detallado
#define DEBUG_MODE

// En config.h
#ifdef DEBUG_MODE
    #define DEBUG_PRINT(x) Serial.print(x)
    #define DEBUG_PRINTLN(x) Serial.println(x)
#else
    #define DEBUG_PRINT(x)
    #define DEBUG_PRINTLN(x)
#endif
```

### ⚙️ Configuración de Red

```cpp
// Ajustar parámetros de red
#define INTERVALOANNOUNCE 5000      // 5 segundos
#define INTERVALOATMOSPHERIC 30000  // 30 segundos
#define TIMEOUTGRAL 2000            // 2 segundos
```

### 📈 Monitoreo de Memoria

```cpp
// Verificar uso de memoria
Serial.printf("Memoria libre: %d bytes\n", ESP.getFreeHeap());
Serial.printf("Memoria total: %d bytes\n", ESP.getHeapSize());
```

## 🔧 Herramientas Recomendadas

### 📱 IDEs y Editores

- **Visual Studio Code**: https://code.visualstudio.com/
- **PlatformIO IDE**: https://platformio.org/ide
- **Arduino IDE**: https://www.arduino.cc/en/software

### 🔍 Herramientas de Debugging

- **Serial Monitor**: Integrado en Arduino IDE
- **PlatformIO Monitor**: `pio device monitor`
- **Wireshark**: https://www.wireshark.org/ (para análisis de red)

### 📊 Análisis de Datos

- **Arduino Serial Plotter**: Para visualización en tiempo real
- **Python Matplotlib**: https://matplotlib.org/ (para análisis avanzado)
- **Jupyter Notebooks**: https://jupyter.org/ (para análisis de datos)

## 🚨 Solución de Problemas

### ❌ Problemas Comunes

#### **Radio no inicializa**

- Verificar conexiones SPI
- Comprobar alimentación del módulo
- Revisar configuración de pines

#### **RTC no responde**

- Verificar conexiones de 3 hilos
- Comprobar batería del módulo
- Revisar configuración de pines

#### **Memoria insuficiente**

- Reducir MAX_NODES
- Optimizar almacenamiento de datos
- Usar PROGMEM para constantes

### 🔗 Recursos de Ayuda

- **Arduino Forum**: https://forum.arduino.cc/
- **ESP8266 Community**: https://community.espressif.com/
- **RadioHead Support**: https://www.airspayce.com/mikem/arduino/RadioHead/

## 📝 Notas de Desarrollo

### 🎯 Mejores Prácticas

1. **Siempre verificar inicialización** de componentes
2. **Usar DEBUG_MODE** durante desarrollo
3. **Monitorear memoria** regularmente
4. **Documentar cambios** en el código
5. **Probar en hardware real** frecuentemente

### ⚡ Optimizaciones

- **Usar PROGMEM** para strings largos
- **Minimizar uso de String** en favor de char\*
- **Optimizar bucles** críticos
- **Usar interrupciones** cuando sea posible

---

_Esta guía proporciona ejemplos mínimos y enlaces a recursos externos para tutoriales más detallados._ 📚✨
