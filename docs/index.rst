.. Sistema Gateway Agrícola documentation master file, created by
   sphinx-quickstart on Sun Jan 20 14:30:00 2025.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Sistema Gateway Agrícola
========================

Sistema avanzado de **Gateway Central** para monitoreo agrícola basado en **red mesh LoRa** que actúa como punto de concentración y procesamiento de datos provenientes de múltiples nodos sensores distribuidos en el campo.

.. image:: _static/gateway_architecture.png
   :alt: Arquitectura del Sistema Gateway
   :align: center
   :width: 600px

.. toctree::
   :maxdepth: 2
   :caption: Descripción General:

   README
   index

.. toctree::
   :maxdepth: 2
   :caption: Componentes del Sistema:

   main_gateway
   config
   node_identity
   radio_manager
   rtc_manager
   app_logic

.. toctree::
   :maxdepth: 2
   :caption: Guías de Uso:

   installation
   configuration
   troubleshooting
   api_reference

.. toctree::
   :maxdepth: 2
   :caption: Diagramas y Arquitectura:

   diagramas_arquitectura
   logs_ejemplo
   ejemplos_tutoriales

.. toctree::
   :maxdepth: 2
   :caption: Diagramas de Código:

   diagramas_codigo/README
   diagramas_avanzados/README
   herramientas_diagramas

.. toctree::
   :maxdepth: 2
   :caption: Desarrollo:

   contributing
   changelog
   roadmap

Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search` 