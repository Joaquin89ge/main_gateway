#!/usr/bin/env python3
"""
Script para sincronizar submódulos y hacer commits coordinados
en el proyecto red_mesh.

Este script automatiza el proceso de:
1. Detectar cambios en submódulos
2. Hacer commits en submódulos
3. Hacer push a repositorios originales
4. Actualizar referencias en repositorio principal
5. Hacer commit y push del repositorio principal
"""

import os
import subprocess
import sys
import json
from datetime import datetime


class SubmoduleManager:
    def __init__(self, repo_path="."):
        self.repo_path = os.path.abspath(repo_path)
        self.submodules = {
            "main_gateway": {
                "path": "main_gateway",
                "branch": "main",
                "url": "https://github.com/Joaquin89ge/main_gateway.git"
            },
            "main_nodo": {
                "path": "main_nodo",
                "branch": "master",
                "url": "https://github.com/Joaquin89ge/main_nodo.git"
            }
        }

    def run_command(self, command, cwd=None):
        """Ejecuta un comando y retorna el resultado"""
        if cwd is None:
            cwd = self.repo_path

        try:
            result = subprocess.run(
                command,
                shell=True,
                cwd=cwd,
                capture_output=True,
                text=True,
                check=True
            )
            return result.stdout.strip()
        except subprocess.CalledProcessError as e:
            print(f"Error ejecutando comando: {command}")
            print(f"Error: {e.stderr}")
            return None

    def get_submodule_status(self):
        """Obtiene el estado actual de todos los submódulos"""
        status = {}
        for name, config in self.submodules.items():
            submodule_path = os.path.join(self.repo_path, config["path"])

            # Verificar si hay cambios en el submódulo
            git_status = self.run_command(
                "git status --porcelain", cwd=submodule_path)
            current_branch = self.run_command(
                "git branch --show-current", cwd=submodule_path)
            last_commit = self.run_command(
                "git log -1 --oneline", cwd=submodule_path)

            status[name] = {
                "has_changes": bool(git_status),
                "current_branch": current_branch,
                "last_commit": last_commit,
                "changes": git_status.split('\n') if git_status else []
            }

        return status

    def commit_submodule(self, submodule_name, commit_message=None):
        """Hace commit de cambios en un submódulo específico"""
        if submodule_name not in self.submodules:
            print(f"Error: Submódulo '{submodule_name}' no encontrado")
            return False

        config = self.submodules[submodule_name]
        submodule_path = os.path.join(self.repo_path, config["path"])

        # Verificar si hay cambios
        status = self.get_submodule_status()
        if not status[submodule_name]["has_changes"]:
            print(f"No hay cambios en {submodule_name}")
            return True

        # Generar mensaje de commit si no se proporciona
        if not commit_message:
            timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
            commit_message = f"Actualización automática - {timestamp}"

        # Hacer commit
        print(f"Haciendo commit en {submodule_name}...")
        self.run_command("git add .", cwd=submodule_path)
        self.run_command(
            f'git commit -m "{commit_message}"', cwd=submodule_path)

        # Hacer push
        print(f"Haciendo push de {submodule_name}...")
        self.run_command(
            f"git push origin {config['branch']}", cwd=submodule_path)

        return True

    def update_main_repo(self, commit_message=None):
        """Actualiza las referencias de submódulos en el repositorio principal"""
        # Generar mensaje de commit si no se proporciona
        if not commit_message:
            timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
            commit_message = f"Actualizar submódulos - {timestamp}"

        # Añadir cambios de submódulos
        for name in self.submodules.keys():
            self.run_command(f"git add {name}")

        # Hacer commit
        print("Actualizando repositorio principal...")
        self.run_command(f'git commit -m "{commit_message}"')

        # Hacer push
        print("Haciendo push del repositorio principal...")
        self.run_command("git push")

        return True

    def sync_all(self, commit_message=None):
        """Sincroniza todos los submódulos y el repositorio principal"""
        print("=== SINCRONIZACIÓN DE SUBMÓDULOS ===")

        # Verificar estado inicial
        status = self.get_submodule_status()
        print("\nEstado inicial:")
        for name, info in status.items():
            print(
                f"  {name}: {'Cambios pendientes' if info['has_changes'] else 'Sin cambios'}")

        # Hacer commits en submódulos con cambios
        for name in self.submodules.keys():
            if status[name]["has_changes"]:
                print(f"\n--- Procesando {name} ---")
                if not self.commit_submodule(name, commit_message):
                    print(f"Error al procesar {name}")
                    return False

        # Actualizar repositorio principal
        print("\n--- Actualizando repositorio principal ---")
        if not self.update_main_repo(commit_message):
            print("Error al actualizar repositorio principal")
            return False

        print("\n✅ Sincronización completada exitosamente")
        return True

    def show_status(self):
        """Muestra el estado detallado de todos los submódulos"""
        status = self.get_submodule_status()

        print("=== ESTADO DE SUBMÓDULOS ===")
        for name, info in status.items():
            print(f"\n📁 {name}:")
            print(f"  Rama: {info['current_branch']}")
            print(f"  Último commit: {info['last_commit']}")
            print(f"  Cambios: {'Sí' if info['has_changes'] else 'No'}")

            if info['changes']:
                print("  Archivos modificados:")
                for change in info['changes']:
                    print(f"    {change}")


def main():
    """Función principal del script"""
    manager = SubmoduleManager()

    if len(sys.argv) < 2:
        print("Uso: python sync_submodules.py [comando] [opciones]")
        print("\nComandos disponibles:")
        print("  status     - Mostrar estado de submódulos")
        print("  sync       - Sincronizar todos los submódulos")
        print("  commit     - Hacer commit de un submódulo específico")
        print("\nEjemplos:")
        print("  python sync_submodules.py status")
        print("  python sync_submodules.py sync")
        print("  python sync_submodules.py commit main_gateway 'Mensaje del commit'")
        return

    command = sys.argv[1]

    if command == "status":
        manager.show_status()

    elif command == "sync":
        commit_message = sys.argv[2] if len(sys.argv) > 2 else None
        manager.sync_all(commit_message)

    elif command == "commit":
        if len(sys.argv) < 3:
            print("Error: Debe especificar el nombre del submódulo")
            return

        submodule_name = sys.argv[2]
        commit_message = sys.argv[3] if len(sys.argv) > 3 else None
        manager.commit_submodule(submodule_name, commit_message)

    else:
        print(f"Comando '{command}' no reconocido")


if __name__ == "__main__":
    main()
