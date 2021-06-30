#include <stdio.h>
#include <pthread.h>
#include "buffer.h"
#include <stdlib.h>
void inicializarBuffer(Buffer *buffer)
{
    buffer = (Buffer *)malloc(sizeof(buffer));
    pthread_mutex_init(buffer->sem, NULL);
}

void anadirValorBuffer(Buffer *buffer, time_t hora, int valor)
{
    pthread_mutex_lock(buffer->sem);
    for (int i = 0; i < BUFF_SIZE; i++)
    {
        if (buffer->valores[i] != NULL)
        {
            buffer->valores[i]->hora = hora;
            buffer->valores[i]->valor = valor;
        }
    }
    pthread_mutex_unlock(buffer->sem);
}

int obtenerValores(Buffer *buffer)
{
    int prom, sum, cont = 0;
    pthread_mutex_lock(buffer->sem);
    for (size_t i = 0; i < BUFF_SIZE; i++)
    {
        if (buffer->valores[i] != NULL)
        {
            sum += buffer->valores[i]->valor;
            cont += 1;
        }
    }
    pthread_mutex_unlock(buffer->sem);
    prom = sum / cont;
    return prom;
}