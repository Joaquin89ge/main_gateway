# RtcManager - Gestor del Módulo RTC DS1307

## Descripción General

El `RtcManager` es una clase que maneja la comunicación con el módulo RTC DS1307 a través de I2C. Proporciona funciones para obtener fecha/hora actual y comparar intervalos de tiempo.

## Características Técnicas

### Hardware

- **RTC**: DS1307 (I2C, dirección 0x68)
- **Precisión**: ±2ppm a 0°C
- **Batería**: CR2032 (hasta 10 años)
- **Temperatura de operación**: -40°C a +85°C
- **Consumo**: <500nA en modo batería

### Comunicación

- **Protocolo**: I2C
- **Velocidad**: 100kHz (configurado para compatibilidad)
- **Dirección**: 0x68 (DS1307)

## Estructura de la Clase

```cpp
class RtcManager {
private:
    RTC_DS1307 rtc;
    bool isInitialized;
    String getTimeString(const DateTime& dt);

public:
    RtcManager();
    bool begin();
    DateTime getDateTime();
    bool setDateTime(const DateTime& dt);
    bool isDateTimeValid(const DateTime& dt);
    String getTimeString();
    bool compareHsAndMs(const String& time1, const String& time2);
    void printDateTime(const DateTime& dt);
    bool isRunning();
    bool isInitialized() const;
};
```

## Métodos Principales

### Constructor

```cpp
RtcManager::RtcManager()
```

Inicializa el gestor del RTC sin parámetros (usa I2C).

### Inicialización

```cpp
bool RtcManager::begin()
```

Inicializa el RTC DS1307:

- Configura I2C a 100kHz
- Escanea dispositivos I2C
- Verifica presencia del DS1307
- Configura fecha/hora si es necesario
- Valida funcionamiento

**Retorna**: `true` si la inicialización fue exitosa

### Obtención de Fecha/Hora

```cpp
DateTime RtcManager::getDateTime()
```

Obtiene la fecha y hora actual del RTC.

**Retorna**: Objeto `DateTime` con la fecha/hora actual

### Configuración de Fecha/Hora

```cpp
bool RtcManager::setDateTime(const DateTime& dt)
```

Establece la fecha y hora del RTC.

**Parámetros**:

- `dt`: Objeto `DateTime` con la nueva fecha/hora

**Retorna**: `true` si se estableció correctamente

### Validación de Fecha/Hora

```cpp
bool RtcManager::isDateTimeValid(const DateTime& dt)
```

Verifica si una fecha/hora es válida (año entre 2000-2100).

**Parámetros**:

- `dt`: Objeto `DateTime` a validar

**Retorna**: `true` si la fecha/hora es válida

### Obtención de Hora como String

```cpp
String RtcManager::getTimeString()
```

Obtiene la hora actual en formato "HH:MM".

**Retorna**: String con formato "HH:MM"

### Comparación de Horas

```cpp
bool RtcManager::compareHsAndMs(const String& time1, const String& time2)
```

Compara dos horas en formato "HH:MM".

**Parámetros**:

- `time1`: Primera hora (formato "HH:MM")
- `time2`: Segunda hora (formato "HH:MM")

**Retorna**: `true` si las horas son iguales

### Impresión de Fecha/Hora

```cpp
void RtcManager::printDateTime(const DateTime& dt)
```

Imprime la fecha/hora en formato legible (YYYY-MM-DD HH:MM:SS).

**Parámetros**:

- `dt`: Objeto `DateTime` a imprimir

### Verificación de Estado

```cpp
bool RtcManager::isRunning()
```

Verifica si el RTC está funcionando.

**Retorna**: `true` si el RTC está funcionando

```cpp
bool RtcManager::isInitialized() const
```

Verifica si el RTC está inicializado.

**Retorna**: `true` si el RTC está inicializado

## Ejemplo de Uso

```cpp
#include "rtc_manager.h"

RtcManager rtc;

void setup() {
    Serial.begin(115200);

    // Inicializar RTC
    if (!rtc.begin()) {
        Serial.println("Error: No se pudo inicializar el RTC");
        return;
    }

    // Obtener fecha/hora actual
    DateTime now = rtc.getDateTime();
    Serial.print("Fecha/hora actual: ");
    rtc.printDateTime(now);
    Serial.println();

    // Obtener solo la hora
    String currentTime = rtc.getTimeString();
    Serial.printf("Hora actual: %s\n", currentTime.c_str());

    // Comparar horas
    bool isEqual = rtc.compareHsAndMs("12:30", "12:30");
    Serial.printf("¿12:30 = 12:30? %s\n", isEqual ? "Sí" : "No");
}

void loop() {
    // Mostrar hora cada minuto
    static unsigned long lastTime = 0;
    if (millis() - lastTime > 60000) {
        String time = rtc.getTimeString();
        Serial.printf("Hora: %s\n", time.c_str());
        lastTime = millis();
    }

    delay(100);
}
```

## Configuración de Hardware

### Conexiones I2C

```
ESP8266    DS1307
--------   ------
D1 (GPIO5) → SCL
D2 (GPIO4) → SDA
3.3V       → VCC
GND        → GND
```

### Notas Importantes

- El DS1307 funciona a 5V, pero es compatible con 3.3V del ESP8266
- La batería CR2032 mantiene el RTC funcionando cuando no hay alimentación
- El módulo incluye resistencias pull-up para I2C

## Manejo de Errores

### Errores Comunes

1. **DS1307 no encontrado**: Verificar conexiones I2C
2. **RTC no funcionando**: Verificar batería CR2032
3. **Fecha/hora inválida**: Configurar fecha/hora inicial

### Diagnóstico

```cpp
// Verificar inicialización
if (!rtc.isInitialized()) {
    Serial.println("RTC no inicializado");
}

// Verificar funcionamiento
if (!rtc.isRunning()) {
    Serial.println("RTC no está funcionando");
}

// Verificar fecha válida
DateTime now = rtc.getDateTime();
if (!rtc.isDateTimeValid(now)) {
    Serial.println("Fecha/hora inválida");
}
```

## Optimizaciones Implementadas

### Debug Reducido

- Solo mensajes de error y confirmaciones importantes
- Sin spam de debug en funciones de comparación
- Información concisa de inicialización

### Validaciones

- Verificación de direcciones I2C
- Validación de rangos de fecha/hora
- Comprobación de formato de strings

### Compatibilidad

- Configuración I2C a 100kHz para mayor compatibilidad
- Manejo de errores robusto
- Fallbacks para casos de error

## Diferencias con DS1302

| Característica     | DS1302 (Anterior) | DS1307 (Actual) |
| ------------------ | ----------------- | --------------- |
| **Comunicación**   | 3-wire (SPI)      | I2C             |
| **Pines**          | CLK, DAT, RST     | SDA, SCL        |
| **Velocidad**      | Más lento         | Más rápido      |
| **Compatibilidad** | Menor             | Mayor           |
| **EEPROM**         | No incluida       | No usada        |
| **Precisión**      | Similar           | Similar         |

## Notas de Implementación

- Se eliminó toda funcionalidad de EEPROM para simplificar el código
- El debug se optimizó para reducir spam en pantalla
- Se mantienen solo las funciones esenciales para el proyecto
- La validación de fecha/hora es más robusta
- El manejo de errores es más claro y específico
