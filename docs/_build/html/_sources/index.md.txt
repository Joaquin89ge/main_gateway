# Documentación del Sistema Gateway Agrícola

## 📚 Índice de Documentación

### 📋 Descripción General

- **[README.md](README.md)** - Visión general del sistema Gateway agrícola con red mesh LoRa

### 🔧 Componentes Principales

#### Archivos de Sistema

- **[main_gateway.md](main_gateway.md)** - Punto de entrada del sistema Gateway
- **[config.md](config.md)** - Configuración central del sistema

#### Clases del Sistema

- **[node_identity.md](node_identity.md)** - Gestión de identidad única del Gateway
- **[radio_manager.md](radio_manager.md)** - Gestión de comunicación LoRa mesh
- **[rtc_manager.md](rtc_manager.md)** - Gestión de tiempo real con DS1302
- **[app_logic.md](app_logic.md)** - Lógica de aplicación del Gateway

## 🏗️ Arquitectura del Sistema

### Componentes Hardware

- **ESP8266:** Microcontrolador principal
- **SX1278 LoRa:** Módulo de comunicación de radio
- **DS1302 RTC:** Reloj de tiempo real
- **LittleFS:** Sistema de archivos para persistencia

### Componentes Software

- **NodeIdentity:** Gestión de identidad única
- **RadioManager:** Comunicación LoRa mesh
- **RtcManager:** Gestión de tiempo real
- **AppLogic:** Lógica de aplicación central

## 📊 Especificaciones Técnicas

### Hardware

- **Microcontrolador:** ESP8266 80MHz
- **Memoria RAM:** 80KB
- **Memoria Flash:** 4MB
- **Comunicación:** LoRa 433MHz
- **RTC:** DS1302 con interfaz 3-wire

### Software

- **Arquitectura:** Modular con principios SOLID
- **Comunicación:** Protocolo LoRa mesh personalizado
- **Persistencia:** LittleFS para configuración
- **Escalabilidad:** Hasta 250 nodos

## 🚀 Funcionalidades Principales

### 1. Gestión de Identidad

- Generación de ID único basado en MAC address
- Protección contra colisiones con lista negra
- Persistencia en LittleFS
- Gestión de gateway asociado

### 2. Comunicación Mesh

- Protocolo LoRa personalizado
- Enrutamiento automático con RHMesh
- Retransmisión automática
- Gestión de congestión de red

### 3. Gestión de Tiempo

- Sincronización con RTC DS1302
- Comparación de horarios para eventos
- Configuración automática de fecha/hora
- Validación de integridad temporal

### 4. Lógica de Aplicación

- Coordinación centralizada de nodos
- Almacenamiento de muestras por nodo
- Programación temporal de eventos
- Monitoreo de estado de nodos

## 📈 Métricas de Rendimiento

### Comunicación

- **Alcance:** Hasta 10km en condiciones óptimas
- **Throughput:** Hasta 37.5 kbps
- **Latencia:** <2 segundos
- **Nodos Máximos:** 250 nodos

### Recursos

- **RAM:** ~44% (36KB de 80KB)
- **Flash:** ~32% (340KB de 1MB)
- **Energía:** Optimizado para eficiencia
- **CPU:** Bajo impacto en operación

## 🔍 Características Técnicas

### 1. Arquitectura Modular

- **Single Responsibility:** Cada clase tiene una responsabilidad específica
- **Open/Closed:** Extensible sin modificar código existente
- **Liskov Substitution:** Interfaces consistentes
- **Interface Segregation:** Interfaces específicas por funcionalidad
- **Dependency Inversion:** Dependencias a través de abstracciones

### 2. Gestión de Errores

- **Validación de rangos** para todos los parámetros
- **Detección de fallos** de hardware
- **Recuperación automática** de errores
- **Logging detallado** para debugging

### 3. Optimización de Energía

- **WiFi deshabilitado** para reducir interferencias
- **Transmisión selectiva** de datos críticos
- **Gestión de buffer** eficiente
- **Monitoreo continuo** de estado de red

## 🎯 Casos de Uso

### 1. Monitoreo de Red Agrícola

- **Coordinación** de múltiples nodos sensores
- **Consolidación** de datos ambientales
- **Gestión** de muestras de suelo
- **Optimización** de comunicación mesh

### 2. Gestión de Invernaderos

- **Control centralizado** de sensores
- **Monitoreo** de condiciones críticas
- **Alertas** de problemas de red
- **Registro** histórico de datos

### 3. Agricultura de Precisión

- **Mapeo** de variabilidad del suelo
- **Coordinación** de sensores distribuidos
- **Optimización** de recursos de red
- **Análisis** de tendencias centralizadas

## 🔮 Roadmap Técnico

### Fase 1 (Completada)

- ✅ Arquitectura modular básica
- ✅ Sistema de comunicación LoRa mesh
- ✅ Gestión de identidad única
- ✅ RtcManager con DS1302

### Fase 2 (En Desarrollo)

- 🔄 Documentación completa con Sphinx
- 🔄 Optimización de consumo energético
- 🔄 Interfaz web para configuración
- 🔄 Sistema de alertas avanzado

### Fase 3 (Planificada)

- 📋 Integración con sistemas de riego
- 📋 Interfaz móvil para monitoreo
- 📋 Análisis avanzado de datos
- 📋 Integración con IA para predicciones

## 📋 Guías de Uso

### Configuración Inicial

1. **Hardware:** Conectar componentes según diagrama
2. **Software:** Compilar y subir firmware
3. **Configuración:** Ajustar parámetros en config.h
4. **Pruebas:** Verificar comunicación y funcionamiento

### Operación Diaria

1. **Monitoreo:** Verificar estado de nodos
2. **Datos:** Revisar muestras recibidas
3. **Mantenimiento:** Limpiar buffers y logs
4. **Optimización:** Ajustar parámetros según necesidades

### Troubleshooting

1. **Comunicación:** Verificar pines y antenas
2. **RTC:** Validar conexiones y batería
3. **Memoria:** Monitorear uso de RAM
4. **Energía:** Verificar consumo y estabilidad

## 🔗 Enlaces Relacionados

### Documentación Externa

- **RadioHead Library:** Documentación de RHMesh
- **ESP8266 Reference:** Guía del microcontrolador
- **DS1302 Datasheet:** Especificaciones del RTC
- **LoRa Technology:** Información sobre LoRa

### Recursos de Desarrollo

- **PlatformIO:** Entorno de desarrollo
- **Arduino IDE:** Alternativa de desarrollo
- **GitHub Repository:** Código fuente completo
- **Issue Tracker:** Reporte de problemas

## 📞 Soporte

### Contacto

- **Desarrollador:** Equipo de desarrollo agrícola
- **Email:** soporte@agro-iot.com
- **GitHub:** https://github.com/agro-iot/gateway
- **Documentación:** https://docs.agro-iot.com

### Comunidad

- **Foro:** Comunidad de usuarios
- **Wiki:** Documentación colaborativa
- **Tutoriales:** Guías paso a paso
- **Ejemplos:** Código de ejemplo

---

**Nota:** Esta documentación está diseñada para ser expandible y mejorable. Cada componente tiene su propia documentación detallada que puede ser actualizada independientemente según las necesidades del proyecto.
