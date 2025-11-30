# 🔍 Proyecto: Tarea 2

## 👥 Integrantes del Equipo

| Nombre | GitHub | Matrícula |
|--------|--------|-----------|
| Eduardo Alfonso Mora Hernández| [@Eddomora](https://github.com/Eddomora) | 2023403989 |
| Lucas Daniel Morales Oyanedel | [@Falling-Bridge](https://github.com/Falling-Bridge) | 2023441490 |
| Vicente Ignacio Miranda Gómez | [@VicenteM32](https://github.com/VicenteM32) | 2023412848 |
| Ignacio Jesús Soto Miranda |  [@Liivne](http://github.com/Liivne)  | 2023447412 | 

## Introducción.
En el presente proyecto hacemos uso de nuestros conocimientos aprendidos en Sistemas Operativos, la cual está relacionada con las primitivas de sincronización y con el manejo de memoria virtual.

Algunas de sus características principales son:
 * 


## Estructura del proyecto
```
.
├── README.md
├── barrera/
│   ├── barrera
│   └── barrera_reutilizable.c
├── simulador/
│   ├── sim
│   └── simulador_memoria_virtual.c
├── makefile
└── (otros archivos y carpetas de apoyo)
```


gcc src/main.c -o main 
## Compilación y ejecución

Se proporciona un `Makefile` que compila y ejecuta los dos programas principales del repositorio: la `barrera` y el `simulador` de memoria.

Compilar ambos programas:
```bash
make compile
```

Ejecutar el simulador (compila automáticamente si es necesario):
```bash
make run-sim ARGS="<Nmarcos> <tamaño_página> [--verbose] <ruta_traza>"
# ejemplo:
make run-sim ARGS="8 4096 simulador/trace2.txt"
```

Ejecutar la barrera:
```bash
make run-barrera ARGS="<args_para_barrera>"
```

Limpiar binarios generados:
```bash
make clean
```

#### Ejemplos rápidos
- Ejecutar simulador con 8 marcos y tamaño de página 4096:
```bash
make run-sim ARGS="8 4096 simulador/trace2.txt"
```
- Ejecutar simulador con 16 marcos, tamaño 8, y modo verbose:
```bash
make run-sim ARGS="16 8 --verbose simulador/trace1.txt"
```
- Ejecutar barrera:
```bash
make run-barrera
```

## Implementación de Funciones.
#### Función main(): Flujo principal
 * Contiene 

#### Función : .
* 

## Comando Traduccion de Direcciones
Sirve .

**Métricas medidas:** 
 * 

### Modo de uso
Use los objetivos `run-sim` y `run-barrera` para integrarlos en scripts o pruebas automatizadas; el menú `make run` es útil para ejecución manual.



