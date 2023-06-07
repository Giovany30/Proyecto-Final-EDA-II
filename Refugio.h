#ifndef  REFUGIO_INC
#define  REFUGIO_INC

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#include "Graph.h"

/**
 * @brief Representa la dimensión del volumen del animal
 */
typedef enum {
	CHICO, 
	MEDIANO,  
	GRANDE, 
} tamanio;

/**
 * @brief Son todos los datos que caracterizan al animal
 */
typedef struct {
    char name[32];
    char date[10];
    char especie[32];
    tamanio t;
    long int id;
} Animal; 

/**
 * @brief Representa un solo local de todos los que pertenecen al refugio
 */
typedef struct {
    int id; //Clave única que identifica al local
    int len; //Número actual de animales disponibles
    char estado[32]; //Nombre de un estado perteneciente a México
    char ubication[32]; //Ubicación dentro de un estado
    Animal a; //Lista de animales disponibles
} Sucursal;

typedef struct {
    int barcode;
    int price;
    char name[32];
} Product;

//Es la información de la persona que ingresa al sistema
typedef struct {
    char name[32];
    char date[10];
    long int id;
} User;

//Conecta a las unidades con los usuarios
typedef struct {
    User* users;     //Es la lista de usuarios registrados
    Animal* animals; //Es la lista de mascotas registradas
    int cursor_u;    //Nos indica la posición en la que insertaremos el último usuario
    int cursor_a;    //Nos indica la posición en la que insertaremos la última mascota
    Product inventory[11]; //Los productos disponibles
    Graph* sucursales; //El grafo guarda los id's de las sucursales para su identificación;
} Refugio;


Refugio* Refugio_New();
void Refugio_Registro(Refugio* r);
bool Refugio_Search(Refugio* r, int id, User* u);
void Refugio_Delete(Refugio **r);
void Refugio_Menu(Refugio* r);
void User_QuickSortByID(User users[], int first, int last);
Animal* Animal_Deserialize(char* f_name);
void Refugio_Load(Refugio* r, Animal** a);
bool User_BusquedaBinariaByID(User users[], int n, int key);

#endif
