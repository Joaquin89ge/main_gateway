# 📊 Diagramas de Arquitectura - Sistema Gateway Agrícola

## 🏗️ Arquitectura General del Sistema

```mermaid
graph TB
    subgraph "Hardware Layer"
        ESP8266[ESP8266 Microcontroller]
        SX1278[SX1278 LoRa Module]
        DS1302[DS1302 RTC Module]
        ANT[Antena LoRa]
    end

    subgraph "Software Layer"
        subgraph "Core Modules"
            NI[NodeIdentity]
            RM[RadioManager]
            RTCM[RtcManager]
            AL[AppLogic]
        end

        subgraph "Protocol Layer"
            PROTO[Protocol]
            MSG[Message Types]
        end

        subgraph "Data Management"
            ATMOS[Atmospheric Data]
            GROUND[Ground/GPS Data]
            LOGS[System Logs]
        end
    end

    subgraph "Network Layer"
        MESH[LoRa Mesh Network]
        NODES[Sensor Nodes]
    end

    ESP8266 --> NI
    ESP8266 --> RM
    ESP8266 --> RTCM
    ESP8266 --> AL

    SX1278 --> RM
    DS1302 --> RTCM
    ANT --> SX1278

    NI --> PROTO
    RM --> PROTO
    AL --> PROTO
    PROTO --> MSG

    AL --> ATMOS
    AL --> GROUND
    AL --> LOGS

    RM --> MESH
    MESH --> NODES
```

## 🔄 Flujo de Datos del Gateway

```mermaid
sequenceDiagram
    participant Gateway as Gateway
    participant Node1 as Sensor Node 1
    participant Node2 as Sensor Node 2
    participant RTC as RTC Module
    participant Log as System Log

    Note over Gateway: Inicialización del Sistema
    Gateway->>RTC: Inicializar RTC
    Gateway->>Log: Log: Sistema iniciado
    Gateway->>Gateway: Enviar ANNOUNCE

    loop Cada 30 segundos
        Gateway->>Gateway: Enviar ANNOUNCE
        Gateway->>Log: Log: ANNOUNCE enviado
    end

    Node1->>Gateway: HELLO
    Gateway->>Log: Log: Nodo 1 registrado
    Node2->>Gateway: HELLO
    Gateway->>Log: Log: Nodo 2 registrado

    loop Cada 5 minutos
        Gateway->>Node1: REQUEST_DATA_ATMOSPHERIC
        Node1->>Gateway: DATA_ATMOSPHERIC
        Gateway->>Log: Log: Datos atmosféricos recibidos
        Gateway->>Gateway: Almacenar datos

        Gateway->>Node2: REQUEST_DATA_ATMOSPHERIC
        Node2->>Gateway: DATA_ATMOSPHERIC
        Gateway->>Log: Log: Datos atmosféricos recibidos
        Gateway->>Gateway: Almacenar datos
    end

    loop Cada 12/24 horas
        Gateway->>Node1: REQUEST_DATA_GPS_GROUND
        Node1->>Gateway: DATA_GPS_GROUND
        Gateway->>Log: Log: Datos suelo/GPS recibidos
        Gateway->>Gateway: Almacenar datos
    end
```

## 🧩 Estructura Modular del Código

```mermaid
graph LR
    subgraph "Main Gateway"
        MAIN[main_gateway.ino]
    end

    subgraph "Core Classes"
        NI[NodeIdentity]
        RM[RadioManager]
        RTCM[RtcManager]
        AL[AppLogic]
    end

    subgraph "Support"
        CONFIG[config.h]
        PROTO[protocol.h]
    end

    MAIN --> NI
    MAIN --> RM
    MAIN --> RTCM
    MAIN --> AL

    NI --> CONFIG
    RM --> CONFIG
    RTCM --> CONFIG
    AL --> CONFIG

    AL --> PROTO
    RM --> PROTO
```

## 📡 Protocolo de Comunicación

```mermaid
stateDiagram-v2
    [*] --> Inicialización
    Inicialización --> EnviandoANNOUNCE
    EnviandoANNOUNCE --> EsperandoHELLO
    EsperandoHELLO --> RegistrandoNodos
    RegistrandoNodos --> SolicitandoDatosAtmosféricos
    SolicitandoDatosAtmosféricos --> RecibiendoDatosAtmosféricos
    RecibiendoDatosAtmosféricos --> SolicitandoDatosSuelo
    SolicitandoDatosSuelo --> RecibiendoDatosSuelo
    RecibiendoDatosSuelo --> EsperandoHELLO

    EsperandoHELLO --> ErrorComunicación
    ErrorComunicación --> EsperandoHELLO

    RecibiendoDatosAtmosféricos --> ErrorDatos
    ErrorDatos --> SolicitandoDatosAtmosféricos

    RecibiendoDatosSuelo --> ErrorDatos
    ErrorDatos --> SolicitandoDatosSuelo
```

## 🗂️ Estructura de Datos

```mermaid
classDiagram
    class AtmosphericSample {
        +float temperature
        +float humidity
        +uint32_t timestamp
    }

    class GroundGpsPacket {
        +float soilMoisture
        +float soilTemperature
        +float pH
        +float conductivity
        +float latitude
        +float longitude
        +uint32_t timestamp
    }

    class AppLogic {
        -map~uint8_t, array~ atmosphericSamples
        -map~uint8_t, array~ groundGpsSamples
        -map~uint8_t, string~ nodeMacs
        +begin()
        +update()
        -requestAtmosphericData()
        -requestGroundGpsData()
    }

    class NodeIdentity {
        -uint8_t nodeId
        -string macAddress
        +getNodeID()
        +validateKey()
    }

    class RadioManager {
        -RH_RF95 radio
        -RHMesh manager
        +init()
        +sendMessage()
        +recvMessage()
    }

    class RtcManager {
        -RtcDS1302 rtc
        +begin()
        +getDateTime()
        +setDateTime()
    }

    AppLogic --> AtmosphericSample
    AppLogic --> GroundGpsPacket
    AppLogic --> NodeIdentity
    AppLogic --> RadioManager
    AppLogic --> RtcManager
```

## 📊 Logs del Sistema

### Ejemplo de Log de Inicialización

```
[2025-01-15 08:30:15] INFO: Sistema Gateway Agrícola iniciado
[2025-01-15 08:30:15] INFO: NodeIdentity inicializado - MAC: AA:BB:CC:DD:EE:FF
[2025-01-15 08:30:16] INFO: RadioManager inicializado - Frecuencia: 915MHz
[2025-01-15 08:30:16] INFO: RTC inicializado - Fecha: 2025-01-15 08:30:16
[2025-01-15 08:30:17] INFO: AppLogic inicializado - Modo Gateway
[2025-01-15 08:30:17] INFO: ANNOUNCE enviado - Broadcast a la red
```

### Ejemplo de Log de Comunicación

```
[2025-01-15 08:30:45] INFO: HELLO recibido de nodo 0x42 - MAC: 11:22:33:44:55:66
[2025-01-15 08:30:45] INFO: Nodo registrado - ID: 0x42, MAC: 11:22:33:44:55:66
[2025-01-15 08:30:50] INFO: HELLO recibido de nodo 0x7A - MAC: AA:BB:CC:DD:EE:FF
[2025-01-15 08:30:50] INFO: Nodo registrado - ID: 0x7A, MAC: AA:BB:CC:DD:EE:FF
[2025-01-15 08:35:00] INFO: Solicitando datos atmosféricos a nodo 0x42
[2025-01-15 08:35:02] INFO: Datos atmosféricos recibidos de nodo 0x42
[2025-01-15 08:35:02] INFO: Temperatura: 25.3°C, Humedad: 65.2%
[2025-01-15 08:35:05] INFO: Solicitando datos atmosféricos a nodo 0x7A
[2025-01-15 08:35:07] INFO: Datos atmosféricos recibidos de nodo 0x7A
[2025-01-15 08:35:07] INFO: Temperatura: 24.8°C, Humedad: 68.1%
```

### Ejemplo de Log de Errores

```
[2025-01-15 08:40:00] WARN: Timeout en solicitud a nodo 0x42
[2025-01-15 08:40:00] WARN: Reintentando solicitud a nodo 0x42
[2025-01-15 08:40:03] ERROR: Nodo 0x42 no responde - Agregado a nodesDown
[2025-01-15 08:40:05] INFO: Datos atmosféricos recibidos de nodo 0x7A
[2025-01-15 08:40:05] INFO: Temperatura: 25.1°C, Humedad: 66.3%
```

### Ejemplo de Log de Datos de Suelo/GPS

```
[2025-01-15 12:00:00] INFO: Solicitando datos suelo/GPS a nodo 0x42
[2025-01-15 12:00:03] INFO: Datos suelo/GPS recibidos de nodo 0x42
[2025-01-15 12:00:03] INFO: Humedad suelo: 45.2%, Temp suelo: 18.5°C
[2025-01-15 12:00:03] INFO: pH: 6.8, Conductividad: 1250 µS/cm
[2025-01-15 12:00:03] INFO: GPS: Lat: -34.6037, Lon: -58.3816
[2025-01-15 12:00:05] INFO: Solicitando datos suelo/GPS a nodo 0x7A
[2025-01-15 12:00:08] INFO: Datos suelo/GPS recibidos de nodo 0x7A
[2025-01-15 12:00:08] INFO: Humedad suelo: 52.1%, Temp suelo: 19.2°C
[2025-01-15 12:00:08] INFO: pH: 7.1, Conductividad: 1380 µS/cm
[2025-01-15 12:00:08] INFO: GPS: Lat: -34.6042, Lon: -58.3821
```

## 🔧 Configuración de Hardware

```mermaid
graph TB
    subgraph "ESP8266 Pins"
        D1[D1 - GPIO5 - RTC_CLK]
        D2[D2 - GPIO4 - RTC_DAT]
        D3[D3 - GPIO0 - RTC_RST]
        D4[D4 - GPIO2 - RFM95_CS]
        D5[D5 - GPIO14 - RFM95_INT]
        D6[D6 - GPIO12 - RFM95_RST]
    end

    subgraph "Connections"
        ESP8266[ESP8266 NodeMCU]
        RFM95[RFM95 LoRa Module]
        RTC[DS1302 RTC Module]
        ANT[Antena LoRa]
    end

    ESP8266 -->|SPI| RFM95
    RFM95 --> ANT

    ESP8266 -->|3-Wire| RTC
```

## 📈 Métricas del Sistema

### Capacidades del Gateway

- **Nodos máximos**: 250 nodos
- **Muestras atmosféricas por nodo**: 8 muestras
- **Muestras suelo/GPS por nodo**: 2 muestras
- **Intervalo ANNOUNCE**: 5 segundos (debug) / 2 minutos (producción)
- **Intervalo datos atmosféricos**: 30 segundos (debug) / 8 minutos (producción)
- **Intervalo datos suelo/GPS**: 12 y 24 horas

### Consumo de Memoria

- **AtmosphericSample**: 6 bytes por muestra
- **GroundGpsPacket**: 24 bytes por muestra
- **Total por nodo**: 8 × 6 + 2 × 24 = 96 bytes
- **Total 250 nodos**: 24 KB de datos

---

_Diagramas generados con Mermaid para la documentación del Sistema Gateway Agrícola_
