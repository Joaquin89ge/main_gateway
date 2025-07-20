#!/usr/bin/env python3
"""
Script para instalar dependencias para generación de diagramas
"""

import subprocess
import sys
import os


def install_package(package):
    """
    Instala un paquete usando pip
    """
    try:
        subprocess.check_call(
            [sys.executable, "-m", "pip", "install", package])
        print(f"✅ {package} instalado correctamente")
        return True
    except subprocess.CalledProcessError:
        print(f"❌ Error instalando {package}")
        return False


def main():
    """
    Instala todas las dependencias necesarias
    """
    print("🚀 Instalando dependencias para diagramas...")

    # Lista de paquetes necesarios
    packages = [
        "graphviz",           # Para diagramas Graphviz
        "pyflowchart",        # Para diagramas de flujo
        "pydot",             # Para manipulación de archivos DOT
        "matplotlib",         # Para gráficos
        "networkx",          # Para análisis de redes
        "pylint",            # Para análisis de código
        "ast",               # Para análisis de AST (incluido en Python)
    ]

    print("📦 Paquetes a instalar:")
    for package in packages:
        print(f"   - {package}")

    print("\n🔧 Instalando paquetes...")

    success_count = 0
    for package in packages:
        if install_package(package):
            success_count += 1

    print(f"\n📊 Resumen:")
    print(f"   ✅ Instalados: {success_count}/{len(packages)}")
    print(f"   ❌ Fallidos: {len(packages) - success_count}")

    if success_count == len(packages):
        print("\n🎉 ¡Todas las dependencias instaladas correctamente!")
        print("🚀 Ya puedes generar diagramas avanzados")
    else:
        print("\n⚠️  Algunas dependencias no se pudieron instalar")
        print("💡 Puedes intentar instalarlas manualmente:")
        for package in packages:
            print(f"   pip install {package}")


if __name__ == "__main__":
    main()
