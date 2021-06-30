#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define BUFLEN 128
#define QLEN 10

#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX 256
#endif

#define QUEUESIZE 10

void crearInformacion(const char *buffers, char *info);
int gettRandom(int lower, int upper);
int conexionSocket(int puerto, const char *buffers);

int main(int argc, char const *argv[])
{
    if (argc < 0)
    {
        fprintf(stderr, "Se necesita dos argumentos\n");
        return -1;
    }
    const char *buffers = argv[1];
    const int port = atoi(argv[2]);
    int error = conexionSocket(port, buffers);
    if (error < 0)
    {
        return error;
    }
    return 0;
}

int conexionSocket(int puerto, const char *buffers)
{
    struct sockaddr_in direccion_destino;

    memset(&direccion_destino, 0, sizeof(direccion_destino)); //ponemos en 0 la estructura direccion_servidor
    //llenamos los campos
    direccion_destino.sin_family = AF_INET;                     //IPv4
    direccion_destino.sin_port = htons(puerto);                 //Convertimos el numero de puerto al endianness de la red
    direccion_destino.sin_addr.s_addr = inet_addr("127.0.0.1"); //Nos vinculamos a la interface localhost o podemos usar INADDR_ANY para ligarnos A TODAS las interfaces
    int fd;
    if ((fd = socket(direccion_destino.sin_family, SOCK_STREAM, 0)) < 0)
    {
        printf("Error al crear socket\n");
        return -1;
    };

    int res = connect(fd, (struct sockaddr *)&direccion_destino, sizeof(direccion_destino));
    if (res < 0)
    {
        printf("Error al conectar\n");
    }
    else
    {
        printf("Conectado\n");
    }
    while (1)
    {
        int n = 0;
        char info[100];
        crearInformacion(buffers, info);
        n = write(fd, info, 100);
        sleep(5);
    }

    return 0;
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
    sprintf(info, "%s,%d,%ld", buffers, numeroAleatorio, seconds);
}
