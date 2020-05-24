#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define PORT 3540
#define BACKLOG 2
#define BUF_LEN 256

struct dogType
{
	char nombre[32];
	char tipo[32];
	int edad;
	char raza[16];
	int estatura;
	float peso;
	char sexo;
	int next;
};

int main(){

    int serverfd, clientfd, r, opt = 1, tope = 100000000, acc = 0, i, peticion, cero = 0;
    char *vector, *vector2;
    vector = (char *)malloc(tope * sizeof(char));
    vector2 = (char *)malloc(tope * sizeof(char));
    struct sockaddr_in server, client;
    socklen_t tamano;
    char opcion[1], id[10];
    struct dogType *mascota;

    mascota = malloc(sizeof(struct dogType));
	if (mascota == NULL)
	{
		perror("Error malloc");
		exit(-1);
	}

    char bufff[BUF_LEN] = {0};
    time_t rawtime = time(NULL);
    struct tm *ptm = localtime(&rawtime);
    strftime(bufff, BUF_LEN, "%d/%m/%YT%X", ptm);
    

    FILE *f;
    f = fopen("serverDogs.log", "a+"); // a+ (create + append) option will allow appending which is useful in a log file
    if (f == NULL) { /* Something is wrong   */}
        
    serverfd = socket(AF_INET, SOCK_STREAM, 0);
    if(serverfd < 0){
        perror("\n-->Error en socket():");
        exit(-1);
    }
    
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;
    bzero(server.sin_zero, 8); 
    
    setsockopt(serverfd,SOL_SOCKET,SO_REUSEADDR,(const char *)&opt,sizeof(int));

    r = bind(serverfd, (struct sockaddr *)&server, sizeof(struct sockaddr));
    if(r < 0){
        perror("\n-->Error en bind(): ");
        exit(-1);
    }
    
    r = listen(serverfd, BACKLOG);
    if(r < 0){
        perror("\n-->Error en Listen(): ");
        exit(-1);
    }
    
    clientfd = accept(serverfd, (struct sockaddr *)&client, &tamano);
    if(clientfd < 0)
    {
        perror("\n-->Error en accept: ");
        exit(-1);
    }

    struct sockaddr_in addr;
    socklen_t addr_size = sizeof(struct sockaddr_in);
    int res = getpeername(clientfd, (struct sockaddr *)&addr, &addr_size);
    char *clientip = (char *)malloc(20 * sizeof(char));
    strcpy(clientip, inet_ntoa(addr.sin_addr));

    //printf("c ip: %s\n", clientip);

    r = recv(clientfd, opcion, sizeof(char), 0);
    if(r < 0){
        perror("\n-->Error en recv(): ");
        exit(-1);
    }

    //fprintf(f, "puto el que lo lea\n");

    printf("%c", opcion[0]);
    
    switch (buf[0])
		{
		case '1':
            r = recv(clientfd, mascota, sizeof(struct dogType), 0);
            if(r < 0){
                perror("\n-->Error en recv(): ");
                exit(-1);
            }
            fprintf(f,"[Fecha %s] [Cliente %s] [Inserciòn] [Nombre: %s, tipo: %s, edad: %d, raza: %s, estatura: %d, peso: %f, sexo: %c]\n", bufff, clientip, mascota->nombre, mascota->tipo, mascota->edad, mascota->raza, mascota->estatura, mascota->peso, mascota->sexo);
            break;

        case '2':
            r = recv(clientfd, id, sizeof(char) * 10, 0);
            if(r < 0){
                perror("\n-->Error en recv(): ");
                exit(-1);
            }
            fprintf(f,"[Fecha %s] [Cliente %s] [Lectura] [Nombre: %s, tipo: %s, edad: %d, raza: %s, estatura: %d, peso: %f, sexo: %c]\n", bufff, clientip, mascota->nombre, mascota->tipo, mascota->edad, mascota->raza, mascota->estatura, mascota->peso, mascota->sexo);
            
            break;

        case '3':
            break;

        case '4':
            break;

        case '5':
            break;
        }

        
    printf("nombre: %s\n", mascota->nombre);
    printf("edad: %d\n", mascota->edad);
    close(clientfd);
    close(serverfd);   
}