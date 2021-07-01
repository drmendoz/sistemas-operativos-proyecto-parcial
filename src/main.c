#include <sys/types.h> /* some systems still require this */
#include <sys/stat.h>
#include <stdio.h>  /* for convenience */
#include <stdlib.h> /* for convenience */
#include <stddef.h> /* for offsetof */
#include <string.h> /* for convenience */
#include <unistd.h> /* for convenience */
#include <signal.h> /* for SIG_ERR */
#include <netdb.h>
#include <errno.h>
#include <syslog.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/resource.h>
#include <pthread.h>
#include "processor.h"
#include "log.h"

#define BUFLEN 128

#define NUM_BUFF 10

#define QUEUESIZE 1

Buffer *buffs[NUM_BUFF];
Processor *procs[NUM_BUFF];
void decodificarMensajeSensor(char *mensaje);

void inicializarServidor(int port);

void inicializandoBuffers();
void inicializandoProcessors();

void decodificarMensajeSensor(char *str)
{

    char delim[] = " ";
    char *ptr = strtok(str, delim);

    char *primeritem = ptr;
    ptr = strtok(NULL, delim);
    char valor[3];
    strcpy(valor, ptr);
    ptr = strtok(NULL, delim);
    char tiempo[60];
    strcpy(tiempo, ptr);
    // haciendo el array del primer elemento
    int i = 0;
    char *p = strtok(primeritem, ",");
    char *array[100];

    while (p != NULL)
    {
        printf("%s", p);
        int bufferId = atoi(p);
        if (bufferId < 0 || bufferId > NUM_BUFF)
        {
            escribirLog("No existe buffer");
        }
        else
        {
            for (int j = 0; j < NUM_BUFF; j++)
            {
                Buffer *buff = buffs[j];
                if (buff->id == j)
                {
                    anadirValorBuffer(buff, tiempo, atoi(valor));
                }
            }
        }
        array[i++] = p;
        p = strtok(NULL, ",");
    }
}

int main(int argc, char const *argv[])
{
    const int port = atoi(argv[1]);
    const int frecuencia = atoi(argv[2]);
    char *modo = (char *)argv[3];
    inicializarLog(modo);
    inicializandoBuffers();

    inicializandoProcessors(frecuencia);

    inicializarServidor(port);

    return 0;
}

struct SensorPaquete
{
    int fd;
};

void inicializandoProcessors(int frecuencia)
{
    for (int i = 0; i < NUM_BUFF; i++)
    {
        inicializarProcessor(procs[i], i, frecuencia, buffs[i]);
    }
}

void inicializandoBuffers()
{
    for (int i = 0; i < NUM_BUFF; i++)
    {
        buffs[i] = inicializarBuffer(i);
    }
}

void *
manejandoConexion(void *arg)
{
    struct SensorPaquete *var = (struct SensorPaquete *)arg;
    int n;
    printf("Numero conexion exitosa %d\n", var->fd);
    while (1)
    {

        char buf[100]; //16
        n = read(var->fd, buf, sizeof(buf));
        if (n == 0)
        {
            printf("The Client %d closed!\n", var->fd);
            break;
        }
        else
        {
            printf("%s", buf);
            decodificarMensajeSensor(buf);
        }
    }
    close(var->fd);
    return NULL;
}

void inicializarServidor(int port)
{
    struct sockaddr_in direccion_servidor;

    memset(&direccion_servidor, 0, sizeof(direccion_servidor)); //ponemos en 0 la estructura direccion_servidor

    //llenamos los campos
    direccion_servidor.sin_family = AF_INET;                     //IPv4
    direccion_servidor.sin_port = htons(port);                   //Convertimos el numero de puerto al endianness de la red
    direccion_servidor.sin_addr.s_addr = inet_addr("127.0.0.1"); //Nos vinculamos a la interface localhost o podemos usar INADDR_ANY para ligarnos A TODAS las interfaces

    int fd;

    if ((fd = socket(direccion_servidor.sin_family, SOCK_STREAM, 0)) < 0)
    {
        printf("Error al crear socket\n");
    }

    if (bind(fd, (struct sockaddr *)&direccion_servidor, sizeof(direccion_servidor)) < 0)
    {
        printf("Error en bind\n");
    }

    if (listen(fd, QUEUESIZE) < 0)
    {
        printf("Error en listen\n");
    }
    else
    {
        printf("Servidor escuchando en puerto %d \n", port);
    }
    pthread_t tid;
    struct SensorPaquete ts[128];
    int i = 0;

    while (1)
    {
        int sockfd_conectado = accept(fd, NULL, 0);
        if (sockfd_conectado > 0)
        {
            printf("%s", "Conexion exitosa con cliente\n");
            ts[i].fd = sockfd_conectado;
            pthread_create(&tid, NULL, manejandoConexion, (void *)&ts[i]);

            pthread_detach(tid); //Separate thread separation to prevent stiff threads from being generated
            i++;
        }
        else
        {
            printf("%s", "Error de conexion\n");
        }
    }
}
