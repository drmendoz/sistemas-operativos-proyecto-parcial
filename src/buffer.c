#include <stdio.h>
#include <pthread.h>
#include "buffer.h"
#include <stdlib.h>
Buffer *inicializarBuffer(int id)
{
    Buffer *buffer = (Buffer *)malloc(sizeof(*buffer));
    buffer->id = id;
    if (pthread_mutex_init(&(buffer->sem), NULL) != 0)
    {
        printf("Error al iniciar semaforo\n");
    }
    /*  for (size_t i = 0; i < BUFF_SIZE; i++)
    {
        buffer->valores[i] = (ValorSensor *)malloc(sizeof(ValorSensor));
    }
*/
    return buffer;
}

void anadirValorBuffer(Buffer *buffer, char *hora, int valor)
{
    pthread_mutex_lock(&(buffer->sem));
    for (int i = 0; i < BUFF_SIZE; i++)
    {
        if (buffer->valores[i] == NULL || i == BUFF_SIZE - 1)
        {
            struct ValorSensor *valorSensor = malloc(sizeof(*valorSensor));
            valorSensor->hora = hora;
            valorSensor->valor = valor;
            buffer->valores[i] = valorSensor;
            break;
        }
    }
    pthread_mutex_unlock(&(buffer->sem));
}

int obtenerValores(Buffer *buffer)
{
    int prom = 0;
    int sum = 0;
    int cont = 0;
    pthread_mutex_lock(&(buffer->sem));
    for (int i = 0; i < BUFF_SIZE; i++)
    {
        if (buffer->valores[i] != NULL)
        {
            sum += buffer->valores[i]->valor;
            cont += 1;
        }
    }
    if (cont == 0)
    {
        cont = 1;
    }

    pthread_mutex_unlock(&(buffer->sem));
    prom = sum / cont;
    return prom;
}