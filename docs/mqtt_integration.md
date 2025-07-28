# Integración MQTT en el Gateway Agrícola

## Descripción General

El Gateway Agrícola ahora incluye funcionalidad MQTT para publicar datos de sensores en tiempo real. Los datos atmosféricos y de suelo se envían automáticamente al broker MQTT después de cada recepción exitosa de datos.

## Configuración

### Configuración WiFi y MQTT

```cpp
// En src/config.h
#define WIFI_SSID "Galaxy A32 F8E2"
#define WIFI_PASSWORD "vdjn8544"
#define MQTT_SERVER "10.157.89.208"
#define MQTT_PORT 1883
#define MQTT_CLIENT_ID "esp8266_gateway"
#define MQTT_TOPIC_ATMOSPHERIC "sensor/atmospheric"
#define MQTT_TOPIC_GROUND "sensor/ground"
```

### Dependencias

```ini
# En platformio.ini
lib_deps =
    knolleary/PubSubClient@^2.8
```

## Funcionalidades Implementadas

### 1. Conexión WiFi Automática

- **Método**: `connectWiFi()`
- **Función**: Conecta automáticamente a la red WiFi configurada
- **Reintentos**: 20 intentos con delay de 500ms
- **Estado**: Se mantiene en `wifiConnected`

### 2. Conexión MQTT Automática

- **Método**: `connectMQTT()`
- **Función**: Conecta al broker MQTT configurado
- **Dependencia**: Requiere conexión WiFi previa
- **Estado**: Se mantiene en `mqttConnected`

### 3. Publicación de Datos Atmosféricos

- **Método**: `publishAtmosphericData()`
- **Tópico**: `sensor/atmospheric`
- **Formato JSON**:

```json
{
  "nodeId": 66,
  "timestamp": 1234567890,
  "temperature": 25.0,
  "humidity": 60.0,
  "pressure": 1013,
  "altitude": 100,
  "co2": 400,
  "tvoc": 50
}
```

### 4. Publicación de Datos de Suelo

- **Método**: `publishGroundData()`
- **Tópico**: `sensor/ground`
- **Formato JSON**:

```json
{
  "nodeId": 66,
  "timestamp": 1234567890,
  "temperature": 22.0,
  "moisture": 45.0,
  "n": 15,
  "p": 25,
  "k": 30,
  "ec": 1200,
  "ph": 6.5,
  "latitude": -34.56789,
  "longitude": -58.123456
}
```

## Integración Automática

### Datos Atmosféricos

Los datos se publican automáticamente en `requestAtmosphericData()` después de recibir datos exitosamente:

```cpp
// En requestAtmosphericData()
memcpy(atmosSamples.data(), buf, current_recv_len);
AtmosphericSampleNodes[nodeId] = atmosSamples;

// Publicar datos por MQTT
for (const auto& sample : atmosSamples) {
    publishAtmosphericData(nodeId, sample);
}
```

### Datos de Suelo

Los datos se publican automáticamente en `requestGroundGpsData()` después de recibir datos exitosamente:

```cpp
// En requestGroundGpsData()
nodeSamples[countGroundSamples] = receivedPacket;

// Publicar datos por MQTT
publishGroundData(nodeId, receivedPacket);
```

## Manejo de Errores

### Conexión WiFi

- **Timeout**: 10 segundos máximo
- **Reintentos**: 20 intentos
- **Fallback**: Continúa sin MQTT si falla

### Conexión MQTT

- **Dependencia**: WiFi debe estar conectado
- **Timeout**: Configurado por PubSubClient
- **Fallback**: Continúa sin publicar si falla

### Publicación de Datos

- **Verificación**: Conexión MQTT antes de publicar
- **Error**: Log de error si falla la publicación
- **Continuidad**: No interrumpe el flujo principal

## Ejemplo de Uso

```cpp
#include "app_logic.h"

// En setup()
AppLogic logic(identity, radio, rtc);
logic.begin();

// En loop()
logic.update(); // Mantiene conexiones MQTT activas
```

## Monitoreo

### Tópicos MQTT

1. **`sensor/atmospheric`**: Datos atmosféricos
2. **`sensor/ground`**: Datos de suelo y GPS

### Formato de Mensajes

Todos los mensajes incluyen:

- `nodeId`: ID del nodo sensor
- `timestamp`: Timestamp de recepción
- Datos específicos según el tipo

### Debug

Los mensajes de debug incluyen:

- Estado de conexión WiFi/MQTT
- Confirmación de publicaciones
- Errores de conexión/publicación

## Configuración Avanzada

### Personalización de Tópicos

```cpp
// En config.h
#define MQTT_TOPIC_ATMOSPHERIC "mi_granja/atmospheric"
#define MQTT_TOPIC_GROUND "mi_granja/ground"
```

### Configuración de Broker

```cpp
// En config.h
#define MQTT_SERVER "192.168.1.100"
#define MQTT_PORT 1883
#define MQTT_CLIENT_ID "gateway_agricola_001"
```

### QoS y Retención

Por defecto se usa QoS 0 (at most once). Para cambiar:

```cpp
// En publishAtmosphericData() y publishGroundData()
mqttClient.publish(topic, payload, true); // retained = true
```

## Ventajas de la Implementación

1. **Automático**: No requiere intervención manual
2. **Robusto**: Manejo de errores y reconexión
3. **Eficiente**: Solo publica cuando hay datos nuevos
4. **Escalable**: Fácil agregar nuevos tópicos
5. **Compatible**: Formato JSON estándar

## Consideraciones de Rendimiento

- **Memoria**: PubSubClient usa ~2KB de RAM
- **CPU**: Mínimo impacto en operación principal
- **Red**: Solo tráfico cuando hay datos nuevos
- **Batería**: WiFi consume más energía que LoRa

## Troubleshooting

### Problemas Comunes

1. **No se conecta a WiFi**

   - Verificar SSID y contraseña
   - Verificar señal WiFi
   - Revisar logs de debug

2. **No se conecta a MQTT**

   - Verificar IP del broker
   - Verificar puerto
   - Verificar red

3. **No se publican datos**
   - Verificar conexión MQTT
   - Verificar tópicos
   - Revisar logs de debug

### Debug

```cpp
// Habilitar debug detallado
#define DEBUG_MODE
```

## Próximas Mejoras

1. **Seguridad**: TLS/SSL para MQTT
2. **Autenticación**: Usuario/contraseña MQTT
3. **QoS**: Configuración de calidad de servicio
4. **Retención**: Mensajes persistentes
5. **Wildcards**: Suscripción a múltiples tópicos
