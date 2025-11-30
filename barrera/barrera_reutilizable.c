#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

/*
int count; numero de hebras que han llegado a la barrera en tiempo actual
int N; número total de hebras que deben esperar
int etapa; Identificador de etapa donde operan N hebras
*/

typedef struct
{
	int count;
	int N;
	int etapa;
	pthread_mutex_t mutex; // Para exclusión mutua
	pthread_cond_t cond;   // Para esperar a que todas las hebras lleguen
} Barrera;

// Función para inicializar Barrera
void barrera_init(Barrera *b, int n)
{
	b->count = 0;
	b->N = n;
	b->etapa = 0;
	pthread_mutex_init(&b->mutex, NULL);
	pthread_cond_init(&b->cond, NULL);
}

// Función destrucción de Barrera
void barrera_destroy(Barrera *b)
{
	pthread_mutex_destroy(&b->mutex);
	pthread_cond_destroy(&b->cond);
}

// Función de operación wait
void barrera_wait(Barrera *b)
{
	pthread_mutex_lock(&b->mutex); // Entrar al monitor(lock)

	int etapa_actual = b->etapa; // guardar en que etapa cree que está la hebra
	b->count++;			    // Modificar estado

	if (b->count == b->N)
	{
		/*
		Caso para la última hebra en llegar,
		en este caso se resetea el contador y se cambia de etapa.
		Luego se despierta a todas las otras hebras.
		*/
		b->count = 0;
		b->etapa++;
		pthread_cond_broadcast(&b->cond);
	}
	else
	{
		/*
		Caso que la hebra no es la última en entrar.
		La hebra debe esperar mientras la etapa global sea igual a la guardada.
		La última hebra hará 'etapa++' y se romperá el ciclo
		*/
		while (etapa_actual == b->etapa)
		{
			pthread_cond_wait(&b->cond, &b->mutex);
		}
	}

	pthread_mutex_unlock(&b->mutex); // salir del monitor(unlock)
}

// Aplicación de Verificación:

typedef struct
{
	int id;
	int etapas_totales;
	Barrera *barrera;
} ArgsHebras;

void *constructor(void *arg)
{
	ArgsHebras *args = (ArgsHebras *)arg;
	int id = args->id;
	int E = args->etapas_totales;
	Barrera *barrera = args->barrera;

	for (int e = 0; e < E; e++)
	{
		usleep(rand() % 200000); // Para simular y desincronizar las hebras a propósito
		// imprimir antes de esperar
		printf("Hebra %d: esperando en etapa %d\n", id, e);
		barrera_wait(barrera);
		// imprimir después de pasar
		printf("Henbra %d: pasó barrera en etapa %d\n", id, e);
	}
	free(args);
	return NULL;
}

int main(int argc, char *argv[])
{
	// Parámetros por defecto (leer enunciado)
	int N = 5;
	int E = 4;

	// Agregar opción para cambiar los parámetros por defecto para ejecutar con valores personalizados
	if (argc > 1)
		N = atoi(argv[1]); // Convierte argv[1] a entero con atoi
	if (argc > 2)
		E = atoi(argv[2]); // Convierte argv[2] a entero con atoi

	printf("Simulación con N=%d hebras y E=%d etapas. \n", N, E);
	srand(time(NULL));

	// Inicializar barrera
	Barrera barrera;
	barrera_init(&barrera, N);

	pthread_t hebras[N];

	// Crear hebras:
	for (int i = 0; i < N; i++)
	{
		ArgsHebras *args = malloc(sizeof(ArgsHebras));
		args->id = i;
		args->etapas_totales = E;
		args->barrera = &barrera;
		pthread_create(&hebras[i], NULL, constructor, (void *)args);
	}
	for (int i = 0; i < N; i++)
	{ // esperar a que terminen
		pthread_join(hebras[i], NULL);
	}

	// limpiar
	barrera_destroy(&barrera);
	printf("Simulación Finalizada\n");
	return 0;
}

/*
Para compilar: gcc barrera_reutilizable.c
Para ejecutar con valores predefinidos (N=5, E=4): ./a.out
Para ejecutar con valores personalizados: ./a.out (valor1) (valor2)
Ej: ./a.out 3 2
*/