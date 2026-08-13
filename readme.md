# Algorithms for CP

## Generar el algoritmario en PDF

El generador toma automáticamente los archivos C/C++ guardados dentro de las
carpetas del repositorio. Produce un PDF tamaño carta con portada, índice
navegable, separadores por categoría, marcadores y código con resaltado de
sintaxis.

```bash
python3 -m pip install -r requirements-pdf.txt
python3 generate_algoritmario.py
```

El resultado se guarda en `output/pdf/algoritmario_icpc.pdf`.

Opciones útiles:

```bash
python3 generate_algoritmario.py --title "Mi equipo ICPC"
python3 generate_algoritmario.py --output ruta/algoritmario.pdf
python3 generate_algoritmario.py --extensions .cpp .h .hpp
```
