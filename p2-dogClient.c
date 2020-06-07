#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <stdio_ext.h>
#include <termios.h>
#include <math.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#define PORT 3543

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

int menu(int opcion);

char getch(void);

int registrarMascota(void *puntero);

int verMascota(struct dogType mascota);

int mostrarHistoria(char nombre[], char id[], int clientfd);

int main(int argc, char **argv)
{
	int opcion, validacion, id, i, h2,numRegistros;
	long long buffsize = 100000000;
	struct dogType *mascota, dog;
	char nombre[32], historia, charId[12], idS[10], respuesta[1], contenidoHistoria[50], consulta;
	int clientfd, serverfd, r, tope, acc, opt = 1;
	struct sockaddr_in client;
	struct hostent *he;

	clientfd = socket(AF_INET, SOCK_STREAM, 0);
	if (clientfd < 0)
	{
		perror("\n-->Error en socket():");
		exit(-1);
	}
	client.sin_family = AF_INET;
	client.sin_port = htons(PORT);

	inet_aton(argv[1], &client.sin_addr);

	setsockopt(clientfd, SOL_SOCKET, SO_RCVBUF, &buffsize, sizeof(buffsize));

	r = connect(clientfd, (struct sockaddr *)&client, (socklen_t)sizeof(struct sockaddr));
	if (r < 0)
	{
		perror("\n-->Error en connect(): ");
		exit(-1);
	}

	mascota = malloc(sizeof(struct dogType));
	if (mascota == NULL)
	{
		perror("Error malloc");
		exit(-1);
	}

	do
	{
		opcion = menu(opcion);

		switch (opcion)
		{
		case 1:
			validacion = registrarMascota((void *)mascota);
			if (validacion != 0)
			{
				perror("Error registrando");
				exit(-1);
			}

			r = send(clientfd, "1", sizeof(char), 0);
			if (r < 0)
			{
				perror("\n-->Error en send(): ");
				exit(-1);
			}

			r = send(clientfd, mascota, sizeof(struct dogType), 0);
			if (r < 0)
			{
				perror("\n-->Error en send(): ");
				exit(-1);
			}

			__fpurge(stdin);
			printf("\n          Mascota registrada.");
			printf("\n          Presiona cualquier tecla para regresar al menù.");
			getch();
			break;
		case 2:
			r = send(clientfd, "2", sizeof(char), 0);
			if (r < 0)
			{
				perror("\n-->Error en send(): ");
				exit(-1);
			}

			r = recv(clientfd, &numRegistros, sizeof(int), 0); //recibe numero de registros
			if (r < 0)
			{
				perror("\n-->Error en recv(): ");
				exit(-1);
			}
			printf("\n          Nùmero de registros: %d", numRegistros);
			printf("\n          ID de la mascota que deseas ver: ");
			scanf("%i", &id);

			r = send(clientfd, &id, sizeof(int), 0);
			if (r < 0)
			{
				perror("\n-->Error en send(): ");
				exit(-1);
			}
			if(id%100 !=0){
				printf("          ID invàlido.\n");
				break;
			}

			r = recv(clientfd, &dog, sizeof(struct dogType), 0);
			if (r < 0)
			{
				perror("\n-->Error en recv(): ");
				exit(-1);
			}
	

			validacion = verMascota(dog);
			if (validacion == 1)
			{
				printf("          ID invàlido. La mascota con el ID: %d ha sido eliminada del sistema.\n", id);
				printf("\n          Presiona cualquier tecla para regresar al menù.\n");
				__fpurge(stdin);
				getch();
				break;
			}
			__fpurge(stdin);
			printf("          Abrir historia clìnica? (S: sì | N: no): ");
			scanf("%c", &historia);
			if (historia == 's' || historia == 'S')
			{
				sprintf(charId, "%d", id); //madar s al servidor para saber si ya hay historia en el sistema
				r = send(clientfd, "s", sizeof(char), 0);
				if (r < 0)
				{
					perror("\n-->Error en send(): ");
					exit(-1);
				}

				validacion = mostrarHistoria((char *)dog.nombre, charId, clientfd); //haciendolo
				if (validacion != 0)
				{
					perror("          Error abriendo historia.");
					exit(-1);
				}
				__fpurge(stdin);
				printf("\n          Presiona cualquier tecla para regresar al menù.\n");
				getch();
			}
			else
			{
				r = send(clientfd, "n", sizeof(char), 0);
				if (r < 0)
				{
					perror("\n-->Error en send(): ");
					exit(-1);
				}
			}

			break;
		case 3:

			r = send(clientfd, "3", sizeof(char), 0);
			if (r < 0)
			{
				perror("\n-->Error en send(): ");
				exit(-1);
			}

			r = recv(clientfd, &numRegistros, sizeof(int), 0); //recibe numero de registros
			if (r < 0)
			{
				perror("\n-->Error en recv(): ");
				exit(-1);
			}
			printf("\n          Nùmero de registros: %d", numRegistros);
			printf("\n          ID de la mascota que deseas borrar: ");
			scanf("%i", &id);
						
			r = send(clientfd, &id, sizeof(int) , 0);
			if (r < 0)
			{
				perror("\n-->Error en send(): ");
				exit(-1);
			}

			r = recv(clientfd, respuesta, sizeof(char), 0);
			if (r < 0)
			{
				perror("\n-->Error en recv(): ");
				exit(-1);
			}

			if (respuesta[0] == 'o')
			{
				printf("\n          La mascota con el ID: %d fue eliminada del sistema.\n", id);
				printf("\n          Presiona cualquier tecla para regresar al menu.\n");
				getch();
			}
			else
			{
				__fpurge(stdin);
				printf("\n          ID invalido. El ID ingresado ya ha sido eliminado del sistema.\n");
				printf("\n          Presiona cualquier tecla para regresar al menu.\n");
				getch();
			}
			break;
		case 4:
			printf("          Ingresa el nombre de la mascota: ");
			scanf("%s", nombre);
			for (i = 0; nombre[i]; i++)
				if (i == 0)
					nombre[i] = toupper(nombre[i]);
				else
					nombre[i] = tolower(nombre[i]);
			r = send(clientfd, "4", sizeof(char), 0);
			if (r < 0)
			{
				perror("\n-->Error en send(): ");
				exit(-1);
			}
			r = send(clientfd, nombre, sizeof(char) * 32, 0);
			if (r < 0)
			{
				perror("\n-->Error en send() ");
				exit(-1);
			}
			int acc;
			int pos;
			r = recv(clientfd, &consulta, sizeof(char), 0);
			if(consulta == 'n'){
				printf("          La mascota con el ID: %s no està registrada en la base de datos.\n", nombre);
				
			}else{
				printf("\n");
				int i = 0, cont, acc = 0;
				r = recv(clientfd, &cont, sizeof(int), 0);
				struct dogType *vectorR = (struct dogType *)malloc(cont * sizeof(struct dogType));
				r = recv(clientfd, vectorR, sizeof(struct dogType)*cont, 0);
				while (i<cont-1){
					printf("          Id: %d\n", vectorR[i].next);
					verMascota(vectorR[i]);
					i++;
					printf("%c", '\n');
					if(i == cont) break;
				}
				if (cont == 1)
					printf("          La mascota no està registrada en la base de datos.\n");
				printf("contador %d", cont - 1);
				free(vectorR);
			}
			__fpurge(stdin);
			printf("\n          Presiona cualquier tecla para regresar al menu.\n");
			getch();
			break;
		case 5:
			r = send(clientfd, "5", sizeof(char), 0);
			if (r < 0)
			{
				perror("\n-->Error en send(): ");
				exit(-1);
			}
			__fpurge(stdin);
			printf("\n          Presiona cualquier tecla para salir.\n");
			getch();
			free(mascota);
			break;
		}
	} while (opcion != 5);

	return 0;
}

int menu(int opcion)
{
	printf("                                                                   \n");
	printf("                       __                                          \n");
	printf("                     .'  '.                                        \n");
	printf("                 _.-'/  | o .                                      \n");
	printf("      ,      _.-'  ,|   /    `-.                                   \n");
	printf("     /'   .-''       `--''-.__.'=========================          \n");
	printf("    ( '-'`        .___.--._)                             |         \n");
	printf("     '            .'                                     |         \n");
	printf("      |     /,_.-'       Veterinaria Patas               |         \n");
	printf("    _/   _.'(                                            |         \n");
	printf("   /  ,-'|(  '                                           |         \n");
	printf("  (  (   |  `-'        1. Ingresar registro           /`-._        \n");
	printf("   `-'   |             2. Ver registro              _/,.._/        \n");
	printf("         |             3. Borrar registro        ,-'   ,  `-:,.-') \n");
	printf("         |             4. Buscar registro       : o ):';     _  {  \n");
	printf("         |             5. Salir                  `-.  `' _,.-(`-.) \n");
	printf("         |                                           `((`|`-,.-'   \n");
	printf("          ===============================================          \n");
	do
	{
		printf("\n          Opcion: ");
		scanf("%i", &opcion);
	} while (opcion < 1 || opcion > 5);
	return opcion;
}

int registrarMascota(void *puntero)
{
	int i;
	struct dogType *mascota;
	mascota = puntero;
	printf("\n          Nombre: ");
	scanf("%s", mascota->nombre);
	for (i = 0; mascota->nombre[i]; i++)
		if (i == 0)
			mascota->nombre[i] = toupper(mascota->nombre[i]);
		else
			mascota->nombre[i] = tolower(mascota->nombre[i]);
	printf("          Tipo: ");
	scanf("%s", mascota->tipo);
	printf("          Edad: ");
	scanf("%i", &mascota->edad);
	printf("          Raza: ");
	scanf("%s", mascota->raza);
	printf("          Estatura: ");
	scanf(" %i", &mascota->estatura);
	printf("          Peso: ");
	scanf(" %f", &mascota->peso);
	printf("          Sexo: ");
	scanf(" %c", &mascota->sexo);
	mascota->next = -1;
	return 0;
}

int verMascota(struct dogType mascota)
{
	if (mascota.nombre[0] == '*')
		return 1;
	else
	{
		printf("          Nombre: %s\n", mascota.nombre);
		printf("          Tipo: %s\n", mascota.tipo);
		printf("          Edad: %i\n", mascota.edad);
		printf("          Raza: %s\n", mascota.raza);
		printf("          Estatura: %i\n", mascota.estatura);
		printf("          Peso: %f\n", mascota.peso);
		printf("          Sexo: %c\n", mascota.sexo);
		return 0;
	}
}

int mostrarHistoria(char nombre[], char id[], int clientfd)
{

	FILE *historia;
	char buff[500], archivo[500], dir[500] = "gedit ", borrar[500] = "rm ", ch;
	int r, cont = 0;

	strcpy(archivo, id);
	strcat(archivo, "_");
	strcat(archivo, nombre);
	strcat(archivo, ".txt");

	historia = fopen(archivo, "w");
	if (historia == NULL)
	{
		perror("error fopen");
		exit(-1);
	}
	r = recv(clientfd,buff,sizeof(buff), 0);

	if(r < 0){
		perror("\n-->Error en recv(): ");
		exit(-1);
	}

	if(buff[0]==' ') 
		printf(" ");
	else
		fprintf(historia, "%s", buff);

	r = fclose(historia);
	if (r < 0)
	{
		perror("Error fclose");
		exit(-1);
	}

	strcat(dir, id);
	strcat(dir, "_");
	strcat(dir, nombre);
	strcat(dir, ".txt");

	system(dir);

	historia = fopen(archivo, "r");
	if (historia == NULL)
	{
		perror("error fopen ");
		exit(-1);
	}
	while ((ch = fgetc(historia)) != EOF)
        {
            buff[cont] = ch;
            cont++;
        }

    char buffS[cont-2];
    for (int i = 0; i < cont-1; i++)
        buffS[i]=buff[i];

    r = send(clientfd,buffS,sizeof(buffS) + 1, 0);
	if (r < 0)
		{
			perror("\n-->Error en send(): ");
			exit(-1);
		}

	strcat(borrar, id);
	strcat(borrar, "_");
	strcat(borrar, nombre);
	strcat(borrar, ".txt");
	system(borrar);

	return 0;
}

char getch(void)
{
	char buf = 0;
	struct termios old = {0};
	fflush(stdout);
	if (tcgetattr(0, &old) < 0)
		perror("tcsetattr()");
	old.c_lflag &= ~ICANON;
	old.c_lflag &= ~ECHO;
	old.c_cc[VMIN] = 1;
	old.c_cc[VTIME] = 0;
	if (tcsetattr(0, TCSANOW, &old) < 0)
		perror("tcsetattr ICANON");
	if (read(0, &buf, 1) < 0)
		perror("read()");
	old.c_lflag |= ICANON;
	old.c_lflag |= ECHO;
	if (tcsetattr(0, TCSADRAIN, &old) < 0)
		perror("tcsetattr ~ICANON");
	return buf;
}