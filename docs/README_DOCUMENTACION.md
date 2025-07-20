# Documentación del Sistema Gateway Agrícola

## 📚 Documentación Generada

Esta carpeta contiene la documentación completa del Sistema Gateway Agrícola generada con Sphinx.

### 📁 Estructura de Archivos

```
docs/
├── _build/
│   ├── html/          # Documentación HTML
│   └── latex/         # Archivos LaTeX para PDF
├── *.md               # Documentación en Markdown
├── *.rst              # Documentación en reStructuredText
├── conf.py            # Configuración de Sphinx
└── index.rst          # Página principal
```

### 🌐 Documentación HTML

La documentación HTML está disponible en la carpeta `_build/html/`. Para verla:

1. Abre el archivo `_build/html/index.html` en tu navegador web
2. Navega por la documentación usando el menú lateral
3. Usa la función de búsqueda para encontrar contenido específico

### 📄 Generación de PDF

Para generar el PDF desde los archivos LaTeX:

1. Instala LaTeX en tu sistema (MiKTeX, TeX Live, etc.)
2. Navega a la carpeta `_build/latex/`
3. Ejecuta: `make` o `pdflatex GatewayAgricola.tex`

### 🔧 Comandos de Sphinx

#### Generar HTML:

```bash
cd docs
sphinx-build -b html . _build/html
```

#### Generar LaTeX:

```bash
cd docs
sphinx-build -b latex . _build/latex
```

#### Generar PDF (requiere LaTeX):

```bash
cd docs/_build/latex
make
```

### 📖 Contenido de la Documentación

#### Documentación Técnica (Markdown):

- **README.md**: Resumen ejecutivo del sistema
- **main_gateway.md**: Punto de entrada y inicialización
- **config.md**: Configuración central y parámetros
- **node_identity.md**: Gestión de identidad del nodo
- **radio_manager.md**: Comunicación LoRa
- **rtc_manager.md**: Gestión del RTC DS1302
- **app_logic.md**: Lógica de aplicación

#### Guías de Usuario (reStructuredText):

- **installation.rst**: Guía de instalación
- **configuration.rst**: Guía de configuración
- **troubleshooting.rst**: Solución de problemas
- **api_reference.rst**: Referencia de API
- **contributing.rst**: Guía de contribución

### 🎨 Características de la Documentación

- **Tema**: Read the Docs (sphinx_rtd_theme)
- **Idioma**: Español
- **Formato**: HTML y PDF
- **Búsqueda**: Integrada en HTML
- **Navegación**: Menú lateral y breadcrumbs
- **Responsive**: Compatible con móviles

### 🔍 Funcionalidades

- **Búsqueda en tiempo real**
- **Navegación por secciones**
- **Enlaces internos**
- **Resaltado de sintaxis**
- **Índice automático**
- **Referencias cruzadas**

### 📝 Notas Importantes

1. **Advertencias**: Algunas advertencias de formato en archivos RST son normales y no afectan la funcionalidad
2. **Dependencias**: La generación de PDF requiere LaTeX instalado
3. **Actualización**: Para actualizar la documentación, ejecuta los comandos de Sphinx nuevamente
4. **Personalización**: Edita `conf.py` para cambiar la configuración

### 🚀 Próximos Pasos

1. Revisar la documentación HTML generada
2. Generar PDF si es necesario
3. Publicar la documentación en un servidor web
4. Configurar actualizaciones automáticas

---

**Equipo de Desarrollo Agrícola**  
_Documentación generada con Sphinx v8.2.3_
