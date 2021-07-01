#include "buffer.h"
#include <stdio.h>  /* for convenience */
#include <stdlib.h> /* for convenience */
#include <stddef.h> /* for offsetof */
#include <string.h> /* for convenience */
#include <unistd.h> /* for convenience */
#include <signal.h> /* for SIG_ERR */

typedef struct Processor
{
    int id;
    Buffer *buffer;
    int frecuencia;
} Processor;

void inicializarProcessor(Processor *processor, int id, int tiempo, Buffer *buffer);

void *processorWork(void *arg);
