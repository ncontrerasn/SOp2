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

#define PORT 3540

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

int main(int argc, char **argv)
{
	FILE *ptr;
	//HashTable *ht = create_table(CAPACITY);
	//ht = hash_db();
	int opcion, validacion, id, i, h2;
	struct dogType *mascota, dog;
	char nombre[32], historia, charId[12];
	int clientfd, serverfd, r, tope = 100000000, acc, peticion = 8, opt = 1;
    char *vector, *bufferV;
    vector = (char *)malloc(tope * sizeof(char));
    bufferV = (char *)malloc(tope * sizeof(char));
    struct sockaddr_in client;
    struct hostent *he;
	long long buffsize = 1000000;
    
    clientfd = socket(AF_INET, SOCK_STREAM, 0);
    if(clientfd < 0){
        perror("\n-->Error en socket():");
        exit(-1);
    }
    client.sin_family = AF_INET;
    client.sin_port = htons(PORT);

    inet_aton(argv[1], &client.sin_addr);

    setsockopt(clientfd, SOL_SOCKET, SO_RCVBUF, &buffsize, sizeof(buffsize));

	r = connect(clientfd, (struct sockaddr *)&client, (socklen_t)sizeof(struct sockaddr));
    if(r < 0){
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
			if(r < 0){
				perror("\n-->Error en send(): ");
				exit(-1);
			}

			r = send(clientfd, mascota, sizeof(struct dogType), 0);
			if(r < 0){
				perror("\n-->Error en send(): ");
				exit(-1);
			}
			//ht = guardarRegistro(ht, (void *)mascota);
			__fpurge(stdin);
			printf("\n          Presiona cualquier tecla para regresar al menu.");
			getch();
			break;
		case 2:
			printf("\n          Numero de registros: %i", registros);
			printf("\n          ID de la mascota que deseas ver: ");
			scanf("%i", &id);
			if (id % 100 != 0)
			{
				__fpurge(stdin);
				printf("          ID invalido.\n");
				printf("\n          Presiona cualquier tecla para regresar al menu.\n");
				getch();
				break;
			}
			/*dog = leerEsctructura(id, dog);
			validacion = verMascota(dog);
			if (validacion == 1)
			{
				printf("          ID invalido. La mascota con ese ID ha sido eliminada del sistema.\n");
				printf("\n          Presiona cualquier tecla para regresar al menu.\n");
				__fpurge(stdin);
				getch();
				break;
			}
			__fpurge(stdin);
			printf("          Abrir historia clinica? (S: si | N: no): ");
			scanf("%c", &historia);
			if (historia == 's' || historia == 'S')
			{
				sprintf(charId, "%d", id);

				validacion = mostrarHistoria((char *)dog.nombre, charId);
				if (validacion != 0)
				{
					perror("          Error abriendo historia");
					exit(-1);
				}
				__fpurge(stdin);
				printf("\n          Presiona cualquier tecla para regresar al menu.\n");
				getch();
			}
			else
			{
				__fpurge(stdin);
				printf("\n          Presiona cualquier tecla para regresar al menu.\n");
				getch();
			}
			break;
		case 3:
			printf("\n          Numero de registros: %i\n", registros);
			printf("\n          ID de la mascota que deseas borrar: ");
			scanf("%i", &id);
			if (id % 100 != 0)
			{
				__fpurge(stdin);
				printf("          ID invalido.\n");
				printf("\n          Presiona cualquier tecla para regresar al menu.\n");
				getch();
				break;
			}
			dog = leerEsctructura(id, dog);
			if (dog.nombre[0] != '*')
			{
				ht = delete_item(ht, (char *)dog.nombre, id);
				borrarRegistro(id);
				__fpurge(stdin);
				printf("\n          Presiona cualquier tecla para regresar al menu.\n");
				getch();
				break;
			}
			else
			{
				__fpurge(stdin);
				printf("          ID invalido. El ID ingresado ya ha sido eliminado del sistema.\n");
				printf("\n          Presiona cualquier tecla para regresar al menu.\n");
				getch();
				break;
			}
		case 4:
			printf("          Ingresa el nombre de la mascota: ");
			scanf("%s", nombre);
			for (i = 0; nombre[i]; i++)
				if (i == 0)
					nombre[i] = toupper(nombre[i]);
				else
					nombre[i] = tolower(nombre[i]);
			print_search(ht, (char *)nombre);
			__fpurge(stdin);
			printf("\n          Presiona cualquier tecla para regresar al menu.\n");
			getch();
			break;*/
		case 5:
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
	/*printf("\n          Nombre: ");
	scanf("%s", mascota->nombre);
	for (i = 0; mascota->nombre[i]; i++)
		if (i == 0)
			mascota->nombre[i] = toupper(mascota->nombre[i]);
		else
			mascota->nombre[i] = tolower(mascota->nombre[i]);
	printf("          Tipo: ");
	scanf("%s", mascota->tipo);
	printf("          Edad:");
	scanf("%i", &mascota->edad);
	printf("          Raza: ");
	scanf("%s", mascota->raza);
	printf("          Estatura: ");
	scanf(" %i", &mascota->estatura);
	printf("          Peso: ");
	scanf(" %f", &mascota->peso);
	printf("          Sexo: ");
	scanf(" %c", &mascota->sexo);*/

	sprintf(mascota->nombre,"Ringo");
	sprintf(mascota->tipo,"perro");
	mascota->edad = 4;
	sprintf(mascota->raza,"beagle");
	mascota->estatura = 23;
	mascota->peso = 8;
	mascota->sexo = 'M';

	mascota->next = -1;
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