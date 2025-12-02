# 🔍 Proyecto: Tarea 2
## 👥 Integrantes del Equipo

| Nombre | GitHub | Matrícula |
|--------|--------|-----------|
| Eduardo Alfonso Mora Hernández | [@Eddomora](https://github.com/Eddomora) | 2023403989 |
| Lucas Daniel Morales Oyanedel | [@Falling-Bridge](https://github.com/Falling-Bridge) | 2023441490 |
| Vicente Ignacio Miranda Gómez | [@VicenteM32](https://github.com/VicenteM32) | 2023412848 |
| Ignacio Jesús Soto Miranda | [@Liivne](http://github.com/Liivne) | 2023447412 |

## 📖 Descripción del Proyecto
Este repositorio contiene la solución a la Tarea 2 del curso de Sistemas Operativos. El proyecto se divide en dos componentes principales:

1.  **Parte I - Sincronización:** Implementación de una **barrera reutilizable** utilizando el patrón de diseño *Monitor* (con `pthread_mutex` y `pthread_cond`) para coordinar la ejecución concurrente de $N$ hebras a través de múltiples etapas.
2.  **Parte II - Memoria Virtual:** Un simulador de paginación bajo demanda que implementa el **algoritmo de reemplazo "Reloj" (Clock)**. Este programa lee trazas de memoria y gestiona fallos de página simulando una memoria física limitada.

> **Nota:** El análisis experimental detallado, las tablas de resultados de la tasa de fallos y la descripción profunda de la arquitectura se encuentran en el archivo **`Informe.pdf`** incluido en este repositorio.


## 📂 Estructura de Archivos
```text
.
├── README.md                   # Documentación general
├── Informe.pdf                 # Análisis de resultados
├── Makefile                    # Script de automatización
├── barrera/                    # Directorio Parte I
│   └── barrera_reutilizable.c
└── simulador/                  # Directorio Parte II
    ├── simulador_memoria_virtual.c
    ├── trace1.txt             
    └── trace2.txt              # Trazas con direcciones virtuales para analizar
```

## 🛠️ Instrucciones de Compilación

## Compilación y ejecución

Se proporciona un `Makefile` que compila y ejecuta los dos programas principales del repositorio: la `barrera` y el `simulador` de memoria.

Compilar todo:
```bash
make all
```

Compilar ambos programas:
```bash
make compile
```

Limpiar binarios generados:
```bash
make clean
```

Ejecutar el simulador (compila automáticamente si es necesario):
```bash
make run-sim ARGS="<Nmarcos> <tamaño_página> [--verbose] <ruta_traza>"
# ejemplo:
make run-sim ARGS="8 4096 trace2.txt"
```

Ejecutar la barrera:
```bash
make run-barrera 
```

Ejecutar todas las pruebas automatizadas:
```bash
make test
```

Este objetivo ejecuta el simulador con diferentes configuraciones:
- **trace1.txt** (tamaño de marco: 8 bytes): 8, 16, 32 marcos
- **trace2.txt** (tamaño de marco: 4096 bytes): 8, 16, 32 marcos


#### Ejemplos rápidos
- Ejecutar simulador con 8 marcos y tamaño de página 4096:
```bash
make run-sim ARGS="8 4096 trace2.txt"
```
- Ejecutar simulador con 16 marcos, tamaño 8, y modo verbose:
```bash
make run-sim ARGS="16 8 --verbose trace1.txt"
```
- Ejecutar barrera:
```bash
make run-barrera
```
- Ejecutar todas las pruebas:
```bash
make test
```
```bash
make run-barrera
```
