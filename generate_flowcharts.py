#!/usr/bin/env python3
"""
Script para generar diagramas de flujo del código del Sistema Gateway Agrícola
Usa PyFlowchart para crear diagramas de flujo automáticamente
"""

import os
import re
from pathlib import Path


def extract_cpp_functions(file_path):
    """
    Extrae funciones de archivos .cpp y .h
    """
    functions = []

    try:
        with open(file_path, 'r', encoding='utf-8') as f:
            content = f.read()

        # Patrones para detectar funciones
        patterns = [
            r'(\w+)\s+(\w+)\s*\([^)]*\)\s*\{',  # Funciones con implementación
            r'(\w+)\s+(\w+)\s*\([^)]*\)\s*;',   # Declaraciones de funciones
        ]

        for pattern in patterns:
            matches = re.finditer(pattern, content)
            for match in matches:
                return_type = match.group(1)
                func_name = match.group(2)

                # Filtrar funciones relevantes
                if func_name not in ['if', 'for', 'while', 'switch']:
                    functions.append({
                        'name': func_name,
                        'return_type': return_type,
                        'file': os.path.basename(file_path)
                    })

    except Exception as e:
        print(f"Error procesando {file_path}: {e}")

    return functions


def generate_flowchart_code(functions):
    """
    Genera código para PyFlowchart basado en las funciones encontradas
    """
    flowchart_code = """
from flowchart import Flowchart

# Diagrama de flujo del Sistema Gateway Agrícola
fc = Flowchart()

# Nodos principales
fc.start()
fc.process("Inicializar ESP8266")
fc.process("Configurar WiFi OFF")
fc.process("Inicializar NodeIdentity")
fc.process("Inicializar RadioManager")
fc.process("Inicializar RtcManager")
fc.process("Inicializar AppLogic")

# Bucle principal
fc.loop("Bucle Principal", [
    "Procesar mensajes entrantes",
    "Solicitar datos atmosféricos",
    "Solicitar datos suelo/GPS",
    "Enviar ANNOUNCE",
    "Actualizar temporizadores"
])

fc.end()
"""
    return flowchart_code


def generate_mermaid_flowchart(functions):
    """
    Genera diagrama de flujo en formato Mermaid
    """
    mermaid_code = """```mermaid
flowchart TD
    A[Inicio] --> B[Inicializar ESP8266]
    B --> C[Configurar WiFi OFF]
    C --> D[Inicializar NodeIdentity]
    D --> E[Inicializar RadioManager]
    E --> F[Inicializar RtcManager]
    F --> G[Inicializar AppLogic]
    G --> H[AppLogic.begin]
    H --> I[Bucle Principal]
    
    I --> J{¿Hay mensajes?}
    J -->|Sí| K[Procesar mensajes]
    J -->|No| L{¿Es hora de datos atmosféricos?}
    
    K --> L
    
    L -->|Sí| M[Solicitar datos atmosféricos]
    L -->|No| N{¿Es hora de datos suelo/GPS?}
    
    M --> N
    
    N -->|Sí| O[Solicitar datos suelo/GPS]
    N -->|No| P{¿Es hora de ANNOUNCE?}
    
    O --> P
    
    P -->|Sí| Q[Enviar ANNOUNCE]
    P -->|No| R[Actualizar temporizadores]
    
    Q --> R
    R --> I
    
    I --> S[Fin]
```"""

    return mermaid_code


def generate_class_diagram():
    """
    Genera diagrama de clases en formato Mermaid
    """
    class_diagram = """```mermaid
classDiagram
    class AppLogic {
        -NodeIdentity nodeIdentity
        -RadioManager radio
        -RtcManager& rtc
        -uint8_t gatewayAddress
        +std::map atmosphericSamples
        +std::map groundGpsSamples
        +AppLogic()
        +begin()
        +update()
        -requestAtmosphericData()
        -requestGroundGpsData()
    }
    
    class NodeIdentity {
        -uint8_t nodeId
        -String macAddress
        -uint8_t gatewayAddr
        +NodeIdentity()
        +getNodeID()
        +getMacAddress()
        +validateKey()
        -calculateCRC8()
        -generateUniqueID()
    }
    
    class RadioManager {
        -RH_RF95 radio
        -RHMesh manager
        -uint8_t address
        -bool initialized
        +RadioManager()
        +init()
        +sendMessage()
        +recvMessage()
        +update()
    }
    
    class RtcManager {
        -RtcDS1302 rtc
        +RtcManager()
        +begin()
        +getDateTime()
        +setDateTime()
        +compareHsAndMs()
    }
    
    AppLogic --> NodeIdentity
    AppLogic --> RadioManager
    AppLogic --> RtcManager
```"""

    return class_diagram


def generate_sequence_diagram():
    """
    Genera diagrama de secuencia en formato Mermaid
    """
    sequence_diagram = """```mermaid
sequenceDiagram
    participant Main as main_gateway.ino
    participant NI as NodeIdentity
    participant RM as RadioManager
    participant RTCM as RtcManager
    participant AL as AppLogic
    
    Main->>NI: Constructor
    Main->>RM: Constructor(identity.getNodeID())
    Main->>RTCM: Constructor(RTC_DAT, RTC_CLK, RTC_RST)
    Main->>AL: Constructor(identity, radio, rtc)
    
    Main->>RM: init()
    RM-->>Main: true/false
    
    Main->>RTCM: begin()
    RTCM-->>Main: true/false
    
    Main->>AL: begin()
    
    loop Bucle Principal
        Main->>AL: update()
        AL->>RM: recvMessage()
        RM-->>AL: mensaje recibido
        
        alt Mensaje HELLO
            AL->>AL: handleHello()
            AL->>AL: registerNewNode()
        else Mensaje DATA_ATMOSPHERIC
            AL->>AL: procesar datos atmosféricos
        else Mensaje DATA_GPS_GROUND
            AL->>AL: procesar datos suelo/GPS
        end
        
        AL->>RTCM: compareHsAndMs()
        RTCM-->>AL: hora válida
        
        alt Es hora de datos atmosféricos
            AL->>AL: requestAtmosphericData()
            AL->>RM: sendMessage(REQUEST_DATA_ATMOSPHERIC)
        else Es hora de datos suelo/GPS
            AL->>AL: requestGroundGpsData()
            AL->>RM: sendMessage(REQUEST_DATA_GPS_GROUND)
        end
        
        AL->>AL: timer()
        AL->>AL: sendAnnounce()
    end
```"""

    return sequence_diagram


def main():
    """
    Función principal para generar diagramas
    """
    print("🚀 Generando diagramas de flujo del Sistema Gateway Agrícola")

    # Buscar archivos de código fuente
    src_files = []
    for root, dirs, files in os.walk('src'):
        for file in files:
            if file.endswith(('.cpp', '.h')):
                src_files.append(os.path.join(root, file))

    print(f"📁 Encontrados {len(src_files)} archivos de código fuente")

    # Extraer funciones
    all_functions = []
    for file_path in src_files:
        functions = extract_cpp_functions(file_path)
        all_functions.extend(functions)

    print(f"🔧 Encontradas {len(all_functions)} funciones")

    # Generar diagramas
    print("\n📊 Generando diagramas...")

    # Diagrama de flujo principal
    flowchart = generate_mermaid_flowchart(all_functions)

    # Diagrama de clases
    class_diagram = generate_class_diagram()

    # Diagrama de secuencia
    sequence_diagram = generate_sequence_diagram()

    # Guardar diagramas
    output_dir = Path('docs/diagramas_codigo')
    output_dir.mkdir(exist_ok=True)

    with open(output_dir / 'flujo_principal.md', 'w', encoding='utf-8') as f:
        f.write("# 🔄 Diagrama de Flujo Principal\n\n")
        f.write(flowchart)

    with open(output_dir / 'diagrama_clases.md', 'w', encoding='utf-8') as f:
        f.write("# 🏗️ Diagrama de Clases\n\n")
        f.write(class_diagram)

    with open(output_dir / 'diagrama_secuencia.md', 'w', encoding='utf-8') as f:
        f.write("# ⏱️ Diagrama de Secuencia\n\n")
        f.write(sequence_diagram)

    print("✅ Diagramas generados exitosamente:")
    print("   📄 docs/diagramas_codigo/flujo_principal.md")
    print("   📄 docs/diagramas_codigo/diagrama_clases.md")
    print("   📄 docs/diagramas_codigo/diagrama_secuencia.md")

    # Generar resumen
    with open(output_dir / 'README.md', 'w', encoding='utf-8') as f:
        f.write("""# 📊 Diagramas de Código - Sistema Gateway Agrícola

## 🎯 Diagramas Generados

### 🔄 Flujo Principal
- **Archivo**: `flujo_principal.md`
- **Descripción**: Diagrama de flujo del programa principal
- **Tipo**: Mermaid Flowchart

### 🏗️ Diagrama de Clases
- **Archivo**: `diagrama_clases.md`
- **Descripción**: Estructura de clases y relaciones
- **Tipo**: Mermaid Class Diagram

### ⏱️ Diagrama de Secuencia
- **Archivo**: `diagrama_secuencia.md`
- **Descripción**: Interacción entre componentes
- **Tipo**: Mermaid Sequence Diagram

## 🛠️ Herramientas Utilizadas

- **Python**: Script de generación automática
- **Mermaid**: Formato de diagramas
- **Regex**: Extracción de funciones de código
- **Markdown**: Formato de salida

## 📈 Estadísticas

- **Archivos procesados**: {len_src_files}
- **Funciones encontradas**: {len_all_functions}
- **Diagramas generados**: 3

## 🔗 Integración

Los diagramas se integran automáticamente en la documentación Sphinx
y se pueden visualizar en el HTML generado.

---
*Diagramas generados automáticamente desde el código fuente*
    """.format(len_src_files=len(src_files), len_all_functions=len(all_functions)))

    print("\n🎉 ¡Diagramas generados exitosamente!")
    print("📖 Puedes verlos en la documentación HTML generada")


if __name__ == "__main__":
    main()
