#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#define BUFLEN 128
#define QLEN 10

#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX 256
#endif

#define QUEUESIZE 10

void crearInformacion(const char *buffers, char *info);
int gettRandom(int lower, int upper);
void *conexionSocket(void *args);
void *solicitarComandos(void *args);

typedef struct SocketArgs
{
    char *buffers;
    int port;
} SocketArgs;

int main(int argc, char const *argv[])
{
    if (argc < 0)
    {
        fprintf(stderr, "Se necesita dos argumentos\n");
        return -1;
    }
    char *buffers = (char *)argv[1];
    const int port = atoi(argv[2]);

    pthread_t tid[2];
    pthread_create(&tid[0], NULL, solicitarComandos, NULL);
    struct SocketArgs sa;
    sa.buffers = buffers;
    sa.port = port;
    pthread_create(&tid[1], NULL, conexionSocket, (void *)&sa);
    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);

    return 0;
}

void *solicitarComandos(void *args)
{
    while (1)
    {
        char comando[10];
        printf("Ingrese un comando:\n");
        scanf("%s", comando);
        if (strcmp(comando, "pausa") == 0)
        {
        }
        else if (strcmp(comando, "continuar") == 0)
        {
        }
    }
}

void *conexionSocket(void *args)
{

    struct SocketArgs *var = (struct SocketArgs *)args;
    struct sockaddr_in direccion_destino;

    memset(&direccion_destino, 0, sizeof(direccion_destino)); //ponemos en 0 la estructura direccion_servidor
    //llenamos los campos
    direccion_destino.sin_family = AF_INET;                     //IPv4
    direccion_destino.sin_port = htons(var->port);              //Convertimos el numero de puerto al endianness de la red
    direccion_destino.sin_addr.s_addr = inet_addr("127.0.0.1"); //Nos vinculamos a la interface localhost o podemos usar INADDR_ANY para ligarnos A TODAS las interfaces
    int fd;
    if ((fd = socket(direccion_destino.sin_family, SOCK_STREAM, 0)) < 0)
    {
        printf("Error al crear socket\n");
    };

    int res = connect(fd, (struct sockaddr *)&direccion_destino, sizeof(direccion_destino));
    if (res < 0)
    {
        printf("Error al conectar\n");
    }
    else
    {
        printf("Conectado\n");
        while (1)
        {
            int n = 0;
            char info[100];
            crearInformacion(var->buffers, info);
            n = write(fd, info, 100);
            if (n < 0)
            {
                printf("Error de conexion con framework");
            }
            sleep(5);
        }
    }
    return NULL;
}

int getRandom(int lower, int upper)
{
    return (rand() %
            (upper - lower + 1)) +
           lower;
}

void crearInformacion(const char *buffers, char *info)
{
    time_t seconds;

    seconds = time(NULL);

    int numeroAleatorio = getRandom(0, 255);
    sprintf(info, "%s %d %ld", buffers, numeroAleatorio, seconds);
}
