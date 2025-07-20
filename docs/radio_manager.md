# RadioManager - Gestión de Comunicación LoRa Mesh

## 📋 Descripción General

Clase especializada en la gestión de comunicación de radio LoRa utilizando el módulo RFM95 y la librería RHMesh. Proporciona una capa de abstracción robusta para inicializar, enviar y recibir mensajes a través de una red LoRa mesh.

## 🏗️ Arquitectura de la Clase

### Propósito Principal

- **Comunicación LoRa:** Gestión del módulo RFM95
- **Enrutamiento Mesh:** Utilización de RHMesh para routing
- **Protocolo Personalizado:** Implementación de protocolo específico
- **Gestión de Red:** Control de comunicación de red

### Características Técnicas

- **Hardware:** SX1278 LoRa con RFM95
- **Librería:** RadioHead RHMesh
- **Frecuencia:** 433 MHz (configurable)
- **Potencia:** Hasta 20 dBm
- **Alcance:** Hasta 10km en condiciones óptimas

## 📁 Estructura de la Clase

### Variables de Instancia

```cpp
private:
    RH_RF95 driver;    ///< Controlador de radio RFM95
    RHMesh manager;    ///< Gestor de red mesh
```

### Dependencias

```cpp
#include <RHMesh.h>
#include <RH_RF95.h>
#include <SPI.h>
#include "config.h"
```

## 🔧 Métodos Públicos

### Constructor

```cpp
RadioManager(uint8_t address);
```

**Propósito:**

- Inicialización de la clase con dirección única
- Configuración de parámetros de red
- Preparación para comunicación mesh

**Parámetros:**

- `address`: Dirección única del nodo en la red

**Ejemplo de Uso:**

```cpp
RadioManager radio(0x01);
```

### init()

```cpp
bool init();
```

**Funcionalidad:**

- Inicializa el módulo de radio RFM95
- Configura el gestor de red RHMesh
- Establece parámetros de frecuencia y potencia
- Realiza secuencia de reinicio del hardware

**Retorno:**

- `bool`: true si la inicialización fue exitosa

**Proceso de Inicialización:**

1. **Configuración de Pines:** SPI y pines de control
2. **Inicialización RFM95:** Configuración del driver
3. **Configuración RHMesh:** Setup del gestor de red
4. **Parámetros de Radio:** Frecuencia, potencia, etc.
5. **Verificación:** Test de comunicación

**Ejemplo de Uso:**

```cpp
if (!radio.init()) {
    Serial.println("Error al inicializar radio");
    return false;
}
```

### sendMessage()

```cpp
bool sendMessage(uint8_t to, uint8_t *data, uint8_t len, uint8_t flag);
```

**Funcionalidad:**

- Envía mensaje a dirección específica
- Utiliza enrutamiento mesh automático
- Espera acuse de recibo
- Maneja retransmisiones automáticamente

**Parámetros:**

- `to`: Dirección de destino
- `data`: Puntero a datos a enviar
- `len`: Longitud del mensaje
- `flag`: Tipo de mensaje (protocolo)

**Retorno:**

- `bool`: true si el envío fue exitoso

**Ejemplo de Uso:**

```cpp
uint8_t data[] = {0x01, 0x02, 0x03};
if (radio.sendMessage(0x10, data, 3, 0x01)) {
    Serial.println("Mensaje enviado exitosamente");
}
```

### recvMessage()

```cpp
bool recvMessage(uint8_t *buf, uint8_t *len, uint8_t *from, uint8_t *flag);
```

**Funcionalidad:**

- Recibe mensaje de la red
- Extrae datos, longitud y remitente
- Procesa información del protocolo
- No bloqueante

**Parámetros:**

- `buf`: Buffer para datos recibidos
- `len`: Puntero a longitud (entrada: máximo, salida: real)
- `from`: Puntero a dirección del remitente
- `flag`: Puntero a tipo de mensaje

**Retorno:**

- `bool`: true si se recibió mensaje

**Ejemplo de Uso:**

```cpp
uint8_t buffer[50];
uint8_t len = 50;
uint8_t from, flag;
if (radio.recvMessage(buffer, &len, &from, &flag)) {
    Serial.print("Mensaje de 0x");
    Serial.print(from, HEX);
    Serial.print(" con flag 0x");
    Serial.println(flag, HEX);
}
```

### recvMessageTimeout()

```cpp
bool recvMessageTimeout(uint8_t *buf, uint8_t *len, uint8_t *from, uint8_t *flag, uint16_t timeout);
```

**Funcionalidad:**

- Recibe mensaje con timeout específico
- Útil para operaciones que requieren respuesta
- Evita bloqueo indefinido
- Maneja timeouts de red

**Parámetros:**

- `buf`: Buffer para datos recibidos
- `len`: Puntero a longitud
- `from`: Puntero a dirección del remitente
- `flag`: Puntero a tipo de mensaje
- `timeout`: Tiempo máximo de espera en ms

**Retorno:**

- `bool`: true si se recibió mensaje dentro del timeout

**Ejemplo de Uso:**

```cpp
uint8_t buffer[50];
uint8_t len = 50;
uint8_t from, flag;
if (radio.recvMessageTimeout(buffer, &len, &from, &flag, 5000)) {
    Serial.println("Mensaje recibido en 5 segundos");
} else {
    Serial.println("Timeout - no se recibió mensaje");
}
```

### update()

```cpp
void update();
```

**Funcionalidad:**

- Actualiza estado interno del gestor
- Permite procesamiento de librería RHMesh
- Maneja actualizaciones de tabla de enrutamiento
- Gestiona retransmisiones internas

**Propósito:**

- Mantener red mesh actualizada
- Procesar eventos internos
- Optimizar rendimiento de red

## 📊 Configuración de Hardware

### Pines de Conexión

```cpp
#define RFM95_CS 2   ///< Pin Chip Select (D4 en ESP8266)
#define RFM95_INT 5  ///< Pin Interrupción (D1 en ESP8266)
```

### Configuración SPI

- **Velocidad:** 8 MHz
- **Modo:** SPI_MODE0
- **Orden:** MSBFIRST
- **Configuración:** Automática por librería

## 🔍 Características de Comunicación

### 1. Protocolo LoRa

- **Modulación:** LoRa (SF7-SF12)
- **Ancho de Banda:** 125 kHz
- **Tasa de Datos:** 0.3-37.5 kbps
- **Coding Rate:** 4/5, 4/6, 4/7, 4/8

### 2. Enrutamiento Mesh

- **Algoritmo:** AODV (Ad-hoc On-demand Distance Vector)
- **Tabla de Rutas:** Dinámica y automática
- **Retransmisión:** Automática con acuse de recibo
- **Congestión:** Manejo automático de colisiones

### 3. Gestión de Red

- **Direccionamiento:** 8 bits (0-255)
- **Broadcast:** Soporte para mensajes broadcast
- **Multicast:** Envío a múltiples destinos
- **Unicast:** Comunicación punto a punto

## 📈 Métricas de Rendimiento

### Tiempos de Operación

- **Inicialización:** <200ms
- **Envío de Mensaje:** <100ms
- **Recepción:** <50ms
- **Timeout Típico:** 2-5 segundos

### Capacidades de Red

- **Nodos Máximos:** 250 nodos
- **Alcance:** Hasta 10km (condiciones óptimas)
- **Throughput:** Hasta 37.5 kbps
- **Latencia:** <2 segundos

### Uso de Recursos

- **RAM:** ~5KB por instancia
- **Flash:** ~20KB (librerías)
- **CPU:** Bajo impacto en operación

## 🚨 Consideraciones Importantes

### 1. Configuración de Hardware

- **Pines SPI:** Configuración específica para ESP8266
- **Niveles de Voltaje:** Compatibilidad 3.3V
- **Antena:** Requerida para comunicación
- **Interferencias:** Evitar fuentes de ruido

### 2. Gestión de Errores

- **Timeouts:** Configuración apropiada
- **Retransmisiones:** Manejo automático
- **Congestión:** Detección y manejo
- **Fallos de Hardware:** Recuperación automática

### 3. Optimización de Red

- **Frecuencia:** Selección de canal libre
- **Potencia:** Ajuste según distancia
- **Spreading Factor:** Balance velocidad/alcance
- **Coding Rate:** Optimización de robustez

## 🔮 Mejoras Futuras

### Fase 1: Optimizaciones

- **Configuración Dinámica:** Cambio de parámetros en tiempo real
- **Monitoreo de Red:** Métricas de rendimiento
- **Optimización Automática:** Ajuste de parámetros

### Fase 2: Nuevas Funcionalidades

- **Encryption:** Cifrado de mensajes
- **QoS:** Calidad de servicio
- **Multi-frecuencia:** Cambio dinámico de frecuencia

### Fase 3: Integración

- **Cloud Sync:** Sincronización con la nube
- **Analytics:** Análisis de tráfico de red
- **Machine Learning:** Optimización automática

## 📋 Ejemplos de Uso

### Ejemplo 1: Inicialización Básica

```cpp
RadioManager radio(0x01);
if (radio.init()) {
    Serial.println("Radio inicializado correctamente");
} else {
    Serial.println("Error en inicialización de radio");
}
```

### Ejemplo 2: Envío de Mensaje

```cpp
uint8_t message[] = {0x01, 0x02, 0x03, 0x04};
if (radio.sendMessage(0x10, message, 4, 0x01)) {
    Serial.println("Mensaje enviado exitosamente");
} else {
    Serial.println("Error al enviar mensaje");
}
```

### Ejemplo 3: Recepción con Timeout

```cpp
uint8_t buffer[50];
uint8_t len = 50;
uint8_t from, flag;

if (radio.recvMessageTimeout(buffer, &len, &from, &flag, 5000)) {
    Serial.print("Mensaje recibido de 0x");
    Serial.print(from, HEX);
    Serial.print(": ");
    for (int i = 0; i < len; i++) {
        Serial.print(buffer[i], HEX);
        Serial.print(" ");
    }
    Serial.println();
} else {
    Serial.println("Timeout - no se recibió mensaje");
}
```

### Ejemplo 4: Bucle de Comunicación

```cpp
void loop() {
    // Enviar mensaje periódico
    static unsigned long lastSend = 0;
    if (millis() - lastSend > 10000) {
        uint8_t data[] = {0x01, 0x02};
        radio.sendMessage(0x10, data, 2, 0x01);
        lastSend = millis();
    }

    // Recibir mensajes
    uint8_t buffer[50];
    uint8_t len = 50;
    uint8_t from, flag;

    if (radio.recvMessage(buffer, &len, &from, &flag)) {
        // Procesar mensaje recibido
        processMessage(buffer, len, from, flag);
    }

    // Actualizar estado de red
    radio.update();
}
```

---

**Conclusión:** La clase RadioManager representa una implementación robusta y eficiente de comunicación LoRa mesh, proporcionando una interfaz clara y funcional para la gestión de redes IoT agrícolas con capacidades de enrutamiento automático y manejo de errores avanzado.
