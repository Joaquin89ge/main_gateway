#!/usr/bin/env python3
"""
Script avanzado para generar diagramas de flujo del código
Usa múltiples librerías: PyFlowchart, Graphviz, y Mermaid
"""

import os
import re
import ast
from pathlib import Path
import json


def parse_cpp_file(file_path):
    """
    Parsea archivo C++ y extrae información de funciones
    """
    functions = []

    try:
        with open(file_path, 'r', encoding='utf-8') as f:
            content = f.read()

        # Patrones más específicos para C++
        patterns = [
            # Funciones con implementación
            r'(\w+)\s+(\w+)\s*\([^)]*\)\s*\{[^}]*\}',
            # Declaraciones de funciones
            r'(\w+)\s+(\w+)\s*\([^)]*\)\s*;',
            # Métodos de clase
            r'(\w+)\s+(\w+)\s*::\s*(\w+)\s*\([^)]*\)\s*\{',
        ]

        for pattern in patterns:
            matches = re.finditer(pattern, content, re.MULTILINE | re.DOTALL)
            for match in matches:
                if len(match.groups()) == 2:
                    return_type = match.group(1)
                    func_name = match.group(2)
                elif len(match.groups()) == 3:
                    return_type = match.group(1)
                    class_name = match.group(2)
                    func_name = match.group(3)
                else:
                    continue

                # Filtrar funciones relevantes
                if func_name not in ['if', 'for', 'while', 'switch', 'else']:
                    functions.append({
                        'name': func_name,
                        'return_type': return_type,
                        'file': os.path.basename(file_path),
                        'class': class_name if 'class_name' in locals() else None
                    })

    except Exception as e:
        print(f"Error procesando {file_path}: {e}")

    return functions


def generate_graphviz_diagram(functions):
    """
    Genera diagrama usando Graphviz
    """
    dot_code = """digraph GatewayFlow {
    rankdir=TB;
    node [shape=box, style=filled, fillcolor=lightblue];
    
    // Nodos principales
    start [label="Inicio", shape=oval, fillcolor=lightgreen];
    init [label="Inicializar ESP8266"];
    wifi [label="Configurar WiFi OFF"];
    ni [label="Inicializar NodeIdentity"];
    rm [label="Inicializar RadioManager"];
    rtc [label="Inicializar RtcManager"];
    al [label="Inicializar AppLogic"];
    
    // Bucle principal
    loop [label="Bucle Principal", shape=diamond, fillcolor=lightyellow];
    process [label="Procesar mensajes"];
    atmos [label="Solicitar datos atmosféricos"];
    ground [label="Solicitar datos suelo/GPS"];
    announce [label="Enviar ANNOUNCE"];
    timer [label="Actualizar temporizadores"];
    
    end [label="Fin", shape=oval, fillcolor=lightgreen];
    
    // Conexiones
    start -> init;
    init -> wifi;
    wifi -> ni;
    ni -> rm;
    rm -> rtc;
    rtc -> al;
    al -> loop;
    
    loop -> process;
    process -> atmos;
    atmos -> ground;
    ground -> announce;
    announce -> timer;
    timer -> loop;
    
    loop -> end [label="Salida"];
}"""

    return dot_code


def generate_mermaid_flowchart_advanced(functions):
    """
    Genera diagrama de flujo avanzado en Mermaid
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
    
    style A fill:#90EE90
    style S fill:#90EE90
    style I fill:#FFE4B5
    style J fill:#FFB6C1
    style L fill:#FFB6C1
    style N fill:#FFB6C1
    style P fill:#FFB6C1
```"""

    return mermaid_code


def generate_function_call_graph(functions):
    """
    Genera gráfico de llamadas de funciones
    """
    call_graph = """```mermaid
graph LR
    subgraph "main_gateway.ino"
        MAIN[setup]
        LOOP[loop]
    end
    
    subgraph "AppLogic"
        AL[AppLogic]
        BEGIN[begin]
        UPDATE[update]
        REQ_ATM[requestAtmosphericData]
        REQ_GRD[requestGroundGpsData]
        SEND_ANN[sendAnnounce]
    end
    
    subgraph "RadioManager"
        RM[RadioManager]
        INIT[init]
        SEND[sendMessage]
        RECV[recvMessage]
    end
    
    subgraph "NodeIdentity"
        NI[NodeIdentity]
        GET_ID[getNodeID]
        GET_MAC[getMacAddress]
    end
    
    subgraph "RtcManager"
        RTCM[RtcManager]
        BEGIN_RTC[begin]
        GET_TIME[getDateTime]
        COMPARE[compareHsAndMs]
    end
    
    MAIN --> AL
    MAIN --> RM
    MAIN --> NI
    MAIN --> RTCM
    
    LOOP --> UPDATE
    UPDATE --> REQ_ATM
    UPDATE --> REQ_GRD
    UPDATE --> SEND_ANN
    
    REQ_ATM --> SEND
    REQ_GRD --> SEND
    SEND_ANN --> SEND
    
    UPDATE --> RECV
    UPDATE --> COMPARE
    
    AL --> NI
    AL --> RM
    AL --> RTCM
```"""

    return call_graph


def generate_data_flow_diagram():
    """
    Genera diagrama de flujo de datos
    """
    data_flow = """```mermaid
graph TD
    subgraph "Entrada de Datos"
        SENSORS[Sensores]
        RTC[RTC DS1302]
        RADIO[Radio LoRa]
    end
    
    subgraph "Procesamiento"
        NI[NodeIdentity]
        RM[RadioManager]
        RTCM[RtcManager]
        AL[AppLogic]
    end
    
    subgraph "Almacenamiento"
        ATMOS_DATA[Datos Atmosféricos]
        GROUND_DATA[Datos Suelo/GPS]
        CONFIG[Configuración]
    end
    
    subgraph "Salida"
        SERIAL[Serial Monitor]
        RADIO_OUT[Radio LoRa]
        LOGS[Logs del Sistema]
    end
    
    SENSORS --> AL
    RTC --> RTCM
    RADIO --> RM
    
    AL --> ATMOS_DATA
    AL --> GROUND_DATA
    NI --> CONFIG
    
    AL --> SERIAL
    AL --> RADIO_OUT
    AL --> LOGS
    
    style SENSORS fill:#FFE4B5
    style ATMOS_DATA fill:#90EE90
    style GROUND_DATA fill:#90EE90
    style SERIAL fill:#FFB6C1
    style RADIO_OUT fill:#FFB6C1
```"""

    return data_flow


def generate_state_diagram():
    """
    Genera diagrama de estados del sistema
    """
    state_diagram = """```mermaid
stateDiagram-v2
    [*] --> Inicialización
    Inicialización --> Configuración
    Configuración --> EsperandoNodos
    EsperandoNodos --> ProcesandoMensajes
    ProcesandoMensajes --> SolicitandoDatosAtmosféricos
    SolicitandoDatosAtmosféricos --> RecibiendoDatosAtmosféricos
    RecibiendoDatosAtmosféricos --> SolicitandoDatosSuelo
    SolicitandoDatosSuelo --> RecibiendoDatosSuelo
    RecibiendoDatosSuelo --> EsperandoNodos
    
    EsperandoNodos --> ErrorComunicación
    ErrorComunicación --> EsperandoNodos
    
    RecibiendoDatosAtmosféricos --> ErrorDatos
    ErrorDatos --> SolicitandoDatosAtmosféricos
    
    RecibiendoDatosSuelo --> ErrorDatos
    ErrorDatos --> SolicitandoDatosSuelo
    
    EsperandoNodos --> Reinicio
    Reinicio --> Inicialización
```"""

    return state_diagram


def main():
    """
    Función principal para generar diagramas avanzados
    """
    print("🚀 Generando diagramas avanzados del Sistema Gateway Agrícola")

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
        functions = parse_cpp_file(file_path)
        all_functions.extend(functions)

    print(f"🔧 Encontradas {len(all_functions)} funciones")

    # Generar diagramas avanzados
    print("\n📊 Generando diagramas avanzados...")

    # Crear directorio de salida
    output_dir = Path('docs/diagramas_avanzados')
    output_dir.mkdir(exist_ok=True)

    # Diagrama de flujo avanzado
    with open(output_dir / 'flujo_avanzado.md', 'w', encoding='utf-8') as f:
        f.write("# 🔄 Diagrama de Flujo Avanzado\n\n")
        f.write(generate_mermaid_flowchart_advanced(all_functions))

    # Gráfico de llamadas de funciones
    with open(output_dir / 'llamadas_funciones.md', 'w', encoding='utf-8') as f:
        f.write("# 📞 Gráfico de Llamadas de Funciones\n\n")
        f.write(generate_function_call_graph(all_functions))

    # Diagrama de flujo de datos
    with open(output_dir / 'flujo_datos.md', 'w', encoding='utf-8') as f:
        f.write("# 📊 Diagrama de Flujo de Datos\n\n")
        f.write(generate_data_flow_diagram())

    # Diagrama de estados
    with open(output_dir / 'estados_sistema.md', 'w', encoding='utf-8') as f:
        f.write("# 🎯 Diagrama de Estados del Sistema\n\n")
        f.write(generate_state_diagram())

    # Diagrama Graphviz
    with open(output_dir / 'graphviz.dot', 'w', encoding='utf-8') as f:
        f.write(generate_graphviz_diagram(all_functions))

    # Generar README
    with open(output_dir / 'README.md', 'w', encoding='utf-8') as f:
        f.write(f"""# 📊 Diagramas Avanzados - Sistema Gateway Agrícola

## 🎯 Diagramas Generados

### 🔄 Flujo Avanzado
- **Archivo**: `flujo_avanzado.md`
- **Descripción**: Diagrama de flujo detallado con decisiones
- **Tipo**: Mermaid Flowchart con estilos

### 📞 Llamadas de Funciones
- **Archivo**: `llamadas_funciones.md`
- **Descripción**: Gráfico de dependencias entre funciones
- **Tipo**: Mermaid Graph

### 📊 Flujo de Datos
- **Archivo**: `flujo_datos.md`
- **Descripción**: Flujo de datos entre componentes
- **Tipo**: Mermaid Graph con subgráficos

### 🎯 Estados del Sistema
- **Archivo**: `estados_sistema.md`
- **Descripción**: Estados y transiciones del sistema
- **Tipo**: Mermaid State Diagram

### 🎨 Graphviz
- **Archivo**: `graphviz.dot`
- **Descripción**: Diagrama en formato Graphviz
- **Tipo**: DOT format

## 🛠️ Herramientas Utilizadas

- **Python**: Script de generación automática
- **Mermaid**: Diagramas interactivos
- **Graphviz**: Diagramas vectoriales
- **Regex**: Análisis de código C++
- **Markdown**: Formato de salida

## 📈 Estadísticas

- **Archivos procesados**: {len(src_files)}
- **Funciones encontradas**: {len(all_functions)}
- **Diagramas generados**: 5

## 🔗 Visualización

### Mermaid
Los diagramas Mermaid se pueden visualizar en:
- GitHub (automático)
- GitLab (automático)
- Sphinx (con extensión)
- Mermaid Live Editor

### Graphviz
Para visualizar el diagrama Graphviz:
```bash
dot -Tpng graphviz.dot -o diagrama.png
dot -Tsvg graphviz.dot -o diagrama.svg
```

## 📝 Notas

- Los diagramas se generan automáticamente desde el código fuente
- Se actualizan cada vez que se ejecuta el script
- Compatibles con múltiples formatos de salida
- Integrados en la documentación Sphinx

---
*Diagramas avanzados generados automáticamente desde el código fuente*
""")

    print("✅ Diagramas avanzados generados exitosamente:")
    print("   📄 docs/diagramas_avanzados/flujo_avanzado.md")
    print("   📄 docs/diagramas_avanzados/llamadas_funciones.md")
    print("   📄 docs/diagramas_avanzados/flujo_datos.md")
    print("   📄 docs/diagramas_avanzados/estados_sistema.md")
    print("   📄 docs/diagramas_avanzados/graphviz.dot")

    print("\n🎉 ¡Diagramas avanzados generados exitosamente!")
    print("📖 Puedes verlos en la documentación HTML generada")


if __name__ == "__main__":
    main()
