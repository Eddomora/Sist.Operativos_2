/* Simulador simple de memoria virtual

Objetivos
Implementar un simulador secuencial de memoria virtual que procese una traza de
páginas virtuales.
Soportar el algoritmo de reemplazo Reloj.
Medir y reportar el número de fallos de página y la tasa de fallos para distintas
parámetros de configuración.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>   

#define MAX_PAGES 100000
#define TRACE_FILE "trace.txt"
#define NUM_PAGES MAX_PAGES

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

    /* Imprimir encabezado con nombre de traza en mayúsculas y configuración */
    char trace_basename[256];
    const char *slash = strrchr(trace_file, '/');
    const char *bname = (slash) ? slash + 1 : trace_file;
    size_t blen = strlen(bname);
    if (blen >= sizeof(trace_basename)) blen = sizeof(trace_basename) - 1;
    for (size_t i = 0; i < blen; i++) trace_basename[i] = toupper((unsigned char)bname[i]);
    trace_basename[blen] = '\0';

    printf("=== %s ===\n", trace_basename);
    printf("Config: %d marcos, tamaño %d\n\n", num_frames, page_size);

    /* Asignación dinámica de memoria basada en num_frames */
    int *page_table = (int *)malloc(NUM_PAGES * sizeof(int));
    bool *reference_bits = (bool *)malloc(num_frames * sizeof(bool));
    int *frame_pages = (int *)malloc(num_frames * sizeof(int));

    if (!page_table || !reference_bits || !frame_pages) {
        fprintf(stderr, "Error al asignar memoria\n");
        return 1;
    }
    
    for (int i = 0; i < NUM_PAGES; i++) page_table[i] = -1;
    for (int i = 0; i < num_frames; i++) {
        reference_bits[i] = false;
        frame_pages[i] = -1;
    }

    int clock_pointer = 0;
    int total_references = 0;
    int page_faults = 0;
    int frames_used = 0;

    unsigned long virtual_address;
    while (fscanf(file, "%lx", &virtual_address) == 1) {
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
    printf("=== Resultados ===\n");
    printf("Referencias totales: %d\n", total_references);
    printf("Fallos de página: %d\n", page_faults);
    printf("Tasa de fallos: %.2f%%\n", fault_rate);

    /* Liberar memoria asignada dinámicamente */
    free(page_table);
    free(reference_bits);
    free(frame_pages);

    return 0;
}