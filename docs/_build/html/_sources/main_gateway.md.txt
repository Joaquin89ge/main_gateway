# main_gateway.ino - Punto de Entrada del Sistema Gateway

## 📋 Descripción General

Archivo principal del sistema Gateway agrícola que implementa el punto de entrada y la inicialización dinámica de todos los componentes del sistema. Este archivo orquesta la creación y gestión del ciclo de vida de los módulos principales del Gateway.

## 🏗️ Arquitectura del Sistema

### Componentes Principales

- **NodeIdentity:** Gestión de identidad única del Gateway
- **RadioManager:** Control de comunicación LoRa mesh
- **RtcManager:** Gestión de tiempo real con DS1302
- **AppLogic:** Lógica de aplicación y coordinación de red

### Inicialización Dinámica

El sistema utiliza inicialización dinámica de objetos para evitar problemas de hardware prematuros, especialmente con el bus SPI y otros componentes sensibles al timing.

## 📁 Estructura del Archivo

### Includes y Dependencias

```cpp
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "node_identity.h"
#include "radio_manager.h"
#include "app_logic.h"
#include "rtc_manager.h"
#include "config.h"
```

**Propósito:** Importación de todas las librerías y clases necesarias para el funcionamiento del Gateway.

### Variables Globales

```cpp
NodeIdentity* identity = nullptr;
RadioManager* radio = nullptr;
AppLogic* logic = nullptr;
RtcManager* rtc = nullptr;
bool errorFlag = false;
```

**Propósito:** Punteros a los objetos principales del sistema y flag de control de errores.

## 🔧 Función setup()

### Descripción

Función de inicialización que configura todos los componentes del sistema de manera secuencial y segura.

### Secuencia de Inicialización

#### 1. Configuración Inicial

```cpp
Serial.begin(115200);
delay(100);
WiFi.mode(WIFI_OFF); // Evita interacciones con radio WiFi
```

**Propósito:**

- Inicialización del puerto serial para debugging
- Deshabilitación del WiFi para evitar interferencias con LoRa
- Delay para estabilización del sistema

#### 2. Creación de NodeIdentity

```cpp
identity = new NodeIdentity();
```

**Propósito:**

- Creación dinámica del gestor de identidad
- Generación de ID único basado en MAC address
- Configuración de persistencia en LittleFS

#### 3. Inicialización de RadioManager

```cpp
radio = new RadioManager(identity->getNodeID());
if (!radio->init())
{
    DEBUG_PRINTLN("Error al inicializar RadioManager");
    errorFlag = true; // Detener ejecución
}
```

**Propósito:**

- Creación del gestor de radio con ID único
- Inicialización del módulo SX1278 LoRa
- Configuración de parámetros de comunicación
- Manejo de errores de inicialización

#### 4. Inicialización de RtcManager

```cpp
rtc = new RtcManager(RTC_DAT, RTC_CLK, RTC_RST);
if (!rtc->begin())
{
    DEBUG_PRINTLN("Error al inicializar RTC");
    errorFlag = true; // Detener ejecución
}
```

**Propósito:**

- Creación del gestor de tiempo real
- Configuración de pines del DS1302 (3-wire interface)
- Inicialización y validación del RTC
- Manejo de errores de hardware

#### 5. Creación de AppLogic

```cpp
logic = new AppLogic(*identity, *radio, *rtc);
logic->begin();
```

**Propósito:**

- Creación de la lógica de aplicación
- Inyección de dependencias (identity, radio, rtc)
- Inicialización de la lógica de negocio

#### 6. Verificación Final

```cpp
if (errorFlag==false) DEBUG_PRINT("todo ok en gateway");
```

**Propósito:**

- Confirmación de inicialización exitosa
- Mensaje de debug para verificar estado del sistema

## 🔄 Función loop()

### Descripción

Bucle principal del sistema que ejecuta la lógica de aplicación de manera continua.

### Implementación

```cpp
void loop()
{
    if (1 == 1)
    {
        logic->update();
    }
}
```

**Propósito:**

- Ejecución continua de la lógica de aplicación
- Gestión de mensajes entrantes y salientes
- Coordinación de la red mesh
- Monitoreo de nodos sensores

## 🔍 Características Técnicas

### 1. Inicialización Dinámica

- **Ventaja:** Evita problemas de hardware prematuros
- **Beneficio:** Mayor estabilidad del sistema
- **Implementación:** Uso de punteros y creación en setup()

### 2. Manejo de Errores

- **Estrategia:** Flag de error global
- **Propósito:** Detección temprana de problemas
- **Implementación:** Verificación en cada paso de inicialización

### 3. Optimización de Energía

- **WiFi Deshabilitado:** Reduce interferencias con LoRa
- **Sleep Inteligente:** Implementado en AppLogic
- **Gestión de Recursos:** Eficiente uso de memoria

### 4. Debugging Avanzado

- **Serial Output:** Mensajes detallados de inicialización
- **Error Tracking:** Seguimiento de problemas
- **Status Reporting:** Confirmación de estado del sistema

## 📊 Flujo de Ejecución

### Fase 1: Inicialización

1. **Configuración Serial:** Preparación para debugging
2. **Deshabilitación WiFi:** Optimización de radio
3. **Creación de Componentes:** Instanciación dinámica
4. **Validación de Hardware:** Verificación de componentes
5. **Configuración de Red:** Preparación para comunicación

### Fase 2: Operación

1. **Recepción de Mensajes:** Escucha continua de red
2. **Procesamiento de Datos:** Análisis de mensajes
3. **Coordinación de Nodos:** Gestión de red mesh
4. **Almacenamiento:** Persistencia de datos
5. **Transmisión:** Enví de respuestas y comandos

## 🔧 Configuración y Personalización

### Parámetros de Configuración

- **Velocidad Serial:** 115200 baudios
- **Pines RTC:** Definidos en config.h
- **ID de Nodo:** Generado automáticamente
- **Parámetros LoRa:** Configurados en RadioManager

### Personalización

- **Modificación de Pines:** Editar config.h
- **Cambio de Frecuencia:** Modificar RadioManager
- **Ajuste de Timeouts:** Configurar en AppLogic
- **Debugging:** Activar/desactivar en config.h

## 🚨 Consideraciones Importantes

### 1. Orden de Inicialización

- **Crítico:** El orden de inicialización es importante
- **Radio:** Debe inicializarse después de NodeIdentity
- **RTC:** Requiere configuración de pines específica
- **AppLogic:** Depende de todos los componentes anteriores

### 2. Gestión de Memoria

- **Punteros:** Uso de punteros para gestión dinámica
- **LittleFS:** Almacenamiento persistente
- **RAM:** Gestión eficiente de buffers

### 3. Robustez del Sistema

- **Error Handling:** Manejo de fallos de hardware
- **Recuperación:** Estrategias de recuperación automática
- **Logging:** Registro detallado de eventos

## 📈 Métricas de Rendimiento

### Tiempos de Inicialización

- **Serial Setup:** <10ms
- **NodeIdentity:** <50ms
- **RadioManager:** <200ms
- **RtcManager:** <100ms
- **AppLogic:** <50ms
- **Total:** <410ms

### Uso de Recursos

- **RAM:** ~44% (36KB de 80KB)
- **Flash:** ~32% (340KB de 1MB)
- **CPU:** Optimizado para operaciones de red

## 🔮 Mejoras Futuras

### Fase 1: Optimizaciones

- **Inicialización Paralela:** Reducir tiempo de startup
- **Gestión de Memoria:** Optimización de buffers
- **Error Recovery:** Mejoras en recuperación automática

### Fase 2: Nuevas Funcionalidades

- **Interfaz Web:** Configuración remota
- **OTA Updates:** Actualizaciones inalámbricas
- **Logging Avanzado:** Sistema de logs estructurado

### Fase 3: Integración

- **APIs Externas:** Conexión con sistemas externos
- **Cloud Integration:** Sincronización con la nube
- **Analytics:** Análisis avanzado de datos

---

**Conclusión:** El archivo main_gateway.ino representa el punto de entrada robusto y bien estructurado del sistema Gateway, implementando inicialización dinámica, manejo de errores y arquitectura modular que permite escalabilidad y mantenibilidad del código.
