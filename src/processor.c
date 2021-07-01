#include <stdio.h>
#include "processor.h"

void inicializarProcessor(Processor *processor, int id, int tiempo, Buffer *buffer)
{
    processor = (Processor *)malloc(sizeof(*processor));
    processor->id = id;
    processor->frecuencia = tiempo;
    processor->buffer = buffer;

    pthread_t tid;
    pthread_create(&tid, NULL, processorWork, (void *)processor);

    pthread_detach(tid); //Separate thread separation to prevent stiff threads from being generated
}

void *processorWork(void *arg)
{
    struct Processor *proc = (struct Processor *)arg;
    while (1)
    {
        int resultado = obtenerValores(proc->buffer);
        printf("Resultado de buffer %d es %d\n", proc->buffer->id, resultado);
        sleep(proc->frecuencia);
    }

    return NULL;
}
