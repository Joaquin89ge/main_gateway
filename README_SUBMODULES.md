# 🚀 SCRIPT DE SINCRONIZACIÓN DE SUBMÓDULOS

## 📋 Descripción

Este script automatiza el proceso de sincronización de submódulos en el proyecto `red_mesh`, permitiendo hacer commits coordinados que se reflejen en todos los repositorios relacionados.

## 🎯 Funcionalidades

- ✅ **Detectar cambios** en submódulos automáticamente
- ✅ **Hacer commits** en submódulos individuales
- ✅ **Hacer push** a repositorios originales
- ✅ **Actualizar referencias** en repositorio principal
- ✅ **Sincronización completa** de todos los componentes

## 🛠️ Instalación y Uso

### Prerrequisitos

1. **Permisos de escritura** en todos los repositorios:

   - `https://github.com/Joaquin89ge/red_mesh.git`
   - `https://github.com/Joaquin89ge/main_gateway.git`
   - `https://github.com/Joaquin89ge/main_nodo.git`

2. **Python 3.6+** instalado

3. **Git configurado** con credenciales válidas

### Comandos Disponibles

#### 1. Ver Estado de Submódulos

```bash
python sync_submodules.py status
```

**Salida esperada:**

```
=== ESTADO DE SUBMÓDULOS ===

📁 main_gateway:
  Rama: main
  Último commit: abc1234 Actualizar configuración
  Cambios: No

📁 main_nodo:
  Rama: master
  Último commit: def5678 Mejorar comunicación
  Cambios: Sí
  Archivos modificados:
    M src/main.cpp
    A include/new_header.h
```

#### 2. Sincronización Completa

```bash
# Sincronización con mensaje automático
python sync_submodules.py sync

# Sincronización con mensaje personalizado
python sync_submodules.py sync "Mejoras en protocolo de comunicación"
```

**Proceso automático:**

1. Detecta cambios en submódulos
2. Hace commits en submódulos con cambios
3. Hace push a repositorios originales
4. Actualiza referencias en repositorio principal
5. Hace commit y push del repositorio principal

#### 3. Commit de Submódulo Específico

```bash
# Commit con mensaje automático
python sync_submodules.py commit main_gateway

# Commit con mensaje personalizado
python sync_submodules.py commit main_nodo "Corregir bug en envío de datos"
```

## 🔄 Flujo de Trabajo Recomendado

### Escenario 1: Desarrollo Normal

```bash
# 1. Hacer cambios en submódulos
# 2. Verificar estado
python sync_submodules.py status

# 3. Sincronizar todo
python sync_submodules.py sync "Descripción de los cambios"
```

### Escenario 2: Cambios Coordinados

```bash
# 1. Hacer cambios en múltiples submódulos
# 2. Verificar estado
python sync_submodules.py status

# 3. Hacer commits individuales si es necesario
python sync_submodules.py commit main_gateway "Actualizar protocolo"
python sync_submodules.py commit main_nodo "Adaptar a nuevo protocolo"

# 4. Sincronización final
python sync_submodules.py sync "Integración de cambios coordinados"
```

## ⚠️ Consideraciones Importantes

### Antes de Usar el Script

1. **Verificar permisos** de escritura en todos los repositorios
2. **Configurar Git** con credenciales válidas
3. **Hacer backup** de cambios importantes
4. **Revisar cambios** antes de sincronizar

### Durante la Ejecución

1. **No interrumpir** el proceso de sincronización
2. **Verificar** que no haya conflictos de merge
3. **Revisar** los mensajes de commit generados
4. **Confirmar** que los pushes fueron exitosos

### Después de la Sincronización

1. **Verificar** que los cambios aparecen en GitHub
2. **Comprobar** que las referencias están actualizadas
3. **Revisar** el historial de commits
4. **Probar** que todo funciona correctamente

## 🔧 Configuración Avanzada

### Personalizar el Script

Puedes modificar el archivo `sync_submodules.py` para:

1. **Añadir más submódulos**:

```python
self.submodules = {
    "main_gateway": {...},
    "main_nodo": {...},
    "nuevo_submodulo": {
        "path": "nuevo_submodulo",
        "branch": "main",
        "url": "https://github.com/usuario/nuevo_repo.git"
    }
}
```

2. **Cambiar ramas por defecto**:

```python
"main_gateway": {
    "branch": "develop",  # Cambiar de "main" a "develop"
    ...
}
```

3. **Añadir validaciones personalizadas**:

```python
def validate_changes(self, submodule_name):
    # Tu lógica de validación aquí
    pass
```

### Hooks de Git

Para automatizar aún más el proceso, puedes crear hooks de Git:

#### Pre-commit Hook

```bash
#!/bin/bash
# .git/hooks/pre-commit
python sync_submodules.py status
```

#### Post-commit Hook

```bash
#!/bin/bash
# .git/hooks/post-commit
python sync_submodules.py sync
```

## 🐛 Solución de Problemas

### Error: "Permission denied"

- Verificar permisos de escritura en repositorios
- Configurar credenciales de Git correctamente

### Error: "Submodule not found"

- Verificar que los submódulos están inicializados
- Ejecutar `git submodule init` si es necesario

### Error: "Push failed"

- Verificar conexión a internet
- Comprobar que no hay conflictos de merge
- Revisar que la rama destino existe

### Error: "Working tree clean"

- No hay cambios para hacer commit
- Verificar que los archivos están añadidos al staging area

## 📞 Soporte

Si encuentras problemas:

1. **Revisar logs** del script
2. **Verificar estado** de submódulos manualmente
3. **Comprobar permisos** en GitHub
4. **Revisar configuración** de Git

## 📝 Notas de Desarrollo

- El script está diseñado para ser **idempotente** (puede ejecutarse múltiples veces sin efectos secundarios)
- Los mensajes de commit automáticos incluyen **timestamps** para evitar duplicados
- El script **detecta automáticamente** si hay cambios antes de hacer commits
- Se incluyen **validaciones** para evitar errores comunes

---

**¡Listo para usar!** 🚀
