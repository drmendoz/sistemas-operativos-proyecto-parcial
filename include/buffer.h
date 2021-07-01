#include <stdio.h>
#include <pthread.h>
#include <time.h>

#define BUFF_SIZE 20

typedef struct ValorSensor
{
    int valor;
    char *hora;
} ValorSensor;

typedef struct Buffer
{
    int id;
    pthread_mutex_t sem;
    ValorSensor *valores[BUFF_SIZE];
} Buffer;

Buffer *inicializarBuffer(int id);

int obtenerValores(Buffer *buffer);

void anadirValorBuffer(Buffer *buffer, char *hora, int valor);