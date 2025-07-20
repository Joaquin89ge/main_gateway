@echo off
REM Actualiza el diagrama de conexiones y la documentación HTML

REM 1. Generar el diagrama de conexiones automáticamente
echo Generando diagrama de conexiones desde config.h...
python generate_board_diagram.py

REM 2. Generar la documentación HTML con Sphinx
echo Generando documentación HTML...
cd docs
sphinx-build -b html . _build/html
cd ..

echo.
echo Documentación y diagrama actualizados correctamente.
pause 