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
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

#define PORT 3543
#define BACKLOG 2
#define BUF_LEN 256
#define CAPACITY 1800

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

unsigned long hash_function(char *str);

typedef struct Ht_item Ht_item;

struct Ht_item
{
	int head;
	int tail;
};

typedef struct HashTable HashTable;

struct HashTable
{

	Ht_item **items;
	int size;
	int count;
};

Ht_item *create_item(int key, int value);

HashTable *create_table(int size);

void free_item(Ht_item *item);

void free_table(HashTable *table);

void print_table(HashTable *table);

HashTable *ht_insert(HashTable *table, char *key, int value);

Ht_item *ht_search(HashTable *table, char *key);

HashTable *guardarRegistro(HashTable *table, void *puntero);

struct HashTable *hash_db();
//FIN HASH
int borrarRegistro(int posicion);

HashTable *delete_item(HashTable *table, char *key, int code);

struct dogType leerEsctructura(int id, struct dogType dog);

//NUMERO REGISTROS
static int registros = 0;

//Thread function
void *socketThread(void *arg);
//Thread function args
struct arguments{
	
	HashTable *ht;
	int socket;
	FILE *f;
};

int main()
{
	
	struct arguments *arg = (struct arguments *)malloc(sizeof(struct arguments));

	//FILE *ptr;
	
	HashTable *ht = create_table(CAPACITY);
	ht = hash_db();

	printf("Server ready to connect \n");
	//Moved to thread
	/*
	int r, opt = 1, tope = 100000000, acc = 0, i, peticion, cero = 0, id;

	char *vector, *vector2, *key;
	vector = (char *)malloc(tope * sizeof(char));
	vector2 = (char *)malloc(tope * sizeof(char));

	char opcion[1], idS[10], nombre[32], historia[1];
	struct dogType *mascota, m2, m3;

	mascota = malloc(sizeof(struct dogType));
	if (mascota == NULL)
	{
		perror("Error malloc");
		exit(-1);
	}*/
	//Added
	int r, opt = 1;


	//Log - moved to thread
	/*char bufff[BUF_LEN] = {0};
	time_t rawtime = time(NULL);
	struct tm *ptm = localtime(&rawtime);
	strftime(bufff, BUF_LEN, "%d/%m/%YT%X", ptm);*/

	FILE *f;
	f = fopen("serverDogs.log", "a+");
	if (f == NULL)
	{
		perror("Error fopen");
		exit(-1);
	}

	//Sockets
	int serverfd, clientfd;
	struct sockaddr_in server, client;
	socklen_t tamano;

	serverfd = socket(AF_INET, SOCK_STREAM, 0);
	if (serverfd < 0)
	{
		perror("\n-->Error en socket():");
		exit(-1);
	}

	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	server.sin_addr.s_addr = INADDR_ANY;
	bzero(server.sin_zero, 8);

	setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, (const char *)&opt, sizeof(int));

	r = bind(serverfd, (struct sockaddr *)&server, sizeof(struct sockaddr));
	if (r < 0)
	{
		perror("\n-->Error en bind(): ");
		exit(-1);
	}

	r = listen(serverfd, BACKLOG);
	if (r < 0)
	{
		perror("\n-->Error en Listen(): ");
		exit(-1);
	}
	//TODO OK

	//Cambio a threads


	//Threads
	pthread_t tid[60]; //max connections 
    int connections = 0;

	while (1)
	{
		clientfd = accept(serverfd, (struct sockaddr *)&client, &tamano);
		arg->ht =ht;
		arg->socket = clientfd;
		arg->f = f;

		if (pthread_create(&tid[connections], NULL, socketThread, (void * )arg) != 0)
		{
			perror("Failed to create thread\n");
		}
	}

	//ip in log - moved to thread
	/*

	*/

	// ALL MAIN moved to thread function
	/*

	*/
	return 0;
}

void *socketThread(void *arg)
{
	int r, opt = 1, tope = 100000000, acc = 0, i, peticion, cero = 0, id;

	char *vector, *vector2, *key;
	vector = (char *)malloc(tope * sizeof(char));
	vector2 = (char *)malloc(tope * sizeof(char));

	char opcion[1], idS[10], nombre[32], historia[1];
	struct dogType *mascota, m2, m3;

	mascota = malloc(sizeof(struct dogType));
	if (mascota == NULL)
	{
		perror("Error malloc");
		exit(-1);
	}
	//Arguments
	HashTable *ht = ((struct arguments*)arg)->ht;
	int clientfd = ((struct arguments*)arg)->socket;
	FILE *f = ((struct arguments*)arg)->f;

	//Ip log
	struct sockaddr_in addr;
	socklen_t addr_size = sizeof(struct sockaddr_in);
	int res = getpeername(clientfd, (struct sockaddr *)&addr, &addr_size);
	char *clientip = (char *)malloc(20 * sizeof(char));
	strcpy(clientip, inet_ntoa(addr.sin_addr));
	printf("Client IP: %s\n", clientip);
	//


	//LOG
	char bufff[BUF_LEN] = {0};
	time_t rawtime = time(NULL);
	struct tm *ptm = localtime(&rawtime);
	strftime(bufff, BUF_LEN, "%d/%m/%YT%X", ptm);


	do
	{
		r = recv(clientfd, opcion, sizeof(char), 0);
		if (r < 0)
		{
			perror("\n-->Error en recv(): ");
			exit(-1);
		}


		switch (opcion[0])
		{
		case '1':
			r = recv(clientfd, mascota, sizeof(struct dogType), 0);
			ht = guardarRegistro(ht, (void *)mascota);
			if (r < 0)
			{
				perror("\n-->Error en recv(): ");
				exit(-1);
			}

			r=fprintf(f, "[Fecha %s] [Cliente %s] [Inserción] [Nombre: %s, tipo: %s, edad: %d, raza: %s, estatura: %d, peso: %f, 				sexo: %c]\n", bufff, clientip, mascota->nombre, mascota->tipo, mascota->edad, mascota->raza, mascota->estatura, 			mascota->peso, mascota->sexo);
			if (r = 0)
			{
				perror("\n-->Error en fprintf: ");
				//exit(-1);
			}
			printf("NOOO");
			break;

		case '2':
			r = send(clientfd, &registros, sizeof(int), 0);
			if (r < 0)
			{
				perror("\n-->Error en send(): ");
				exit(-1);
			}

			r = recv(clientfd, &id, sizeof(int), 0);
			if (r < 0)
			{
				perror("\n-->Error en recv(): ");
				exit(-1);
			}
			struct dogType dog;
			dog = leerEsctructura(id, dog);

			r = send(clientfd, &dog, sizeof(struct dogType), 0);
			if (r < 0)
			{
				perror("\n-->Error en send(): ");
				exit(-1);
			}

			r = recv(clientfd, historia, sizeof(char), 0);
			if (r < 0)
			{
				perror("\n-->Error en recv(): ");
				exit(-1);
			}

			if (historia[0] == 's')
			{ //revisar si ya existe la historia de ese perro
				sprintf(idS, "%d", id);
				r = mostrarHistoria((char *)dog.nombre, idS, clientfd);
				if (r != 0)
				{
					perror("          Error abriendo historia");
					exit(-1);
				}

				guardarHistoria((char *)dog.nombre, idS, clientfd);
			}
			r=fprintf(f, "[Fecha %s] [Cliente %s] [Lectura] [Nombre: %s, tipo: %s, edad: %d, raza: %s, estatura: %d, peso: %f, sexo: %c]\n", bufff, clientip, m2.nombre, m2.tipo, m2.edad, m2.raza, m2.estatura, m2.peso, m2.sexo);
			
			if (r = 0)
			{
				perror("\n-->Error en fprintf: ");
				//exit(-1);
			}
			break;

		case '3':
			//printf("numero registros %d\n", registros); 
			r = send(clientfd, &registros, sizeof(int), 0);
			if (r < 0)
			{
				perror("\n-->Error en send(): ");
				exit(-1);
			}

			r = recv(clientfd, &id, sizeof(int), 0);
			if (r < 0)
			{
				perror("\n-->Error en recv(): ");
				exit(-1);
			}

			dog = leerEsctructura(id, dog);
			if (dog.nombre[0] != '*')
			{
				ht = delete_item(ht, (char *)dog.nombre, id);
				borrarRegistro(id);
				r = send(clientfd, "o", sizeof(char), 0);

				break;
			}
			else
			{
				r = send(clientfd, "x", sizeof(char), 0);
				if (r < 0)
				{
					perror("\n-->Error en send(): ");
					exit(-1);
				}
			}
			r=fprintf(f, "[Fecha %s] [Cliente %s] [Borrado] [Nombre: %s, tipo: %s, edad: %d, raza: %s, estatura: %d, peso: %f, sexo: %c]\n", bufff, clientip, m2.nombre, m2.tipo, m2.edad, m2.raza, m2.estatura, m2.peso, m2.sexo);
			if (r = 0)
			{
				perror("\n-->Error en fprintf: ");
				//exit(-1);
			}
			break;

		case '4':
			r = recv(clientfd, nombre, sizeof(char) * 32, 0);
			if (r < 0)
			{
				perror("\n-->Error en recv(): ");
				exit(-1);
			}
			key = (char *)nombre;

			Ht_item *val = ht_search(ht, key);
			if (val == NULL)
			{
				r = send(clientfd, "n", sizeof(char), 0);
				break;
			}
			else
			{
				r = send(clientfd, "s", sizeof(char), 0);
				FILE *archivo;
				archivo = fopen("dataDogs.dat", "ab+");
				if (archivo == NULL)
				{
					perror("Error fopen");
					exit(-1);
				}
				struct dogType mascota;
				int pos = val->head, r;
				r = fseek(archivo, val->head, SEEK_SET);
				if (r != 0)
				{
					perror("Error fseek");
					exit(-1);
				}
				r = fread(&mascota, sizeof(struct dogType), 1, archivo);
				if (r = 0)
				{
					perror("Error fread");
					exit(-1);
				}
			
				//int i=0;
				//int j=0;

				while (mascota.next != -1)
				{
					if (strcmp(mascota.nombre, key) == 0)
					{
						r = send(clientfd, &pos, sizeof(int), 0);
						r = send(clientfd, &mascota, sizeof(struct dogType), 0);
					}
					r = fseek(archivo, mascota.next, SEEK_SET);
					if (r != 0)
					{
						perror("Error fseek");
						exit(-1);
					}
					pos = ftell(archivo);
					r = fread(&mascota, sizeof(struct dogType), 1, archivo);
					if (r = 0)
					{
						perror("Error fread");
						exit(-1);
					}
				}
				if (strcmp(mascota.nombre, key) == 0)
				{
					r = send(clientfd, &pos, sizeof(int), 0);
					r = send(clientfd, &mascota, sizeof(struct dogType), 0);
				}
				r = fclose(archivo);
				if (r < 0)
				{
					perror("Error fclose");
					exit(-1);
				}
			}

			struct dogType ultimo;
			ultimo.nombre[0] = '*';
			int x = -1;
			r = send(clientfd, &x, sizeof(int), 0);
			r = send(clientfd, &ultimo, sizeof(struct dogType), 0);
			r=fprintf(f, "[Fecha %s] [Cliente %s] [Bùsqueda] [Cadena buscada: %s]\n", bufff, clientip, nombre);
			if (r = 0)
			{
				perror("\n-->Error en fprintf: ");
				//exit(-1);
			}
			break;

		case '5':
			break;
		}

	} while (opcion[0] != '5');
	printf("\nExit client \n");
	close(clientfd);
	pthread_exit(NULL);
}


int mostrarHistoria(char nombre[], char id[], int clientfd)
{

	FILE *historia;
	int r;
	char dir[500], cwd[500], buff[100], borrar[500] = "rm ";

	strcpy(dir, "historias_clinicas/");
	strcat(dir, id);
	strcat(dir, "_");
	strcat(dir, nombre);
	strcat(dir, ".txt");

	historia = fopen(dir, "a+");
    if (historia == NULL)
	{
		perror("error fopen ");
		exit(-1);
	}
	fseek( historia, 0, SEEK_END );
	if (ftell( historia ) == 0 )
	{
		r = send(clientfd," ",sizeof(char), 0);
		if(r < 0){
			perror("\n-->Error en send(): ");
			exit(-1);
		}
	}else{
		fseek( historia, 0, SEEK_SET );
		fread(buff,sizeof(buff),1,historia);
		size_t ln = strlen(buff)-1;
		if (buff[ln] == '\n')
			buff[ln] = '\0';
		//printf("BUFF %s",buff);
		r = send(clientfd,buff,sizeof(buff), 0);
			if(r < 0){
				perror("\n-->Error en send(): ");
				exit(-1);
			}
		 
	}
    fclose(historia);


	return 0;
}

int guardarHistoria(char nombre[], char id[], int clientfd)
{

	FILE *historia;
	int r;
	char dir[500], cwd[500], buff[100];

	strcpy(dir, "historias_clinicas/");
	strcat(dir, id);
	strcat(dir, "_");
	strcat(dir, nombre);
	strcat(dir, ".txt");

	historia = fopen(dir, "w");
    if (historia == NULL)
	{
		perror("error fopen");
		exit(-1);
	}
    r = recv(clientfd,buff,100, 0);
	
	//printf("BUFF %s",buff);
	//printf("r %d",r);
	fprintf(historia, "%s", buff);
    fclose(historia);

	return 0;
}
//HASH
unsigned long hash_function(char *str)
{
	unsigned long i = 0;
	int j;
	for (j = 0; str[j]; j++)
		i += str[j];
	return i % CAPACITY;
}

Ht_item *create_item(int key, int value)
{
	// Creates a pointer to a new hash table item
	Ht_item *item = (Ht_item *)malloc(sizeof(Ht_item));
	item->head = key;
	item->tail = value;
	return item;
}

HashTable *create_table(int size)
{
	// Creates a new HashTable
	int i;
	HashTable *table = (HashTable *)malloc(sizeof(HashTable));
	table->size = size;
	table->count = 0;
	table->items = (Ht_item **)calloc(table->size, sizeof(Ht_item *));
	for (i = 0; i < table->size; i++)
		table->items[i] = NULL;
	return table;
}

void free_item(Ht_item *item)
{
	// Frees an item
	free(&item->head);
	free(&item->tail);
	free(item);
}

void free_table(HashTable *table)
{
	// Frees the table
	int i;
	for (i = 0; i < table->size; i++)
	{
		Ht_item *item = table->items[i];
		if (item != NULL)
			free_item(item);
	}
	free(table->items);
	free(table);
}

/*imprimir HASH TABLE (Pruebas)
void print_table(HashTable *table)
{
	int i;
	printf("\nHash Table\n-------------------\n");
	for (i = 0; i < table->size; i++)
		if (table->items[i])
			printf("Index:%d, Key:%d, Value:%d\n", i, table->items[i]->head, table->items[i]->tail);
	printf("-------------------\n\n");
}
*/

HashTable *ht_insert(HashTable *table, char *key, int value)
{
	registros++;
	unsigned long index = hash_function(key);
	Ht_item *current_item = table->items[index];
	if (current_item == NULL)
	{
		// Key does not exist.
		if (table->count == table->size)
		{
			// Hash Table Full
			perror("Insert Error: Hash Table is full\n");
			return NULL;
		}
		// Insert directly
		Ht_item *item = create_item(value, value);
		table->items[index] = item;
		//print_table(table);
		table->count++;
		return table;
	}
	else
	{
		// Scenario 1: We only need to update value
		int posicion = current_item->tail, r;
		FILE *archivo;
		archivo = fopen("dataDogs.dat", "rb+");
		if (archivo == NULL)
		{
			perror("error fopen");
			exit(-1);
		}
		struct dogType mascota;
		r = fseek(archivo, posicion, SEEK_SET);
		if (r != 0)
		{
			perror("Error fseek");
			exit(-1);
		}
		r = fread(&mascota, sizeof(struct dogType), 1, archivo);
		if (r = 0)
		{
			perror("Error fread");
			exit(-1);
		}
		mascota.next = value;
		current_item->tail = value;
		r = fseek(archivo, posicion, SEEK_SET);
		if (r != 0)
		{
			perror("Error fseek");
			exit(-1);
		}
		r = fwrite(&mascota, sizeof(struct dogType), 1, archivo);
		if (r = 0)
		{
			perror("Error fwrite");
			exit(-1);
		}
		r = fclose(archivo);
		if (r < 0)
		{
			perror("Error fclose");
			exit(-1);
		}
		return table;
	}


}

Ht_item *ht_search(HashTable *table, char *key)
{
	// Searches the key in the hashtable
	// and returns NULL if it doesn't exist
	int index = hash_function(key);
	Ht_item *item = table->items[index];
	// Ensure that we move to a non NULL item
	if (item != NULL)
		return item;
	return NULL;
}

HashTable *guardarRegistro(HashTable *table, void *puntero)
{
	FILE *archivo;
	struct dogType *mascota;
	mascota = puntero;
	int r;

	archivo = fopen("dataDogs.dat", "ab+");
	if (archivo == NULL)
	{
		perror("Error fopen");
		exit(-1);
	}
	r = fseek(archivo, 0, SEEK_END);
	if (r != 0)
	{
		perror("Error fseek");
		exit(-1);
	}
	r = fwrite(puntero, sizeof(struct dogType), 1, archivo);
	if (r = 0)
	{
		perror("Error fwrite");
		exit(-1);
	}
	int posicion = ftell(archivo) - sizeof(struct dogType);
	table = ht_insert(table, mascota->nombre, posicion);
	r = fclose(archivo);
	if (r < 0)
	{
		perror("Error fclose");
		exit(-1);
	}
	return table;
}

struct HashTable *hash_db()
{
	
	FILE *ptr;
	HashTable *ht = create_table(CAPACITY);
	struct dogType dog;
	int r;

	ptr = fopen("dataDogs.dat", "rb");
	if (ptr == NULL)
	{
		perror("error fopen");
		return NULL;
	}
	
	int i=0;
	while (fread(&dog, sizeof(struct dogType), 1, ptr) != 0)
	{
		if (dog.nombre[0] != '*')
		{
			int posicion = ftell(ptr) - sizeof(struct dogType);
			ht_insert(ht, dog.nombre, posicion);
			//registros++;
			//i++;
			//printf("numero registros : %d\n", registros);
		}
		
	}
	r = fclose(ptr);
	if (r < 0)
	{
		perror("Error fclose");
		exit(-1);
	}
	return ht;
}
//FIN HASH

int borrarRegistro(int posicion)
{
	FILE *archivo;
	struct dogType dog;
	int r, i;
	archivo = fopen("dataDogs.dat", "r+b");
	if (archivo == NULL)
	{
		perror("Error fopen");
		exit(-1);
	}
	r = fseek(archivo, posicion, SEEK_SET);
	if (r < 0)
	{
		perror("Error fseek");
		exit(-1);
	}

	r = fread(&dog, sizeof(struct dogType), 1, archivo);
	if (r = 0)
	{
		perror("Error fread");
		exit(-1);
	}

	for (i = 0; dog.nombre[i]; i++)
		dog.nombre[i] = ' ';
	dog.nombre[0] = '*';
	dog.edad = 0;
	dog.estatura = 0;
	dog.peso = 0;
	dog.sexo = ' ';
	for (i = 0; dog.tipo[i]; i++)
		dog.tipo[i] = ' ';
	for (i = 0; dog.raza[i]; i++)
		dog.raza[i] = ' ';

	r = fseek(archivo, posicion, SEEK_SET);
	if (r != 0)
	{
		perror("Error fseek");
		exit(-1);
	}
	r = fwrite(&dog, sizeof(struct dogType), 1, archivo);
	if (r = 0)
	{
		perror("Error fwrite");
		exit(-1);
	}
	r = fclose(archivo);
	if (r < 0)
	{
		perror("Error fclose");
		exit(-1);
	}
	return 0;
}

HashTable *delete_item(HashTable *table, char *key, int code)
{
	registros--;
	Ht_item *val = ht_search(table, key);
	if (val == NULL)
	{
		//printf("          La mascota con el ID: %s no esta registrado en la base de datos\n", key);
		return NULL;
	}
	else
	{
		FILE *archivo;
		struct dogType mascota, eliminar;
		int current = val->head, r;

		archivo = fopen("dataDogs.dat", "rb+");
		if (archivo == NULL)
		{
			perror("Error fopen");
			exit(-1);
		}
		r = fseek(archivo, val->head, SEEK_SET);
		if (r != 0)
		{
			perror("Error fseek");
			exit(-1);
		}
		r = fread(&mascota, sizeof(struct dogType), 1, archivo);
		if (r = 0)
		{
			perror("Error fread");
			exit(-1);
		}
		if (code == current)
		{
			val->head = mascota.next;
			return table;
		}
		while (mascota.next != -1)
		{
			if (mascota.next == code)
			{
				r = fseek(archivo, mascota.next, SEEK_SET);
				if (r != 0)
				{
					perror("Error fseek");
					exit(-1);
				}
				r = fread(&eliminar, sizeof(struct dogType), 1, archivo);
				if (r = 0)
				{
					perror("Error fread");
					exit(-1);
				}
				int sig = eliminar.next;
				mascota.next = sig;
				r = fseek(archivo, current, SEEK_SET);
				if (r != 0)
				{
					perror("Error fseek");
					exit(-1);
				}
				r = fwrite(&mascota, sizeof(struct dogType), 1, archivo);
				if (r = 0)
				{
					perror("Error fwrite");
					exit(-1);
				}
				fclose(archivo);
				return table;
			}
			if (mascota.next == val->tail)
			{
				r = fread(&mascota, sizeof(struct dogType), 1, archivo);
				if (r = 0)
				{
					perror("Error fread");
					exit(-1);
				}
				mascota.next = -1;
				r = fseek(archivo, current, SEEK_SET);
				if (r != 0)
				{
					perror("Error fseek");
					exit(-1);
				}
				r = fwrite(&mascota, sizeof(struct dogType), 1, archivo);
				if (r = 0)
				{
					perror("Error fwrite");
					exit(-1);
				}
				val->tail = current;
				fclose(archivo);
				return table;
			}
			r = fseek(archivo, mascota.next, SEEK_SET);
			if (r != 0)
			{
				perror("Error fseek");
				exit(-1);
			}
			current = ftell(archivo);
			r = fread(&mascota, sizeof(struct dogType), 1, archivo);
			if (r = 0)
			{
				perror("Error fread");
				exit(-1);
			}
		}
		
		//printf("          La mascota con el ID: %s no esta registrado en la base de datos\n", key);
		r = fclose(archivo);
		if (r < 0)
		{
			perror("Error fclose");
			exit(-1);
		}
	}
	
}

struct dogType leerEsctructura(int id, struct dogType dog)
{
	int r;
	FILE *ptr;
	ptr = fopen("dataDogs.dat", "rb");
	if (ptr == NULL)
	{
		perror("error fopen");
		exit(-1);
	}
	r = fseek(ptr, id, SEEK_SET);
	if (r != 0)
	{
		perror("Error fseek");
		exit(-1);
	}
	r = fread(&dog, sizeof(struct dogType), 1, ptr);
	if (r = 0)
	{
		perror("Error fread");
		exit(-1);
	}
	r = fclose(ptr);
	if (r < 0)
	{
		perror("Error fclose");
		exit(-1);
	}
	return dog;
}
