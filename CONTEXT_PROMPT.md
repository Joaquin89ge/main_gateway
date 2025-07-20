# CONTEXTO DEL PROYECTO - RED_MESH CON SUBMÓDULOS

## 🎯 OBJETIVO

Configurar y trabajar con un repositorio principal `red_mesh` que contiene dos submódulos de Git, permitiendo hacer commits desde el repositorio principal que se reflejen automáticamente en todos los repositorios relacionados.

## 📁 ESTRUCTURA ACTUAL DEL PROYECTO

```
red_mesh/ (repositorio principal)
├── .git/
├── .gitmodules
├── main_gateway/ (submódulo)
│   ├── .git/
│   ├── src/
│   ├── include/
│   ├── lib/
│   ├── test/
│   └── platformio.ini
├── main_nodo/ (submódulo)
│   ├── .git/
│   ├── src/
│   ├── include/
│   ├── lib/
│   └── platformio.ini
└── README.md
```

## 🔗 REPOSITORIOS INVOLUCRADOS

### Repositorio Principal

- **URL**: https://github.com/Joaquin89ge/red_mesh.git
- **Propósito**: Proyecto principal que coordina la red mesh
- **Permisos necesarios**: WRITE (para hacer commits y push)

### Submódulo 1: main_gateway

- **URL**: https://github.com/Joaquin89ge/main_gateway.git
- **Propósito**: Código del gateway de la red mesh
- **Permisos necesarios**: WRITE (para hacer commits y push)
- **Rama actual**: main

### Submódulo 2: main_nodo

- **URL**: https://github.com/Joaquin89ge/main_nodo.git
- **Propósito**: Código de los nodos de la red mesh
- **Permisos necesarios**: WRITE (para hacer commits y push)
- **Rama actual**: master

## 🔐 PERMISOS REQUERIDOS

### Para el Agente/Usuario que trabajará en este proyecto:

1. **Permisos de Escritura en red_mesh**:

   - Poder hacer commits
   - Poder hacer push al repositorio principal
   - Poder actualizar referencias de submódulos

2. **Permisos de Escritura en main_gateway**:

   - Poder hacer commits dentro del submódulo
   - Poder hacer push al repositorio original
   - Poder cambiar entre ramas

3. **Permisos de Escritura en main_nodo**:
   - Poder hacer commits dentro del submódulo
   - Poder hacer push al repositorio original
   - Poder cambiar entre ramas

## 🛠️ CONFIGURACIÓN ACTUAL

### Archivo .gitmodules

```ini
[submodule "main_gateway"]
	path = main_gateway
	url = https://github.com/Joaquin89ge/main_gateway.git
[submodule "main_nodo"]
	path = main_nodo
	url = https://github.com/Joaquin89ge/main_nodo.git
```

### Estado de los submódulos

- **main_gateway**: Limpio, sincronizado con origin/main
- **main_nodo**: Limpio, sincronizado con origin/master

## 🎯 FLUJO DE TRABAJO DESEADO

### Escenario 1: Cambios en submódulos individuales

1. Trabajar dentro del submódulo específico
2. Hacer commit en el submódulo
3. Hacer push al repositorio original del submódulo
4. Actualizar referencia en el repositorio principal
5. Hacer commit y push del repositorio principal

### Escenario 2: Cambios coordinados entre submódulos

1. Hacer cambios en múltiples submódulos
2. Hacer commits en cada submódulo
3. Hacer push de cada submódulo a su repositorio original
4. Actualizar todas las referencias en el repositorio principal
5. Hacer commit y push del repositorio principal

## 📋 COMANDOS ÚTILES PARA EL AGENTE

### Verificar estado

```bash
git submodule status
git submodule foreach git status
```

### Trabajar en submódulos

```bash
# Entrar a un submódulo
cd main_gateway
git add .
git commit -m "Descripción del cambio"
git push origin main

# Volver al repositorio principal
cd ..
git add main_gateway
git commit -m "Actualizar submódulo main_gateway"
git push
```

### Actualizar submódulos

```bash
git submodule update --remote
git submodule foreach git pull origin main
```

## ⚠️ CONSIDERACIONES IMPORTANTES

1. **Cada submódulo es un repositorio Git independiente**
2. **Los cambios locales NO se sincronizan automáticamente**
3. **Se requieren permisos de escritura en todos los repositorios**
4. **Los commits deben hacerse en el orden correcto**
5. **Las referencias de submódulos deben actualizarse manualmente**

## 🎯 OBJETIVO FINAL

Crear un sistema donde:

- Los cambios se puedan hacer desde el repositorio principal
- Los commits se reflejen automáticamente en todos los repositorios relacionados
- Se mantenga la sincronización entre todos los componentes
- Se preserve el historial de cambios en cada repositorio

## 🔧 CONFIGURACIÓN ADICIONAL NECESARIA

### Para automatizar el proceso:

1. Configurar hooks de Git para sincronización automática
2. Crear scripts para manejo de submódulos
3. Configurar CI/CD para validación de cambios
4. Establecer flujo de trabajo para commits coordinados

### Para permisos de escritura:

1. Añadir al usuario/agente como colaborador en todos los repositorios
2. Configurar tokens de acceso personal si es necesario
3. Verificar permisos de push en todas las ramas
4. Configurar autenticación SSH si se requiere

---

**NOTA**: Este contexto debe ser proporcionado a cualquier agente que vaya a trabajar con este proyecto para asegurar que entienda la estructura de submódulos y los permisos necesarios para lograr el objetivo de commits coordinados.
