#ifndef  REFUGIO_INC
#define  REFUGIO_INC

#include <stdio.h>
#include <time.h>
#include <stdbool.h>

typedef struct {
    int barcode;
    int price;
    char name[32];
} Product;


typedef enum {
	CHICO, 
	MEDIANO,  
	GRANDE, 
} tamanio;

typedef struct {
    char name[32];
    char date[10];
    char especie[32];
    tamanio t;
    long int id;
} Animal; 

//Es la información de la persona que ingresa al sistema
typedef struct {
    char name[32];
    char date[10];
    long int id;
} User;

//Representa un solo local de todos los que pertenecen al refugio
typedef struct {
    char name[32];
} Unidad;

//Conecta a las unidades con los usuarios
typedef struct {
    User* users;     //Es la lista de usuarios registrados
    Animal* animals; //Es la lista de mascotas registradas
    int cursor_u;    //Nos indica la posición en la que insertaremos el último usuario
    int cursor_a;    //Nos indica la posición en la que insertaremos la última mascota
} Refugio;


Refugio* Refugio_New();
void Refugio_Registro(Refugio* r);
bool Refugio_Search(Refugio* r, int id, User* u);
void Refugio_Delete(Refugio **r);
void Refugio_Menu(Refugio* r);
void User_QuickSortByID(User users[], int first, int last);

#endif
