#include <stdio.h>
#include <pthread.h>
#include <time.h>

#define BUFF_SIZE 20

typedef struct ValorSensor
{
    int valor;
    time_t hora;
} ValorSensor;

typedef struct Buffer
{
    pthread_mutex_t sem;
    ValorSensor *valores[BUFF_SIZE];
} Buffer;

Buffer *inicializarBuffer();

int obtenerValores(Buffer *buffer);

void anadirValorBuffer(Buffer *buffer, time_t hora, int valor);