#include <stdio.h>
#include "processor.h"
#include "log.h"

void inicializarProcessor(Processor *processor, int id, int tiempo, Buffer *buffer)
{
    processor = (Processor *)malloc(sizeof(*processor));
    processor->id = id;
    processor->frecuencia = tiempo;
    processor->buffer = buffer;

    pthread_t tid;
    pthread_create(&tid, NULL, processorWork, (void *)processor);

    pthread_detach(tid);
}

void *processorWork(void *arg)
{
    struct Processor *proc = (struct Processor *)arg;
    while (1)
    {
        int resultado = obtenerValores(proc->buffer);
        char info[100];
        if (resultado > 0)
        {
            sprintf(info, "El promedio del buffer %d es %d", proc->buffer->id, resultado);
            escribirLog(info);
        }
        sleep(proc->frecuencia);
    }

    return NULL;
}
