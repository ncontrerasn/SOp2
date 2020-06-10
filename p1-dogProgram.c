//Version Final - Progress

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <stdio_ext.h>
#include <termios.h>
#include <math.h>

#define CAPACITY 5000

int registros = 0;

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

//Hash Table

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

unsigned long hash_function(char *str);

Ht_item *create_item(int key, int value);

HashTable *create_table(int size);

void free_item(Ht_item *item);

void free_table(HashTable *table);

void print_table(HashTable *table);

HashTable *ht_insert(HashTable *table, char *key, int value);

Ht_item *ht_search(HashTable *table, char *key);

HashTable *guardarRegistro(HashTable *table, void *puntero);

HashTable *delete_item(HashTable *table, char *key, int code);

//Progress
struct HashTable *hash_db();

//Progress
unsigned long fsize(char *file);

int menu(int opcion);

int verMascota(struct dogType mascota);

void print_search(HashTable *table, char *key);

int registrarMascota(void *puntero);

int mostrarHistoria(char nombre[], char id[]);

struct dogType leerEsctructura(int id, struct dogType dog);

int borrarRegistro(int posicion);

char getch(void);

int main(int argc, char **argv)
{
	FILE *ptr;
	HashTable *ht=hash_db();;// = create_table(CAPACITY);
	//ht = 
	int opcion, validacion, id, i, h2;
	struct dogType *mascota, dog;
	char nombre[32], historia, charId[12];
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
			validacion = registrarMascota(mascota);
			{
				int i;
				struct dogType *mascota;
				//mascota = puntero;
				printf("\n          Nombre: ");
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
				scanf(" %c", &mascota->sexo);
				mascota->next = -1;
				return 0;
			}
			//cota((void *)mascota);
			if (validacion != 0)
			{
				perror("Error registrando");
				exit(-1);
			}
			ht = guardarRegistro(ht, (void *)mascota);
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
			dog = leerEsctructura(id, dog);
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
			break;
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

//Progress
unsigned long fsize(char *file)
{
	FILE *f = fopen(file, "r");
	fseek(f, 0, SEEK_END);
	unsigned long len = (unsigned long)ftell(f);
	fclose(f);
	return len;
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
	Ht_item *item = (Ht_item *)malloc(sizeof(Ht_item));
	item->head = key;
	item->tail = value;
	return item;
}

HashTable *create_table(int size)
{
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
	free(&item->head);
	free(&item->tail);
	free(item);
}

void free_table(HashTable *table)
{
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

void print_table(HashTable *table)
{
	int i;
	printf("\nHash Table\n-------------------\n");
	for (i = 0; i < table->size; i++)
		if (table->items[i])
			printf("Index:%d, Key:%d, Value:%d\n", i, table->items[i]->head, table->items[i]->tail);
	printf("-------------------\n\n");
}

HashTable *ht_insert(HashTable *table, char *key, int value)
{
	registros++;
	unsigned long index = hash_function(key);
	Ht_item *current_item = table->items[index];
	if (current_item == NULL)
	{
		if (table->count == table->size)
		{
			printf("Insert Error: Hash Table is full\n");
			return NULL;
		}
		Ht_item *item = create_item(value, value);
		table->items[index] = item;
		//print_table(table);
		table->count++;
		return table;
	}
	else
	{
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
	int index = hash_function(key);
	Ht_item *item = table->items[index];
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

HashTable *delete_item(HashTable *table, char *key, int code)
{
	registros--;
	Ht_item *val = ht_search(table, key);
	if (val == NULL)
	{
		printf("          La mascota con el ID: %s no esta registrado en la base de datos\n", key);
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
		printf("          La mascota con el ID: %s no esta registrado en la base de datos\n", key);
		r = fclose(archivo);
		if (r < 0)
		{
			perror("Error fclose");
			exit(-1);
		}
	}
}

//Progress
struct HashTable *hash_db()
{
	int sz = fsize("dataDogs.dat");
	sz = sz / sizeof(struct dogType);
	printf("Cargando base de datos:\n");
	FILE *ptr;
	HashTable *ht = create_table(CAPACITY);
	struct dogType dog;
	int r;
	int count = 0;
	intptr_t value = 0;
	float pcg = 0;

	ptr = fopen("dataDogs.dat", "rb");
	if (ptr == NULL)
	{
		perror("error fopen");
		return NULL;
	}
	while (fread(&dog, sizeof(struct dogType), 1, ptr) != 0)
	{
		if (dog.nombre[0] != '*')
		{
			//Progress
			/*pcg = round((float)count * 100 / 10000001);
			if (value == pcg)
			{
				printf("\rCargando %.2f  %%", pcg);
				fflush(stdout);
				value = 0;
			}
			else
			{
				value = pcg + 1;
			}*/
			//Progress end
			int posicion = ftell(ptr) - sizeof(struct dogType);
			ht_insert(ht, dog.nombre, posicion);
			count++;
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

void print_search(HashTable *table, char *key)
{
	Ht_item *val = ht_search(table, key);
	if (val == NULL)
	{
		printf("          La mascota con el nombre: %s no esta registrado en la base de datos\n", key);
		return;
	}

	else
	{
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
		int i = 0;
		int j = 0;
		while (mascota.next != -1)
		{
			j++;
			if (strcmp(mascota.nombre, key) == 0)
			{
				printf("\n          ID: %d\n", pos);
				verMascota(mascota);
			}
			else
			{
				i++;
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
		j++;
		if (strcmp(mascota.nombre, key) == 0)
		{
			printf("\n          ID: %d\n", pos);
			verMascota(mascota);
		}
		else
		{
			i++;
		}

		if (i == j)
		{
			printf("          La mascota con el nombre: %s no esta registrado en la base de datos\n", key);
			return;
		}
		r = fclose(archivo);
		if (r < 0)
		{
			perror("Error fclose");
			exit(-1);
		}
	}
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
	printf("          Edad:");
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

int mostrarHistoria(char nombre[], char id[])
{

	FILE *historia;
	int r;
	char dir[500] = "cd ", cwd[500];

	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("error getcwd()");
		exit(-1);
	}

	strcat(dir, "historias_clinicas");
	dir[23] = 92;
	strcat(dir, " && gedit ");
	strcat(dir, id);
	strcat(dir, "_");
	strcat(dir, nombre);
	strcat(dir, ".txt");

	system(dir);

	return 0;
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
