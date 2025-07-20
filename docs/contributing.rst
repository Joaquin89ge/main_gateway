Contribución al Proyecto
========================

Gracias por tu interés en contribuir al Sistema Gateway Agrícola. Esta guía te ayudará a participar en el desarrollo del proyecto.

Cómo Contribuir
--------------

Reportar Problemas
~~~~~~~~~~~~~~~~~~

1. **Buscar Issues Existentes:**
   - Revisar si el problema ya está reportado
   - Verificar si hay una solución en progreso

2. **Crear un Issue Detallado:**
   - Usar el template de issue correspondiente
   - Incluir información del sistema
   - Proporcionar logs de error
   - Describir pasos para reproducir

3. **Información Requerida:**
   - Versión del firmware
   - Hardware utilizado
   - Configuración actual
   - Logs completos

Proponer Mejoras
~~~~~~~~~~~~~~~~

1. **Discutir la Idea:**
   - Crear un issue para discutir la mejora
   - Obtener feedback de la comunidad
   - Definir alcance y prioridad

2. **Crear una Propuesta:**
   - Documentar la funcionalidad
   - Diseñar la implementación
   - Considerar impacto en rendimiento

3. **Implementar:**
   - Seguir las guías de desarrollo
   - Escribir tests unitarios
   - Actualizar documentación

Enviar Pull Requests
~~~~~~~~~~~~~~~~~~~

1. **Fork del Repositorio:**
   - Crear un fork del proyecto
   - Clonar tu fork localmente

2. **Crear una Rama:**
   - Usar nombres descriptivos
   - Seguir convenciones de nomenclatura

3. **Desarrollar:**
   - Seguir estándares de código
   - Escribir tests
   - Actualizar documentación

4. **Enviar PR:**
   - Usar template de pull request
   - Describir cambios realizados
   - Incluir tests y documentación

Estándares de Código
-------------------

Formato de Código
~~~~~~~~~~~~~~~~~

1. **Indentación:**
   - Usar 2 espacios para indentación
   - No usar tabs

2. **Nomenclatura:**
   - Clases: PascalCase (ej: `NodeIdentity`)
   - Métodos: camelCase (ej: `getNodeID()`)
   - Constantes: UPPER_SNAKE_CASE (ej: `MAX_NODES`)
   - Variables: camelCase (ej: `nodeID`)

3. **Comentarios:**
   - Usar comentarios descriptivos
   - Documentar funciones complejas
   - Incluir ejemplos cuando sea necesario

Ejemplo de Código:
.. code-block:: cpp

   /**
    * @brief Obtiene el identificador único del nodo
    * @param blacklist_len Tamaño de la lista negra
    * @param blacklist Array de valores prohibidos
    * @return ID único entre 1-254
    */
   uint8_t getNodeID(size_t blacklist_len = 2, 
                     const uint8_t *blacklist = defaultBlacklist) {
     // Implementación aquí
   }

Estructura de Archivos
~~~~~~~~~~~~~~~~~~~~~

1. **Organización:**
   - Un archivo por clase
   - Headers en `include/`
   - Implementación en `src/`
   - Tests en `test/`

2. **Nombres de Archivos:**
   - Usar snake_case
   - Ser descriptivos
   - Incluir extensión apropiada

Ejemplo:
.. code-block:: text

   src/
   ├── main_gateway.ino
   ├── node_identity.h
   ├── node_identity.cpp
   ├── radio_manager.h
   ├── radio_manager.cpp
   └── config.h

Convenciones de Git
~~~~~~~~~~~~~~~~~~

1. **Mensajes de Commit:**
   - Usar formato convencional
   - Ser descriptivo y conciso
   - Incluir tipo de cambio

Ejemplos:
.. code-block:: text

   feat: agregar soporte para RTC DS1302
   fix: corregir timeout en comunicación LoRa
   docs: actualizar documentación de instalación
   test: agregar tests para NodeIdentity
   refactor: optimizar gestión de memoria

2. **Ramas:**
   - `main`: código estable
   - `develop`: desarrollo activo
   - `feature/*`: nuevas funcionalidades
   - `fix/*`: correcciones de bugs
   - `docs/*`: documentación

Testing
-------

Tests Unitarios
~~~~~~~~~~~~~~

1. **Cobertura:**
   - Tests para todas las clases
   - Cobertura de casos edge
   - Tests de integración

2. **Estructura:**
   - Un test por clase
   - Tests independientes
   - Setup y teardown apropiados

Ejemplo:
.. code-block:: cpp

   #include <unity.h>
   #include "node_identity.h"

   void setUp(void) {
     // Setup antes de cada test
   }

   void tearDown(void) {
     // Cleanup después de cada test
   }

   void test_getNodeID_returns_valid_id(void) {
     NodeIdentity identity;
     uint8_t nodeID = identity.getNodeID();
     TEST_ASSERT_GREATER_THAN(0, nodeID);
     TEST_ASSERT_LESS_THAN(255, nodeID);
   }

   int main(void) {
     UNITY_BEGIN();
     RUN_TEST(test_getNodeID_returns_valid_id);
     return UNITY_END();
   }

Tests de Integración
~~~~~~~~~~~~~~~~~~~

1. **Hardware:**
   - Tests con hardware real
   - Simulación cuando sea posible
   - Validación de comunicación

2. **Red:**
   - Tests de comunicación LoRa
   - Validación de protocolo
   - Tests de rendimiento

Documentación
------------

Estándares de Documentación
~~~~~~~~~~~~~~~~~~~~~~~~~~

1. **Comentarios de Código:**
   - Usar formato Doxygen
   - Documentar parámetros y retornos
   - Incluir ejemplos de uso

2. **Documentación de Usuario:**
   - Guías paso a paso
   - Ejemplos prácticos
   - Screenshots cuando sea necesario

3. **Documentación Técnica:**
   - Arquitectura del sistema
   - Diagramas de flujo
   - Especificaciones técnicas

Ejemplo de Documentación:
.. code-block:: cpp

   /**
    * @class NodeIdentity
    * @brief Gestiona identificación única de nodos
    * 
    * Esta clase genera un identificador único basado en la dirección MAC
    * del hardware, con protección contra colisiones mediante lista negra.
    * 
    * @example
    * NodeIdentity identity;
    * uint8_t nodeID = identity.getNodeID();
    * Serial.printf("Node ID: %d\n", nodeID);
    */
   class NodeIdentity {
   public:
     /**
      * @brief Obtiene el identificador único del nodo
      * @param blacklist_len Tamaño de la lista negra
      * @param blacklist Array de valores prohibidos
      * @return ID único entre 1-254
      */
     uint8_t getNodeID(size_t blacklist_len = 2, 
                       const uint8_t *blacklist = defaultBlacklist);
   };

Proceso de Review
----------------

Criterios de Aceptación
~~~~~~~~~~~~~~~~~~~~~~

1. **Funcionalidad:**
   - Código funciona correctamente
   - Tests pasan
   - No introduce regresiones

2. **Calidad:**
   - Sigue estándares de código
   - Documentación actualizada
   - Performance aceptable

3. **Seguridad:**
   - No introduce vulnerabilidades
   - Manejo seguro de datos
   - Validación apropiada

Proceso de Review
~~~~~~~~~~~~~~~~

1. **Automático:**
   - Tests automáticos
   - Análisis de código estático
   - Verificación de formato

2. **Manual:**
   - Review de código por pares
   - Verificación de funcionalidad
   - Validación de documentación

3. **Aprobación:**
   - Al menos 2 aprobaciones
   - Todos los tests pasan
   - Documentación actualizada

Comunicación
-----------

Canales de Comunicación
~~~~~~~~~~~~~~~~~~~~~~

1. **Issues de GitHub:**
   - Reportar problemas
   - Proponer mejoras
   - Discutir implementación

2. **Discussions:**
   - Preguntas generales
   - Compartir experiencias
   - Solicitar ayuda

3. **Email:**
   - Contacto directo
   - Asuntos privados
   - Coordinación de desarrollo

Código de Conducta
~~~~~~~~~~~~~~~~~

1. **Respeto:**
   - Ser respetuoso con otros
   - Escuchar diferentes opiniones
   - Construir sobre ideas de otros

2. **Colaboración:**
   - Ayudar a otros desarrolladores
   - Compartir conocimiento
   - Mentorear nuevos contribuidores

3. **Profesionalismo:**
   - Mantener comunicación profesional
   - Ser constructivo en feedback
   - Reconocer contribuciones

Reconocimiento
-------------

Contribuidores
~~~~~~~~~~~~~

1. **Tipos de Contribución:**
   - Código
   - Documentación
   - Testing
   - Diseño
   - Ideas

2. **Reconocimiento:**
   - Lista de contribuidores
   - Mención en releases
   - Agradecimientos en documentación

3. **Incentivos:**
   - Badges de contribuidor
   - Acceso a features beta
   - Participación en decisiones

Para comenzar a contribuir:

1. **Leer la Documentación:**
   - Guías de instalación
   - Estándares de código
   - Proceso de desarrollo

2. **Explorar el Código:**
   - Entender la arquitectura
   - Revisar issues abiertos
   - Identificar áreas de mejora

3. **Empezar Pequeño:**
   - Corregir bugs simples
   - Mejorar documentación
   - Agregar tests

4. **Contactar:**
   - Crear un issue
   - Unirse a discussions
   - Solicitar mentoría

¡Gracias por contribuir al Sistema Gateway Agrícola! 