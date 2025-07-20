# 📋 Logs de Ejemplo - Sistema Gateway Agrícola

## 🔍 Logs de Inicialización del Sistema

```
[2025-01-15 08:30:15] ========================================
[2025-01-15 08:30:15] SISTEMA GATEWAY AGRÍCOLA INICIANDO
[2025-01-15 08:30:15] ========================================
[2025-01-15 08:30:15] INFO: Configuración de hardware detectada
[2025-01-15 08:30:15] INFO: ESP8266 - Frecuencia CPU: 80MHz
[2025-01-15 08:30:15] INFO: Memoria disponible: 32KB
[2025-01-15 08:30:15] INFO: LittleFS inicializado correctamente
[2025-01-15 08:30:16] INFO: NodeIdentity inicializado
[2025-01-15 08:30:16] INFO: MAC Address: AA:BB:CC:DD:EE:FF
[2025-01-15 08:30:16] INFO: Node ID generado: 0x42
[2025-01-15 08:30:16] INFO: RadioManager inicializando...
[2025-01-15 08:30:16] INFO: RFM95 detectado en SPI
[2025-01-15 08:30:16] INFO: Frecuencia configurada: 915MHz
[2025-01-15 08:30:16] INFO: Potencia TX: 23dBm
[2025-01-15 08:30:16] INFO: RHMesh inicializado - Address: 0x42
[2025-01-15 08:30:17] INFO: RTC inicializando...
[2025-01-15 08:30:17] INFO: DS1302 detectado en pines 4,5,0
[2025-01-15 08:30:17] INFO: RTC sincronizado - 2025-01-15 08:30:17
[2025-01-15 08:30:17] INFO: AppLogic inicializado - Modo Gateway
[2025-01-15 08:30:17] INFO: Sistema listo para operación
[2025-01-15 08:30:17] INFO: Enviando ANNOUNCE inicial...
[2025-01-15 08:30:17] INFO: ANNOUNCE enviado exitosamente
[2025-01-15 08:30:17] ========================================
[2025-01-15 08:30:17] SISTEMA INICIADO CORRECTAMENTE
[2025-01-15 08:30:17] ========================================
```

## 📡 Logs de Comunicación con Nodos

### Registro de Nuevos Nodos

```
[2025-01-15 08:30:45] INFO: HELLO recibido de nodo 0x7A
[2025-01-15 08:30:45] INFO: MAC del nodo: 11:22:33:44:55:66
[2025-01-15 08:30:45] INFO: Validando clave compartida...
[2025-01-15 08:30:45] INFO: Clave válida - Nodo autenticado
[2025-01-15 08:30:45] INFO: Nodo 0x7A registrado exitosamente
[2025-01-15 08:30:45] INFO: Total de nodos registrados: 1

[2025-01-15 08:31:12] INFO: HELLO recibido de nodo 0x3B
[2025-01-15 08:31:12] INFO: MAC del nodo: AA:BB:CC:DD:EE:FF
[2025-01-15 08:31:12] INFO: Validando clave compartida...
[2025-01-15 08:31:12] INFO: Clave válida - Nodo autenticado
[2025-01-15 08:31:12] INFO: Nodo 0x3B registrado exitosamente
[2025-01-15 08:31:12] INFO: Total de nodos registrados: 2

[2025-01-15 08:31:45] INFO: HELLO recibido de nodo 0x9C
[2025-01-15 08:31:45] INFO: MAC del nodo: 33:44:55:66:77:88
[2025-01-15 08:31:45] INFO: Validando clave compartida...
[2025-01-15 08:31:45] WARN: Clave inválida - Nodo rechazado
[2025-01-15 08:31:45] WARN: Nodo 0x9C no autenticado
```

### Solicitud de Datos Atmosféricos

```
[2025-01-15 08:35:00] INFO: Iniciando ciclo de solicitud de datos atmosféricos
[2025-01-15 08:35:00] INFO: Procesando nodo 0x7A...
[2025-01-15 08:35:00] INFO: Enviando REQUEST_DATA_ATMOSPHERIC a nodo 0x7A
[2025-01-15 08:35:02] INFO: Respuesta recibida de nodo 0x7A
[2025-01-15 08:35:02] INFO: Tamaño de datos: 48 bytes (8 muestras)
[2025-01-15 08:35:02] INFO: Datos atmosféricos almacenados:
[2025-01-15 08:35:02] INFO:   Muestra 1: Temp=25.3°C, Hum=65.2%
[2025-01-15 08:35:02] INFO:   Muestra 2: Temp=25.1°C, Hum=65.8%
[2025-01-15 08:35:02] INFO:   Muestra 3: Temp=24.9°C, Hum=66.1%
[2025-01-15 08:35:02] INFO:   Muestra 4: Temp=25.2°C, Hum=65.5%
[2025-01-15 08:35:02] INFO:   Muestra 5: Temp=25.0°C, Hum=65.9%
[2025-01-15 08:35:02] INFO:   Muestra 6: Temp=24.8°C, Hum=66.3%
[2025-01-15 08:35:02] INFO:   Muestra 7: Temp=25.1°C, Hum=65.7%
[2025-01-15 08:35:02] INFO:   Muestra 8: Temp=25.4°C, Hum=65.1%

[2025-01-15 08:35:05] INFO: Procesando nodo 0x3B...
[2025-01-15 08:35:05] INFO: Enviando REQUEST_DATA_ATMOSPHERIC a nodo 0x3B
[2025-01-15 08:35:07] INFO: Respuesta recibida de nodo 0x3B
[2025-01-15 08:35:07] INFO: Tamaño de datos: 48 bytes (8 muestras)
[2025-01-15 08:35:07] INFO: Datos atmosféricos almacenados:
[2025-01-15 08:35:07] INFO:   Muestra 1: Temp=24.8°C, Hum=68.1%
[2025-01-15 08:35:07] INFO:   Muestra 2: Temp=24.6°C, Hum=68.5%
[2025-01-15 08:35:07] INFO:   Muestra 3: Temp=24.9°C, Hum=67.8%
[2025-01-15 08:35:07] INFO:   Muestra 4: Temp=24.7°C, Hum=68.2%
[2025-01-15 08:35:07] INFO:   Muestra 5: Temp=24.5°C, Hum=68.7%
[2025-01-15 08:35:07] INFO:   Muestra 6: Temp=24.8°C, Hum=68.0%
[2025-01-15 08:35:07] INFO:   Muestra 7: Temp=24.6°C, Hum=68.4%
[2025-01-15 08:35:07] INFO:   Muestra 8: Temp=24.9°C, Hum=67.9%
```

### Solicitud de Datos de Suelo/GPS

```
[2025-01-15 12:00:00] INFO: Iniciando ciclo de solicitud de datos suelo/GPS
[2025-01-15 12:00:00] INFO: Verificando horario para solicitud suelo/GPS...
[2025-01-15 12:00:00] INFO: Horario válido - 12:00:00
[2025-01-15 12:00:00] INFO: Procesando nodo 0x7A...
[2025-01-15 12:00:00] INFO: Enviando REQUEST_DATA_GPS_GROUND a nodo 0x7A
[2025-01-15 12:00:03] INFO: Respuesta recibida de nodo 0x7A
[2025-01-15 12:00:03] INFO: Tamaño de datos: 48 bytes (2 muestras)
[2025-01-15 12:00:03] INFO: Datos suelo/GPS almacenados:
[2025-01-15 12:00:03] INFO:   Muestra 1:
[2025-01-15 12:00:03] INFO:     Humedad suelo: 45.2%
[2025-01-15 12:00:03] INFO:     Temperatura suelo: 18.5°C
[2025-01-15 12:00:03] INFO:     pH: 6.8
[2025-01-15 12:00:03] INFO:     Conductividad: 1250 µS/cm
[2025-01-15 12:00:03] INFO:     GPS: Lat=-34.6037, Lon=-58.3816
[2025-01-15 12:00:03] INFO:   Muestra 2:
[2025-01-15 12:00:03] INFO:     Humedad suelo: 44.8%
[2025-01-15 12:00:03] INFO:     Temperatura suelo: 18.3°C
[2025-01-15 12:00:03] INFO:     pH: 6.9
[2025-01-15 12:00:03] INFO:     Conductividad: 1280 µS/cm
[2025-01-15 12:00:03] INFO:     GPS: Lat=-34.6038, Lon=-58.3817

[2025-01-15 12:00:05] INFO: Procesando nodo 0x3B...
[2025-01-15 12:00:05] INFO: Enviando REQUEST_DATA_GPS_GROUND a nodo 0x3B
[2025-01-15 12:00:08] INFO: Respuesta recibida de nodo 0x3B
[2025-01-15 12:00:08] INFO: Tamaño de datos: 48 bytes (2 muestras)
[2025-01-15 12:00:08] INFO: Datos suelo/GPS almacenados:
[2025-01-15 12:00:08] INFO:   Muestra 1:
[2025-01-15 12:00:08] INFO:     Humedad suelo: 52.1%
[2025-01-15 12:00:08] INFO:     Temperatura suelo: 19.2°C
[2025-01-15 12:00:08] INFO:     pH: 7.1
[2025-01-15 12:00:08] INFO:     Conductividad: 1380 µS/cm
[2025-01-15 12:00:08] INFO:     GPS: Lat=-34.6042, Lon=-58.3821
[2025-01-15 12:00:08] INFO:   Muestra 2:
[2025-01-15 12:00:08] INFO:     Humedad suelo: 51.8%
[2025-01-15 12:00:08] INFO:     Temperatura suelo: 19.0°C
[2025-01-15 12:00:08] INFO:     pH: 7.2
[2025-01-15 12:00:08] INFO:     Conductividad: 1410 µS/cm
[2025-01-15 12:00:08] INFO:     GPS: Lat=-34.6043, Lon=-58.3822
```

## ⚠️ Logs de Errores y Advertencias

### Timeout en Comunicación

```
[2025-01-15 08:40:00] WARN: Timeout en solicitud a nodo 0x7A
[2025-01-15 08:40:00] WARN: Reintentando solicitud (intento 1/2)
[2025-01-15 08:40:00] INFO: Enviando REQUEST_DATA_ATMOSPHERIC a nodo 0x7A
[2025-01-15 08:40:03] WARN: Timeout en reintento a nodo 0x7A
[2025-01-15 08:40:03] ERROR: Nodo 0x7A no responde después de 2 intentos
[2025-01-15 08:40:03] ERROR: Agregando nodo 0x7A a lista nodesDown
[2025-01-15 08:40:03] INFO: Continuando con siguiente nodo...

[2025-01-15 08:40:05] INFO: Procesando nodo 0x3B...
[2025-01-15 08:40:05] INFO: Enviando REQUEST_DATA_ATMOSPHERIC a nodo 0x3B
[2025-01-15 08:40:07] INFO: Respuesta recibida de nodo 0x3B
[2025-01-15 08:40:07] INFO: Datos atmosféricos recibidos exitosamente
```

### Error en Datos Recibidos

```
[2025-01-15 08:45:00] INFO: Enviando REQUEST_DATA_ATMOSPHERIC a nodo 0x7A
[2025-01-15 08:45:02] INFO: Respuesta recibida de nodo 0x7A
[2025-01-15 08:45:02] WARN: Tamaño de datos incorrecto
[2025-01-15 08:45:02] WARN: Esperado: 48 bytes, Recibido: 24 bytes
[2025-01-15 08:45:02] WARN: Datos incompletos - descartando paquete
[2025-01-15 08:45:02] WARN: Reintentando solicitud a nodo 0x7A
[2025-01-15 08:45:05] INFO: Respuesta recibida de nodo 0x7A
[2025-01-15 08:45:05] INFO: Tamaño de datos correcto: 48 bytes
[2025-01-15 08:45:05] INFO: Datos atmosféricos procesados exitosamente
```

### Error de RTC

```
[2025-01-15 09:00:00] WARN: Error al leer RTC
[2025-01-15 09:00:00] WARN: RTC no responde - usando tiempo del sistema
[2025-01-15 09:00:00] INFO: Tiempo actual: 09:00:00 (sistema)
[2025-01-15 09:00:00] INFO: Continuando operación normal
[2025-01-15 09:00:05] INFO: RTC recuperado - sincronizando tiempo
[2025-01-15 09:00:05] INFO: Tiempo RTC: 09:00:05
[2025-01-15 09:00:05] INFO: Sistema sincronizado correctamente
```

## 📊 Logs de Estado del Sistema

### Estado de Memoria

```
[2025-01-15 10:00:00] INFO: ===== ESTADO DEL SISTEMA =====
[2025-01-15 10:00:00] INFO: Memoria libre: 15.2KB
[2025-01-15 10:00:00] INFO: Memoria utilizada: 16.8KB
[2025-01-15 10:00:00] INFO: Nodos registrados: 2
[2025-01-15 10:00:00] INFO: Nodos activos: 2
[2025-01-15 10:00:00] INFO: Nodos inactivos: 0
[2025-01-15 10:00:00] INFO: Muestras atmosféricas almacenadas: 16
[2025-01-15 10:00:00] INFO: Muestras suelo/GPS almacenadas: 4
[2025-01-15 10:00:00] INFO: Tiempo de funcionamiento: 1h 30m
[2025-01-15 10:00:00] INFO: ANNOUNCE enviados: 180
[2025-01-15 10:00:00] INFO: Solicitudes exitosas: 24
[2025-01-15 10:00:00] INFO: Solicitudes fallidas: 2
[2025-01-15 10:00:00] INFO: Tasa de éxito: 92.3%
```

### Resumen Diario

```
[2025-01-15 23:59:59] INFO: ===== RESUMEN DIARIO =====
[2025-01-15 23:59:59] INFO: Fecha: 2025-01-15
[2025-01-15 23:59:59] INFO: Tiempo total de funcionamiento: 15h 29m
[2025-01-15 23:59:59] INFO: Nodos únicos conectados: 3
[2025-01-15 23:59:59] INFO: Total de muestras atmosféricas: 288
[2025-01-15 23:59:59] INFO: Total de muestras suelo/GPS: 6
[2025-01-15 23:59:59] INFO: Promedio temperatura: 25.1°C
[2025-01-15 23:59:59] INFO: Promedio humedad: 66.2%
[2025-01-15 23:59:59] INFO: Promedio humedad suelo: 48.7%
[2025-01-15 23:59:59] INFO: Promedio pH: 7.0
[2025-01-15 23:59:59] INFO: Promedio conductividad: 1315 µS/cm
[2025-01-15 23:59:59] INFO: ANNOUNCE enviados: 1,860
[2025-01-15 23:59:59] INFO: Solicitudes exitosas: 312
[2025-01-15 23:59:59] INFO: Solicitudes fallidas: 8
[2025-01-15 23:59:59] INFO: Tasa de éxito general: 97.5%
[2025-01-15 23:59:59] INFO: ================================
```

## 🔧 Logs de Debug

### Debug de Comparación de Tiempo

```
[2025-01-15 12:00:00] DEBUG: compareHsAndMs() - Verificando horario
[2025-01-15 12:00:00] DEBUG: Hora actual: 12:00:00
[2025-01-15 12:00:00] DEBUG: Horarios configurados: [12, 24]
[2025-01-15 12:00:00] DEBUG: Coincidencia encontrada: 12:00
[2025-01-15 12:00:00] DEBUG: Solicitud suelo/GPS autorizada
```

### Debug de Almacenamiento de Datos

```
[2025-01-15 08:35:02] DEBUG: Almacenando datos atmosféricos
[2025-01-15 08:35:02] DEBUG: Nodo ID: 0x7A
[2025-01-15 08:35:02] DEBUG: Posición en array: 0
[2025-01-15 08:35:02] DEBUG: Tamaño de muestra: 6 bytes
[2025-01-15 08:35:02] DEBUG: Datos copiados exitosamente
[2025-01-15 08:35:02] DEBUG: Mapa actualizado correctamente
```

---

_Estos logs son ejemplos representativos del funcionamiento del Sistema Gateway Agrícola. Los logs reales pueden variar según la configuración específica y las condiciones de operación._
