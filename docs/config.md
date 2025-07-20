# config.h - Configuración Central del Sistema Gateway

## 📋 Descripción General

Archivo de configuración central que define todos los parámetros, constantes y configuraciones del sistema Gateway agrícola. Este archivo actúa como punto único de configuración para facilitar el mantenimiento y personalización del sistema.

## 🏗️ Estructura de Configuración

### Modos de Operación

El sistema soporta dos modos de operación principales:

#### Modo Debug

```cpp
#define DEBUG_MODE
#ifdef DEBUG_MODE
    #define DEBUG_PRINT(x) Serial.print(x)
    #define DEBUG_PRINTLN(x) Serial.println(x)
    // Configuraciones optimizadas para desarrollo
#endif
```

**Características:**

- **Debugging Activo:** Salida detallada por Serial
- **Timeouts Reducidos:** Para desarrollo rápido
- **Intervalos Cortos:** Para testing intensivo
- **Logging Detallado:** Información completa del sistema

#### Modo Producción

```cpp
#else
    #define DEBUG_PRINT(x)
    #define DEBUG_PRINTLN(x)
    // Configuraciones optimizadas para producción
#endif
```

**Características:**

- **Debugging Deshabilitado:** Sin salida Serial
- **Timeouts Extendidos:** Para operación estable
- **Intervalos Largos:** Para conservación de energía
- **Logging Mínimo:** Solo información crítica

## 📊 Parámetros de Red

### Configuración de Nodos

```cpp
#define MAX_NODES 250                    ///< Número máximo de nodos en la red
#define NUMERO_MUESTRAS_ATMOSFERICAS 8   ///< Muestras atmosféricas por nodo
#define CANTIDAD_MUESTRAS_SUELO 2        ///< Muestras de suelo por nodo
```

**Propósito:**

- **Escalabilidad:** Soporte para redes grandes
- **Almacenamiento:** Optimización de memoria
- **Flexibilidad:** Configuración por tipo de dato

### Timeouts y Delays

```cpp
// Modo Debug
#define TIMEOUTGRAL 2000                 ///< Timeout general (2 segundos)
#define INTERVALOANNOUNCE 5000           ///< Intervalo de announce (5 segundos)
#define INTERVALOATMOSPHERIC 30000       ///< Intervalo atmosférico (30 segundos)

// Modo Producción
#define TIMEOUTGRAL 1500                 ///< Timeout general (1.5 segundos)
#define INTERVALOANNOUNCE 120000         ///< Intervalo de announce (2 minutos)
#define INTERVALOATMOSPHERIC 480000      ///< Intervalo atmosférico (8 minutos)
```

**Características:**

- **Adaptabilidad:** Diferentes valores según modo
- **Optimización:** Balance entre responsividad y energía
- **Escalabilidad:** Configuración por tipo de operación

### Delays de Procesamiento

```cpp
#define DELAY_BETWEEN_NODES 200          ///< Delay entre nodos (200ms)
#define DELAY_BEFORE_RETRY_ATMOSPHERIC 2000  ///< Retry atmosférico (2 segundos)
#define DELAY_BEFORE_RETRY_GROUND 4000   ///< Retry suelo/GPS (4 segundos)
```

**Propósito:**

- **Gestión de Red:** Evitar congestión
- **Recuperación:** Estrategias de retry
- **Optimización:** Balance entre velocidad y estabilidad

## 🔧 Configuración de Hardware

### Comunicación LoRa

```cpp
#define RFM95_CS 2   ///< Pin Chip Select (D4 en ESP8266)
#define RFM95_INT 5  ///< Pin Interrupción (D1 en ESP8266)
```

**Características:**

- **Configuración SPI:** Pines específicos para ESP8266
- **Optimización:** Selección de pines para mejor rendimiento
- **Compatibilidad:** Mapeo correcto de pines

### RTC DS1302 (3-wire interface)

```cpp
#define RTC_CLK 18   ///< Pin CLK del DS1302 (D3 en ESP8266)
#define RTC_DAT 19   ///< Pin DAT del DS1302 (D2 en ESP8266)
#define RTC_RST 4    ///< Pin RST del DS1302 (D0 en ESP8266)
```

**Características:**

- **Interfaz 3-wire:** Configuración específica para DS1302
- **Mapeo de Pines:** Compatibilidad con ESP8266
- **Optimización:** Selección de pines para estabilidad

### Configuración de Comunicación

```cpp
#define RH_MESH_MAX_MESSAGE_LEN 50       ///< Longitud máxima de mensaje
#define MAC_STR_LEN_WITH_NULL 18         ///< Longitud de MAC con null
```

**Propósito:**

- **Optimización de Memoria:** Buffers eficientes
- **Compatibilidad:** Con librerías RadioHead
- **Escalabilidad:** Configuración para redes grandes

## 🔍 Macros de Debugging

### Sistema de Logging

```cpp
#ifdef DEBUG_MODE
    #define DEBUG_PRINT(x) Serial.print(x)
    #define DEBUG_PRINTLN(x) Serial.println(x)
#else
    #define DEBUG_PRINT(x)
    #define DEBUG_PRINTLN(x)
#endif
```

**Características:**

- **Compilación Condicional:** Optimización automática
- **Flexibilidad:** Fácil activación/desactivación
- **Eficiencia:** Sin overhead en producción

## 📈 Configuraciones Avanzadas

### Parámetros de Comunicación

```cpp
// Configuraciones comentadas para futuras implementaciones
//#define DELAY_BETWEEN_ATMOSPHERIC_REQUESTS 2000
//#define DELAY_BETWEEN_GROUND_REQUESTS 5000
```

**Propósito:**

- **Extensibilidad:** Preparación para futuras funcionalidades
- **Documentación:** Referencia para desarrolladores
- **Flexibilidad:** Configuración granular

## 🔧 Personalización y Optimización

### Modificación de Parámetros

#### 1. Cambio de Pines

```cpp
// Para cambiar pines LoRa
#define RFM95_CS 15   // Nuevo pin CS
#define RFM95_INT 13  // Nuevo pin INT

// Para cambiar pines RTC
#define RTC_CLK 14    // Nuevo pin CLK
#define RTC_DAT 12    // Nuevo pin DAT
#define RTC_RST 16    // Nuevo pin RST
```

#### 2. Ajuste de Timeouts

```cpp
// Para redes más lentas
#define TIMEOUTGRAL 3000
#define INTERVALOANNOUNCE 10000

// Para redes más rápidas
#define TIMEOUTGRAL 1000
#define INTERVALOANNOUNCE 3000
```

#### 3. Configuración de Red

```cpp
// Para redes pequeñas
#define MAX_NODES 50
#define NUMERO_MUESTRAS_ATMOSFERICAS 4

// Para redes grandes
#define MAX_NODES 500
#define NUMERO_MUESTRAS_ATMOSFERICAS 16
```

## 🚨 Consideraciones Importantes

### 1. Compatibilidad de Hardware

- **ESP8266:** Configuración específica para este microcontrolador
- **Pines:** Selección cuidadosa para evitar conflictos
- **Voltajes:** Compatibilidad con niveles lógicos de 3.3V

### 2. Optimización de Energía

- **Modo Debug:** Mayor consumo para desarrollo
- **Modo Producción:** Optimización para baterías
- **Sleep Modes:** Configuración para ahorro energético

### 3. Escalabilidad

- **Memoria:** Consideración de límites de RAM
- **Red:** Configuración para número de nodos
- **Comunicación:** Balance entre velocidad y estabilidad

## 📊 Métricas de Configuración

### Uso de Recursos

| Parámetro     | Debug  | Producción | Impacto               |
| ------------- | ------ | ---------- | --------------------- |
| **RAM**       | ~50%   | ~44%       | Memoria de buffers    |
| **Flash**     | ~35%   | ~32%       | Código de debug       |
| **Energía**   | Alto   | Bajo       | Intervalos y timeouts |
| **Velocidad** | Rápida | Moderada   | Responsividad         |

### Optimizaciones

- **Debug Mode:** +15% RAM, +10% Flash
- **Producción:** Optimizado para eficiencia
- **Flexibilidad:** Configuración por requerimientos

## 🔮 Mejoras Futuras

### Fase 1: Configuración Dinámica

- **Configuración por UART:** Cambio de parámetros en tiempo real
- **Configuración por Web:** Interfaz web para configuración
- **Configuración por LoRa:** Actualización remota de parámetros

### Fase 2: Optimizaciones Avanzadas

- **Auto-tuning:** Ajuste automático de parámetros
- **Machine Learning:** Optimización basada en uso
- **Adaptive Timeouts:** Timeouts dinámicos según condiciones

### Fase 3: Integración

- **Cloud Configuration:** Sincronización con la nube
- **Multi-environment:** Configuraciones por ambiente
- **Versioning:** Control de versiones de configuración

## 📋 Checklist de Configuración

### Antes del Despliegue

- [ ] **Verificar Pines:** Compatibilidad con hardware
- [ ] **Ajustar Timeouts:** Según condiciones de red
- [ ] **Configurar Modo:** Debug vs Producción
- [ ] **Validar Parámetros:** Rango de valores correctos
- [ ] **Probar Comunicación:** Verificar conectividad

### Durante la Operación

- [ ] **Monitorear Recursos:** Uso de RAM y Flash
- [ ] **Verificar Estabilidad:** Timeouts y delays
- [ ] **Optimizar Parámetros:** Según rendimiento observado
- [ ] **Actualizar Configuración:** Según necesidades

---

**Conclusión:** El archivo config.h representa el centro de configuración del sistema Gateway, proporcionando flexibilidad, optimización y escalabilidad a través de parámetros bien definidos y documentados que permiten adaptar el sistema a diferentes entornos y requerimientos.
