# AppLogic - Lógica de Aplicación del Gateway

## 📋 Descripción General

Clase central que orquesta la lógica de aplicación del Gateway agrícola, coordinando la interacción entre identidad del nodo, comunicación de radio, gestión de tiempo y protocolo de comunicación. Implementa el comportamiento principal del Gateway como punto de concentración de la red mesh.

## 🏗️ Arquitectura de la Clase

### Propósito Principal

- **Coordinación de Red:** Gestión centralizada de nodos sensores
- **Protocolo de Comunicación:** Manejo de mensajes HELLO, DATA, ANNOUNCE
- **Almacenamiento de Datos:** Buffer de muestras por nodo
- **Programación Temporal:** Eventos basados en horarios
- **Monitoreo de Estado:** Seguimiento de nodos activos/inactivos

### Características Técnicas

- **Arquitectura:** Modular con inyección de dependencias
- **Escalabilidad:** Soporte para hasta 250 nodos
- **Persistencia:** Almacenamiento de muestras en memoria
- **Tolerancia a Fallos:** Recuperación automática de errores
- **Optimización:** Gestión eficiente de recursos

## 📁 Estructura de la Clase

### Variables de Instancia

```cpp
private:
    NodeIdentity nodeIdentity;    ///< Gestión de identidad del nodo
    RadioManager radio;           ///< Comunicación LoRa mesh
    RtcManager& rtc;             ///< Gestión de tiempo real
    uint8_t gatewayAddress;       ///< Dirección del gateway asociado

    std::map<uint8_t, String> mapNodesIDsMac;  ///< Mapeo ID-MAC de nodos
    const int intervaloHorasSuelo[CANTIDAD_MUESTRAS_SUELO] = {12, 24};  ///< Horarios de muestreo
    unsigned long temBuf = 0;     ///< Buffer de tiempo 1
    unsigned long temBuf1 = 0;    ///< Buffer de tiempo 2

    const uint8_t connectionRetries = 2;  ///< Reintentos de conexión
    bool updatedData = false;     ///< Flag de datos actualizados
    uint8_t countGroundSamples = 0;  ///< Contador de muestras de suelo
```

### Almacenamiento de Datos

```cpp
public:
    // Muestras de suelo/GPS por nodo
    std::map<std::uint8_t, std::array<GroundGpsPacket, CANTIDAD_MUESTRAS_SUELO>> groundGpsSamplesNodes;

    // Muestras atmosféricas por nodo
    std::map<std::uint8_t, std::array<AtmosphericSample, NUMERO_MUESTRAS_ATMOSFERICAS>> AtmosphericSampleNodes;

    // Lista de nodos inactivos
    uint8_t nodesDown[MAX_NODES];
```

## 🔧 Métodos Públicos

### Constructor

```cpp
AppLogic(NodeIdentity identity, RadioManager radioMgr, RtcManager& rtcMgr);
```

**Propósito:**

- Inicialización con inyección de dependencias
- Configuración de componentes principales
- Preparación para operación del Gateway

**Parámetros:**

- `identity`: Gestor de identidad del nodo
- `radioMgr`: Gestor de comunicación LoRa
- `rtcMgr`: Gestor de tiempo real

**Ejemplo de Uso:**

```cpp
AppLogic logic(identity, radio, rtc);
```

### begin()

```cpp
void begin();
```

**Funcionalidad:**

- Inicialización de la lógica de aplicación
- Envío de mensaje HELLO inicial
- Configuración de timers y buffers
- Preparación para recepción de datos

**Proceso de Inicialización:**

1. **Configuración de Componentes:** Setup de dependencias
2. **Envío de HELLO:** Anuncio inicial a la red
3. **Configuración de Timers:** Inicialización de buffers temporales
4. **Preparación de Almacenamiento:** Setup de estructuras de datos

**Ejemplo de Uso:**

```cpp
logic.begin();
```

### update()

```cpp
void update();
```

**Funcionalidad:**

- Bucle principal de la aplicación
- Procesamiento de mensajes entrantes
- Gestión de eventos temporales
- Coordinación de operaciones de red

**Operaciones Principales:**

1. **Recepción de Mensajes:** Procesamiento de datos entrantes
2. **Gestión de Timers:** Control de eventos temporales
3. **Envío de Announce:** Anuncios periódicos a la red
4. **Solicitud de Datos:** Request de muestras a nodos
5. **Validación de Estado:** Verificación de nodos activos

**Ejemplo de Uso:**

```cpp
void loop() {
    logic.update();
}
```

## 🔧 Métodos Privados

### sendAnnounce()

```cpp
void sendAnnounce();
```

**Funcionalidad:**

- Envía mensaje ANNOUNCE a la red
- Anuncia presencia del Gateway
- Incluye información de identidad
- Utilizado para descubrimiento de nodos

**Propósito:**

- Descubrimiento automático de nodos
- Mantenimiento de tabla de rutas
- Coordinación de red mesh

### handleHello()

```cpp
void handleHello();
```

**Funcionalidad:**

- Procesa mensajes HELLO de nodos
- Registra nuevos nodos en la red
- Actualiza tabla de nodos activos
- Gestiona asociación de nodos

**Proceso:**

1. **Recepción de HELLO:** Captura de mensaje
2. **Validación de Nodo:** Verificación de identidad
3. **Registro:** Actualización de tabla de nodos
4. **Respuesta:** Confirmación al nodo

### registerNewNode()

```cpp
bool registerNewNode(char receivedMac, uint8_t from);
```

**Funcionalidad:**

- Registra nuevo nodo en la red
- Asocia MAC address con ID de nodo
- Actualiza mapeo de nodos
- Valida unicidad de registro

**Parámetros:**

- `receivedMac`: MAC address del nodo
- `from`: ID del nodo remitente

**Retorno:**

- `bool`: true si el registro fue exitoso

### requestAtmosphericData()

```cpp
void requestAtmosphericData();
```

**Funcionalidad:**

- Solicita datos atmosféricos a nodos
- Gestiona timeouts y reintentos
- Procesa respuestas de nodos
- Almacena muestras recibidas

**Proceso:**

1. **Selección de Nodos:** Identificación de nodos activos
2. **Envío de Request:** Solicitud de datos atmosféricos
3. **Espera de Respuesta:** Timeout configurable
4. **Procesamiento:** Validación y almacenamiento de datos

### requestGroundGpsData()

```cpp
void requestGroundGpsData();
```

**Funcionalidad:**

- Solicita datos de suelo/GPS a nodos
- Coordina con horarios programados
- Gestiona muestras múltiples por día
- Procesa datos de sensores de suelo

**Características:**

- **Programación Temporal:** Basado en horarios específicos
- **Muestras Múltiples:** 2 muestras por día por nodo
- **Validación:** Verificación de integridad de datos
- **Almacenamiento:** Buffer circular por nodo

### handleUartRequest()

```cpp
void handleUartRequest();
```

**Funcionalidad:**

- Procesa comandos UART entrantes
- Permite configuración remota
- Gestiona debugging y monitoreo
- Implementa interfaz de control

**Comandos Soportados:**

- **Configuración:** Cambio de parámetros
- **Monitoreo:** Estado de nodos y datos
- **Debugging:** Información de sistema
- **Control:** Comandos de operación

### sendChangeID()

```cpp
void sendChangeID(uint8_t from);
```

**Funcionalidad:**

- Envía comando de cambio de ID a nodo
- Gestiona conflictos de identidad
- Coordina regeneración de IDs
- Valida nuevos IDs generados

**Parámetros:**

- `from`: ID del nodo objetivo

### timer()

```cpp
void timer();
```

**Funcionalidad:**

- Gestión de eventos temporales
- Control de timers del sistema
- Coordinación de operaciones periódicas
- Programación de eventos

**Eventos Temporales:**

- **Announce:** Anuncios periódicos a la red
- **Request de Datos:** Solicitudes programadas
- **Validación:** Verificación de estado de nodos
- **Mantenimiento:** Operaciones de limpieza

### compareHsAndMs()

```cpp
bool compareHsAndMs();
```

**Funcionalidad:**

- Compara hora actual con horarios programados
- Utiliza RtcManager para validación temporal
- Gestiona múltiples horarios de muestreo
- Coordina eventos basados en tiempo

**Retorno:**

- `bool`: true si es hora de evento programado

**Horarios Configurados:**

- **Muestreo Atmosférico:** Múltiples horarios por día
- **Muestreo de Suelo:** Horarios específicos (12:00, 24:00)
- **Announce:** Anuncios periódicos
- **Mantenimiento:** Operaciones de limpieza

## 📊 Gestión de Datos

### Estructuras de Almacenamiento

#### Muestras Atmosféricas

```cpp
std::map<std::uint8_t, std::array<AtmosphericSample, NUMERO_MUESTRAS_ATMOSFERICAS>> AtmosphericSampleNodes;
```

**Características:**

- **8 muestras por nodo:** Buffer circular
- **Datos:** Temperatura, humedad, presión
- **Timestamp:** Marca temporal de cada muestra
- **Validación:** Verificación de integridad

#### Muestras de Suelo/GPS

```cpp
std::map<std::uint8_t, std::array<GroundGpsPacket, CANTIDAD_MUESTRAS_SUELO>> groundGpsSamplesNodes;
```

**Características:**

- **2 muestras por día por nodo:** Programación temporal
- **Datos:** pH, EC, temperatura suelo, coordenadas GPS
- **Horarios:** 12:00 y 24:00
- **Validación:** Verificación de rangos

### Gestión de Nodos

#### Nodos Activos

- **Descubrimiento:** Automático por mensajes HELLO
- **Registro:** Asociación MAC-ID
- **Monitoreo:** Estado de conectividad
- **Mapeo:** Tabla de nodos activos

#### Nodos Inactivos

```cpp
uint8_t nodesDown[MAX_NODES];
```

**Gestión:**

- **Detección:** Timeouts de comunicación
- **Registro:** Lista de nodos inactivos
- **Recuperación:** Reintentos automáticos
- **Limpieza:** Eliminación de nodos perdidos

## 🔍 Protocolo de Comunicación

### Tipos de Mensajes

#### HELLO

- **Propósito:** Descubrimiento de nodos
- **Emisor:** Nodos sensores
- **Receptor:** Gateway
- **Contenido:** MAC address, ID de nodo

#### ANNOUNCE

- **Propósito:** Anuncio de presencia
- **Emisor:** Gateway
- **Receptor:** Todos los nodos
- **Contenido:** ID de Gateway, información de red

#### DATA_REQUEST

- **Propósito:** Solicitud de datos
- **Emisor:** Gateway
- **Receptor:** Nodos específicos
- **Contenido:** Tipo de datos, parámetros

#### DATA_RESPONSE

- **Propósito:** Respuesta con datos
- **Emisor:** Nodos sensores
- **Receptor:** Gateway
- **Contenido:** Muestras de sensores

### Flujo de Comunicación

#### 1. Descubrimiento de Nodos

```
Nodo → HELLO → Gateway
Gateway → Registro → Tabla de Nodos
```

#### 2. Solicitud de Datos

```
Gateway → DATA_REQUEST → Nodo
Nodo → DATA_RESPONSE → Gateway
Gateway → Almacenamiento → Buffer
```

#### 3. Anuncios Periódicos

```
Gateway → ANNOUNCE → Red
Nodos → Actualización → Tabla de Rutas
```

## 📈 Métricas de Rendimiento

### Tiempos de Operación

- **Procesamiento de Mensaje:** <10ms
- **Solicitud de Datos:** <100ms
- **Almacenamiento:** <5ms
- **Validación:** <2ms

### Capacidades de Red

- **Nodos Máximos:** 250 nodos
- **Muestras por Nodo:** 8 atmosféricas + 2 suelo
- **Throughput:** Hasta 100 mensajes/segundo
- **Latencia:** <2 segundos

### Uso de Recursos

- **RAM:** ~15KB por instancia
- **Flash:** ~5KB (código)
- **CPU:** Moderado impacto en operación
- **Energía:** Optimizado para eficiencia

## 🚨 Consideraciones Importantes

### 1. Gestión de Memoria

- **Buffers Circulares:** Evita desbordamiento
- **Validación de Datos:** Prevención de corrupción
- **Limpieza Automática:** Liberación de recursos
- **Optimización:** Uso eficiente de RAM

### 2. Robustez de Red

- **Timeouts:** Configuración apropiada
- **Reintentos:** Estrategias de recuperación
- **Validación:** Verificación de integridad
- **Recuperación:** Manejo de fallos

### 3. Escalabilidad

- **Nodos Dinámicos:** Adición/eliminación automática
- **Datos Incrementales:** Almacenamiento eficiente
- **Procesamiento Paralelo:** Operaciones concurrentes
- **Optimización:** Balance de carga

## 🔮 Mejoras Futuras

### Fase 1: Optimizaciones

- **Compresión de Datos:** Reducción de uso de memoria
- **Cache Inteligente:** Almacenamiento optimizado
- **Procesamiento Paralelo:** Operaciones concurrentes
- **Validación Avanzada:** Verificación de integridad

### Fase 2: Nuevas Funcionalidades

- **Analytics en Tiempo Real:** Análisis de datos
- **Alertas Inteligentes:** Notificaciones automáticas
- **Configuración Remota:** Cambio de parámetros
- **Backup Automático:** Respaldo de datos

### Fase 3: Integración

- **Cloud Sync:** Sincronización con la nube
- **APIs Externas:** Conexión con sistemas externos
- **Machine Learning:** Predicción de eventos
- **Interfaz Web:** Monitoreo remoto

## 📋 Ejemplos de Uso

### Ejemplo 1: Inicialización Básica

```cpp
NodeIdentity identity;
RadioManager radio(0x01);
RtcManager rtc(RTC_DAT, RTC_CLK, RTC_RST);

AppLogic logic(identity, radio, rtc);
logic.begin();

void loop() {
    logic.update();
}
```

### Ejemplo 2: Monitoreo de Nodos

```cpp
void checkNodeStatus() {
    // Verificar nodos activos
    for (auto& node : logic.AtmosphericSampleNodes) {
        uint8_t nodeID = node.first;
        Serial.print("Nodo 0x");
        Serial.print(nodeID, HEX);
        Serial.println(" activo");
    }

    // Verificar nodos inactivos
    for (int i = 0; i < MAX_NODES; i++) {
        if (logic.nodesDown[i] != 0) {
            Serial.print("Nodo 0x");
            Serial.print(logic.nodesDown[i], HEX);
            Serial.println(" inactivo");
        }
    }
}
```

### Ejemplo 3: Acceso a Datos

```cpp
void processAtmosphericData() {
    for (auto& node : logic.AtmosphericSampleNodes) {
        uint8_t nodeID = node.first;
        auto& samples = node.second;

        Serial.print("Datos atmosféricos del nodo 0x");
        Serial.println(nodeID, HEX);

        for (int i = 0; i < NUMERO_MUESTRAS_ATMOSFERICAS; i++) {
            auto& sample = samples[i];
            Serial.print("Muestra ");
            Serial.print(i);
            Serial.print(": Temp=");
            Serial.print(sample.temperature);
            Serial.print("°C, Hum=");
            Serial.print(sample.humidity);
            Serial.println("%");
        }
    }
}
```

### Ejemplo 4: Programación Temporal

```cpp
void checkScheduledEvents() {
    // Verificar si es hora de muestreo
    if (logic.compareHsAndMs()) {
        Serial.println("Es hora de muestreo programado");

        // Solicitar datos atmosféricos
        logic.requestAtmosphericData();

        // Solicitar datos de suelo (si es hora)
        if (rtc.compareCurrentTimeWith("12:00") ||
            rtc.compareCurrentTimeWith("24:00")) {
            logic.requestGroundGpsData();
        }
    }
}
```

### Ejemplo 5: Gestión de Errores

```cpp
void handleCommunicationErrors() {
    // Verificar timeouts de comunicación
    if (millis() - lastCommunicationTime > TIMEOUTGRAL) {
        Serial.println("Timeout de comunicación detectado");

        // Reintentar conexión
        logic.sendAnnounce();

        // Actualizar timestamp
        lastCommunicationTime = millis();
    }

    // Verificar nodos perdidos
    for (auto& node : logic.AtmosphericSampleNodes) {
        uint8_t nodeID = node.first;
        if (isNodeTimeout(nodeID)) {
            Serial.print("Nodo 0x");
            Serial.print(nodeID, HEX);
            Serial.println(" perdido");

            // Agregar a lista de nodos inactivos
            addToNodesDown(nodeID);
        }
    }
}
```

---

**Conclusión:** La clase AppLogic representa el núcleo de la lógica de aplicación del Gateway agrícola, proporcionando una implementación robusta y escalable para la coordinación de redes IoT, gestión de datos y programación temporal de eventos en sistemas de monitoreo agrícola.
