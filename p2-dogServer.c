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

#define PORT 3543
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

int mostrarHistoria(char nombre[], char id[], int clientfd);

int guardarHistoria(char nombre[], char id[], int clientfd);

int main(){

    int serverfd, clientfd, r, opt = 1, tope = 100000000, acc = 0, i, peticion, cero = 0;
    char *vector, *vector2;
    vector = (char *)malloc(tope * sizeof(char));
    vector2 = (char *)malloc(tope * sizeof(char));
    struct sockaddr_in server, client;
    socklen_t tamano;
    char opcion[1], idS[10], nombre[32], historia[1];
    struct dogType *mascota, m2, m3;

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

    r = recv(clientfd, opcion, sizeof(char), 0);
    if(r < 0){
        perror("\n-->Error en recv(): ");
        exit(-1);
    }

    printf("%c", opcion[0]);
    
    switch (opcion[0])
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
            r = send(clientfd, "920", sizeof(char) * 3, 0);
			if(r < 0){
				perror("\n-->Error en send(): ");
				exit(-1);
			}

            r = recv(clientfd, idS, sizeof(char) * 10, 0);
            if(r < 0){
                perror("\n-->Error en recv(): ");
                exit(-1);
            }
            printf("id %s", idS);

            sprintf(m2.nombre,"Luna");
            sprintf(m2.tipo,"perro");
            m2.edad = 5;
            sprintf(m2.raza,"criollo");
            m2.estatura = 21;
            m2.peso = 9;
            m2.sexo = 'M';
            m2.next = -1;

            r = send(clientfd, &m2, sizeof(struct dogType), 0);
			if(r < 0){
				perror("\n-->Error en send(): ");
				exit(-1);
			}

            r = recv(clientfd, historia, sizeof(char), 0);
            if(r < 0){
                perror("\n-->Error en recv(): ");
                exit(-1);
            }

            if(historia[0] == 's'){//revisar si ya existe la historia de ese perro
                
                r = mostrarHistoria((char *)m2.nombre, idS, clientfd);
				if (r != 0)
				{
					perror("          Error abriendo historia");
					exit(-1);
				}

                guardarHistoria((char *)m2.nombre, idS, clientfd);

            }
            fprintf(f,"[Fecha %s] [Cliente %s] [Lectura] [Nombre: %s, tipo: %s, edad: %d, raza: %s, estatura: %d, peso: %f, sexo: %c]\n", bufff, clientip, m2.nombre, m2.tipo, m2.edad, m2.raza, m2.estatura, m2.peso, m2.sexo);
            break;

        case '3':
            r = send(clientfd, "920", sizeof(char) * 3, 0);
			if(r < 0){
				perror("\n-->Error en send(): ");
				exit(-1);
			}

            r = recv(clientfd, idS, sizeof(char) * 10, 0);
            if(r < 0){
                perror("\n-->Error en recv(): ");
                exit(-1);
            }

            printf("id %s", idS);
            sprintf(m2.nombre,"Luna");
            sprintf(m2.tipo,"perro");
            m2.edad = 5;
            sprintf(m2.raza,"criollo");
            m2.estatura = 21;
            m2.peso = 9;
            m2.sexo = 'M';
            m2.next = -1;

            //if exito en el borrado, mandar o, de lo contrario, mandar x
            r = send(clientfd, "x", sizeof(char), 0);
			if(r < 0){
				perror("\n-->Error en send(): ");
				exit(-1);
			}

            fprintf(f,"[Fecha %s] [Cliente %s] [Borrado] [Nombre: %s, tipo: %s, edad: %d, raza: %s, estatura: %d, peso: %f, sexo: %c]\n", bufff, clientip, m2.nombre, m2.tipo, m2.edad, m2.raza, m2.estatura, m2.peso, m2.sexo);
            break;

        case '4':
            r = recv(clientfd, nombre, sizeof(char) * 32, 0);
            if(r < 0){
                perror("\n-->Error en recv(): ");
                exit(-1);
            }
            printf("nombre %s", nombre);
            struct dogType *mascotas;
            mascotas = (struct dogType *)malloc(50 * sizeof(struct dogType));
            sprintf(m2.nombre,"Luna");
            sprintf(m2.tipo,"perro");
            m2.edad = 5;
            sprintf(m2.raza,"criollo");
            m2.estatura = 21;
            m2.peso = 9;
            m2.sexo = 'M';
            m2.next = -1;

            sprintf(m3.nombre,"Tobby");
            sprintf(m3.tipo,"perro");
            m3.edad = 6;
            sprintf(m3.raza,"shitzu");
            m3.estatura = 18;
            m3.peso = 6;
            m3.sexo = 'H';
            m3.next = -1;

            for(i = 0; i < 25; i++)
                mascotas[i] = m2;

            for(i = 25; i < 50; i++)
                mascotas[i] = m3;

            //mandar todas las mascotas de la busquda
            r = send(clientfd, mascotas, sizeof(struct dogType)*50, 0);
            printf("%d", r);
            fprintf(f,"[Fecha %s] [Cliente %s] [Bùsqueda] [Cadena buscada: %s]\n", bufff, clientip, nombre);

            break;

        case '5':
            break;
        }
    

    close(clientfd);
    close(serverfd);   

    return 0;
}

int mostrarHistoria(char nombre[], char id[], int clientfd)
{

	FILE *historia;
	int r;
	char dir[500], cwd[500], buff[500];

	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("error getcwd()");
		exit(-1);
	}

	strcat(dir, "historias_clinicas");
	dir[23] = 92;
	//strcat(dir, " && gedit ");
	strcat(dir, id);
	strcat(dir, "_");
	strcat(dir, nombre);
	strcat(dir, ".txt");

	historia = fopen("historias_clinicas/100_Luna.txt", "r");
    if(historia == NULL) printf("marica");
    //while ( ) != NULL ){ // fgets reads upto MAX character or EOF 
    fgets(buff,500,historia);
    r = send(clientfd,buff,sizeof(buff), 0);//send(clientfd, buff, sizeof(char) * 500, 0);
			if(r < 0){
				perror("\n-->Error en send(): ");
				exit(-1);
			}
    printf("%s",buff); 
    printf("%d",r); 
    //}
    printf("----");
    fclose(historia);

	return 0;
}

int guardarHistoria(char nombre[], char id[], int clientfd)
{

	FILE *historia;
	int r;
	char dir[500], cwd[500], buff[500];

	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("error getcwd()");
		exit(-1);
	}

	strcat(dir, "historias_clinicas");
	dir[23] = 92;
	//strcat(dir, " && gedit ");
	strcat(dir, id);
	strcat(dir, "_");
	strcat(dir, nombre);
	strcat(dir, ".txt");

	historia = fopen("historias_clinicas/100_Luna.txt", "w");
    if(historia == NULL) printf("marica");
    recv(clientfd,buff,500, 0);

	fprintf(historia, "%s", buff);
    fclose(historia);

	return 0;
}