#include "log.h"
#include <string.h>

char *modo = "r";

void inicializarLog(char *log_mode)
{
    if (log_mode != NULL)
    {
        modo = log_mode;
    }

    if (strcmp(modo, "d") == 0)
    {
        openlog("rover", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
    }
}

void escribirLog(char *mensaje)
{
    if (strcmp(modo, "d") == 0)
    {
        printf("Entre");
        syslog(LOG_INFO, "%s", mensaje);
    }
    else
    {
        printf("%s\n", mensaje);
    }
}