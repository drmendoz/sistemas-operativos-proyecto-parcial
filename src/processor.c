#include <stdio.h>
#include "processor.h"

void inicializarProcessor(Processor *processor, int tiempo, Buffer *buffer)
{
    processor->frecuencia = tiempo;
    processor->buffer = buffer;
}

void *proccesorWork(void *arg)
{
    struct Processor *proc = (struct Processor *)arg;
    while (1)
    {
        int resultado = obtenerValores(proc->buffer);
        printf("%d", resultado);
        sleep(proc->frecuencia);
    }
}
