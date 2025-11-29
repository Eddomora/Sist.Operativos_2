/* Simulador simple de memoria virtual

Objetivos
Implementar un simulador secuencial de memoria virtual que procese una traza de
páginas virtuales.
Soportar el algoritmo de reemplazo Reloj.
Medir y reportar el número de fallos de página y la tasa de fallos para distintas
parámetros de configuración.

*/

/*En esta parte debe implementar un simulador, sim con paginación simple y soporte de
algoritmo de reemplazo Reloj con un solo puntero.*/

/*Actividad 1: Simulador de Traducción de Direcciones
Implemente un simulador, sim, que lea una traza de direcciones virtuales (una por línea, en
decimal o 0xHEX), realice la traducción a direcciones físicas usando paginación simple, y reporte
el número de fallos de página y la dirección física resultante. Mantenga un conjunto de marcos
físicos de tamaño fijo y use una política simple de asignación: si hay marcos libres, asignar; si
no, reemplazar usando el algoritmo del Reloj.
1. Definición: Traducción en hexadecimal
Sea una dirección virtual DV en hexadecimal, con un tamaño de marco de página PAGE_SIZE =
2
b bytes. Sea además MASK = 2b − 1 el valor que permite extraer los bits de desplazamiento (offset) dentro de la página. Recuerde que en notación binaria, la dirección virtual
se divide en componente nvp y offset:
Descomposición de la dirección virtual:
offset = VA & MASK,
npv = DV ≫ b.
donde & denota la operación AND bit a bit y ≫ b el desplazamiento a la derecha de
b bits.
Cálculo de la dirección física:
Si la tabla de páginas asocia la página virtual nvp a un marco físico marco, entonces
la dirección física (DF) se obtiene como:
DF = (marco ≪ b) | offset,
donde ≪ b representa el desplazamiento a la izquierda (equivalente a multiplicar por
2
b
), y | la operación OR bit a bit.
2. Reglas de traducción
DV compuesta por npv y offset usando PAGE_SIZE.
Si npv está en la Tabla de páginas hay un HIT: luego, se obtiene marco y computa
DF.
Si no está hay un FALLO: luego, si hay marco libre asignarlo a pv; en otro caso,
expulsar víctima según algoritmo del Reloj. Actualizar la tabla de páginas (invalidar
npv expulsada, mapear npv con fallo a marco).
3
3. Parámetros del simulador
./sim Nmarcos tamañomarco [--verbose] traza.txt
Donde Nmarcos corresponde al número de margos disponibles, tamaño
del marco en bytes y verbose es opcional e indica la secuencia paso a paso en
el proceso de traducción. En caso de no realizarse solo reportar el número
de páginas.
# Ejemplo:
./sim 8 4096 traza.txt
4. Salida de simulación
Totales: Referencias, Fallos de página, y Tasa de fallos.
Con –verbose: por cada DV, imprimir: DV, nvp, offset, HIT/FALLO, marco usado, DF
calculada.
5. Datos de prueba
Genere entre 5 y 10 referencias y verifique sus resultados.
Use los dos archivos de referencias en traces.zip y realice un análisis experimental para
ver como se comporta la tasa de fallos variando el número de marcos disponibles.
Considere los siguientes rangos de marcos de páginas (8, 16, 32) y tamaño de marco
de 8 para trace1.txt y 4096 para trace2.txt.*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>   

#define MAX_PAGES 100000
#define MAX_FRAMES 100
#define TRACE_FILE "trace.txt"
#define PAGE_SIZE 4096
#define MEMORY_SIZE (PAGE_SIZE * MAX_FRAMES)
#define NUM_PAGES MAX_PAGES
#define CLOCK_ALGORITHM 1

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Uso: %s Nmarcos tamañomarco [--verbose] traza.txt\n", argv[0]);
        return 1;
    }

    int num_frames = atoi(argv[1]);
    int page_size = atoi(argv[2]);
    bool verbose = false;
    char *trace_file;

    if (argc == 5 && strcmp(argv[3], "--verbose") == 0) {
        verbose = true;
        trace_file = argv[4];
    } else {
        trace_file = argv[3];
    }

    FILE *file = fopen(trace_file, "r");
    if (!file) {
        perror("Error al abrir el archivo de traza");
        return 1;
    }

    int page_table[NUM_PAGES];
    bool reference_bits[MAX_FRAMES];
    int frame_pages[MAX_FRAMES];  // Mapea marco a página virtual
    
    for (int i = 0; i < NUM_PAGES; i++) page_table[i] = -1;
    for (int i = 0; i < MAX_FRAMES; i++) {
        reference_bits[i] = false;
        frame_pages[i] = -1;
    }

    int clock_pointer = 0;
    int total_references = 0;
    int page_faults = 0;
    int frames_used = 0;

    unsigned long virtual_address;
    while (fscanf(file, "%lx", &virtual_address) != EOF) {
        total_references++;
        int page_number = virtual_address / page_size;
        int offset = virtual_address % page_size;

        if (page_table[page_number] != -1) {
            // HIT
            int frame_number = page_table[page_number];
            reference_bits[frame_number] = true;
            unsigned long physical_address = (frame_number * page_size) + offset;

            if (verbose) {
                printf("VA: 0x%lx, Page: %d, Offset: %d, HIT, Frame: %d, PA: 0x%lx\n",
                       virtual_address, page_number, offset, frame_number, physical_address);
            }
        } else {
            // FALLO
            page_faults++;
            int frame_number = -1;

            // Buscar un marco libre
            if (frames_used < num_frames) {
                frame_number = frames_used;
                frames_used++;
            } else {
                // Algoritmo del Reloj: buscar víctima
                while (reference_bits[clock_pointer]) {
                    reference_bits[clock_pointer] = false;
                    clock_pointer = (clock_pointer + 1) % num_frames;
                }
                frame_number = clock_pointer;
                int victim_page = frame_pages[frame_number];
                page_table[victim_page] = -1;  // Invalidar página expulsada
                clock_pointer = (clock_pointer + 1) % num_frames;
            }

            // Asignar nueva página al marco
            page_table[page_number] = frame_number;
            frame_pages[frame_number] = page_number;
            reference_bits[frame_number] = true;
            unsigned long physical_address = (frame_number * page_size) + offset;

            if (verbose) {
                printf("VA: 0x%lx, Page: %d, Offset: %d, FALLO, Frame: %d, PA: 0x%lx\n",
                       virtual_address, page_number, offset, frame_number, physical_address);
            }
        }
    }

    fclose(file);

    // Imprimir resultados
    double fault_rate = (total_references > 0) ? (100.0 * page_faults / total_references) : 0.0;
    printf("\n=== Resultados ===\n");
    printf("Referencias totales: %d\n", total_references);
    printf("Fallos de página: %d\n", page_faults);
    printf("Tasa de fallos: %.2f%%\n", fault_rate);

    return 0;
}