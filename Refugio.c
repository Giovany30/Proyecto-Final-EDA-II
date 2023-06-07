#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include "cJSON.h"
#include <omp.h>
#include "Refugio.h"

static Sucursal Sucursales_Get(int id){
    Sucursal s;
    strcpy(s.estado, "CDMX");
    s.len = 0;
    if(id == 1){
        s.id = 1;
        strncpy(s.ubication, "Copilco", 32);
    }
    else if(id == 2){
        s.id = 2;
        strncpy(s.ubication, "Universidad", 32);
    }
    else if(id == 3){
        s.id = 3;
        strncpy(s.ubication, "Coyoacán", 32);
    }
    else if(id == 4){
        s.id = 4;
        strncpy(s.ubication, "Indios Verdes", 32);
    }
    return s;
}

static void Sucursales_Set(Refugio* r){
    Graph* g = r->sucursales; //Para simplificar la notación
    Graph_AddVertex(g, 1); 
    Graph_AddVertex(g, 2);
    Graph_AddVertex(g, 3);
    Graph_AddVertex(g, 4);

    Graph_AddEdge(g, 1, 4);
    Graph_AddEdge(g, 2, 4);
    Graph_AddEdge(g, 1, 3);
}

static void Productos_Set(Product p[]){
    p[0].barcode = 6;
    p[0].price = 35;
    strcpy(p[0].name, "Croquetas chicas");
    p[1].barcode = 7;
    p[1].price = 85;
    strcpy(p[1].name, "Croquetas medianas");
    p[2].barcode = 1;
    p[2].price = 185;
    strcpy(p[2].name, "Croquetas grandes");
    p[3].barcode = 6;
    p[3].price = 35;
    strcpy(p[3].name, "Croquetas gigantes");
    p[4].barcode = 9;
    p[4].price = 225;
    strcpy(p[4].name, "Croquetas de gato grandes");
    p[5].barcode = 8;
    p[5].price = 125;
    strcpy(p[5].name, "Croquetas de gato medianas");
    p[6].barcode = 5;
    p[6].price = 65;
    strcpy(p[6].name, "Croquetas de gato chicas");
    p[7].barcode = 15;
    p[7].price = 18;
    strcpy(p[7].name, "Bolsa de semillas");
    p[8].barcode = 20;
    p[8].price = 40;
    strcpy(p[8].name, "Comida de peces chica");
    p[9].barcode = 19;
    p[9].price = 80;
    strcpy(p[9].name, "Comida de peces mediana");
    p[10].barcode = 12;
    p[10].price = 130;
    strcpy(p[10].name, "Comida de peces grande");
}

static void Product_Print(Product p[]){
    for(int i = 0; i < 11; ++i){
        printf("\nBarcode: %d.  Price: %d.  Name: %s.", p[i].barcode, p[i].price, p[i].name);
    }
}

void Product_Swap(Product* p1, Product* p2){
    Product tmp;
    tmp.barcode = p1->barcode;
    tmp.price = p1->price;
    strcpy(tmp.name, p1->name);

    p1->barcode = p2->barcode;
    p1->price = p2->price;
    strcpy(p1->name, p2->name);

    p2->barcode = tmp.barcode;
    p2->price = tmp.price;
    strcpy(p2->name, tmp.name);
}

static void Product_QuickSortByBarcode(Product p[], int first, int last){
    int x0 = first;
    int x1 = last;
    int mid = (first + last) / 2;
    int piv = p[mid].barcode;  //Nuestro pivote será el elemento de en medio

    while(x0 <= x1){
        
        while(p[x0].barcode < piv){
            ++x0;
        }
        while(p[x1].barcode > piv){
            --x1;
        }
        if(x0 <= x1){
            Product_Swap(&p[x0], &p[x1]);
            ++x0;
            --x1;
        }
    }
    
    if(first < x1){
        Product_QuickSortByBarcode(p, first, x1);
    }
    if(x0 < last){
        Product_QuickSortByBarcode(p, x0, last);
    }
}

static void Product_QuickSortByPrice(Product p[], int first, int last){
    int x0 = first;
    int x1 = last;
    int mid = (first + last) / 2;
    int piv = p[mid].price;  //Nuestro pivote será el elemento de en medio

    while(x0 <= x1){
        
        while(p[x0].price < piv){
            ++x0;
        }
        while(p[x1].price > piv){
            --x1;
        }
        if(x0 <= x1){
            Product_Swap(&p[x0], &p[x1]);
            ++x0;
            --x1;
        }
    }
    
    if(first < x1){
        Product_QuickSortByPrice(p, first, x1);
    }
    if(x0 < last){
        Product_QuickSortByPrice(p, x0, last);
    }
}

static void Product_Sort(Refugio* r){
    char opcionStr[10];
    int option;
    printf("\nEliga el modo de muestro [ 1)CÓDIGO DE BARRAS, 2)PRECIO ]: ");
    fgets(opcionStr, sizeof(opcionStr), stdin);
    option = atoi(opcionStr);

    if(option == 1) Product_QuickSortByBarcode(r->inventory, 0, 11);
    else if(option == 2) Product_QuickSortByPrice(r->inventory, 0, 11);
}

static void User_Registro(User* user){
    printf("\nRegistro del usuario.");
    printf("\nIngrese su nombre completo: ");
    fgets(user->name, sizeof(user->name), stdin);
    printf("Ingresa su fecha de nacimiento [DD-MM-YYYY]: ");
    fgets(user->date, sizeof(user->date), stdin);
    srand(time(NULL));
    user->id = rand() % 9001 + 1000;
}

static void User_Swap(User* u1, User* u2){
    User tmp;
    strcpy(tmp.date, u1->date);
    strcpy(tmp.name, u1->name);
    tmp.id = u1->id;

    strcpy(u1->date, u2->date);
    strcpy(u1->name, u2->name);
    u1->id = u2->id;

    strcpy(u2->date, tmp.date);
    strcpy(u2->name, tmp.name);
    u2->id = tmp.id; 
    //fprintf(stderr, "\nu1 = %s, id = %ld, u2 = %s, id = %ld\n", u1->name, u1->id, u2->name, u2->id);
}

//Revisado
void User_QuickSortByID(User users[], int first, int last){
    int x0 = first;
    int x1 = last;
    int mid = (first + last) / 2;
    int piv = users[mid].id;  //Nuestro pivote será el elemento de en medio

    while(x0 <= x1){
        while(users[x0].id < piv){
            ++x0;
        }
        while(users[x1].id > piv){
            --x1;
        }
        if(x0 <= x1){
            User_Swap(&users[x0], &users[x1]);
            ++x0;
            --x1;
        }
    }
    
    if(first < x1){
        User_QuickSortByID(users, first, x1);
    }
    if(x0 < last){
        User_QuickSortByID(users, x0, last);
    }
}


static User User_BusquedaBinariaRecByID(User users[], int inf, int sup, int key){
	int centro = ((sup + inf)/2);
    User centinela = 
    {
        .date = "NULL",
        .id = -1,
        .name = "NULL",
    };
	
	if(users[centro].id == key){
		return users[centro];
	}

	if(sup == inf){
		return centinela;
	}
	
	if(key < users[centro].id){
		User_BusquedaBinariaRecByID(users, inf, centro, key);
	}
	else{
		User_BusquedaBinariaRecByID(users, centro + 1, sup, key);
	}

    return centinela;
}


static void Animal_Registro(Animal* a){
    char opcionStr[10];
    int option;
    printf("\nRegistro de la mascota.");

    size_t len = strlen(a->especie);
    if (len > 0 && a->especie[len - 1] == '\n') {
        a->especie[len - 1] = '\0'; // Reemplazar '\n' por el terminador nulo '\0'
    }

    printf("\nIngrese la especie del animal: ");
    fgets(a->especie, sizeof(a->especie), stdin);

    len = strlen(a->especie);
    if (len > 0 && a->especie[len - 1] == '\n') {
        a->especie[len - 1] = '\0'; // Reemplazar '\n' por el terminador nulo '\0'
    }

    printf("Ingrese el nombre completo: ");
    fgets(a->name, sizeof(a->name), stdin);

    // Eliminar el carácter de nueva línea si está presente
    len = strlen(a->name);
    if (len > 0 && a->name[len - 1] == '\n') {
        a->name[len - 1] = '\0'; // Reemplazar '\n' por el terminador nulo '\0'
    }

    printf("Ingrese el tamaño del animal [ 1)CHICO, 2)MEDIANO, 3)GRANDE ]: ");
    fgets(opcionStr, sizeof(opcionStr), stdin);
    option = atoi(opcionStr);
    if(option == 1) a->t = CHICO;
    else if(option == 2) a->t = MEDIANO;
    else if(option == 3) a->t = GRANDE;
    printf("Ingresa la fecha de nacimiento [DD-MM-YYYY]: ");
    fgets(a->date, sizeof(a->date), stdin);

    len = strlen(a->date);
    if (len > 0 && a->date[len - 1] == '\n') {
        a->date[len - 1] = '\0'; 
    }

    srand(time(NULL));
    a->id = rand() % 9001 + 1000;
}

Refugio* Refugio_New(){
    Refugio* r = (Refugio*) malloc(sizeof(Refugio));
    r->users = malloc(sizeof(User) * 50); 
    r->animals = malloc(sizeof(Animal) * 100);
    r->cursor_u = 0;
    r->cursor_a = 0;

    r->sucursales = Graph_New(10, eGraphType_UNDIRECTED);

    return r;
}

bool Refugio_Search(Refugio* r, int id, User* u){
    User_QuickSortByID(r->users, 0, r->cursor_u - 1);
    *u = User_BusquedaBinariaRecByID(r->users, 0, r->cursor_u, id);
    if(u->id == -1){
        return false;
    }
    else {
        return true;
    }
}

static void Animal_Serialize(Refugio* r, char* f_name){
    
    Animal* a = r->animals;
    FILE* json_output = fopen(f_name, "w");
    
    if(!json_output){
        printf("Error abriendo archivo\n");
        exit(1);
    }
    
    fprintf(json_output, "{\n");

    fprintf(json_output, "\"animales\": [\n");
    for(size_t i = 0; i < r->cursor_a; ++i){
        if(i > 0) fprintf(json_output, ",\n");
        fprintf(json_output, "{ ");
        fprintf(json_output, "\"name\": \"%s\", \"nacimiento\": \"%s\", \"especie\": \"%s\", \"tamaño\": %d, \"id\": %ld",
            a[i].name,
            a[i].date,
            a[i].especie,
            a[i].t,
            a[i].id);
        fprintf(json_output, " }");
    }

    fprintf(json_output, "\n]");
    //fprintf(json_output, ",\n"); //descomentar si todavÃ­a hay mÃ¡s campos

    fprintf(json_output, "\n}");
    fclose(json_output);
}

Animal* Animal_Deserialize(char* f_name){
    FILE* file = fopen(f_name, "r");
    
    if(!file){
        printf("Error abriendo archivo\n");
        exit(1);
    }

    // Obtener el tamaño del archivo
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    // Leer el contenido del archivo en una cadena
    char* json_data = (char*)malloc(file_size + 1);
    fread(json_data, 1, file_size, file);
    json_data[file_size] = '\0';

    // Analizar la cadena 
    cJSON* root = cJSON_Parse(json_data);
    if (root == NULL) {
        printf("Error al analizar el archivo JSON.\n");
        free(json_data);
        fclose(file);
        exit(1);
    }

    // Obtener el arreglo "animales"
    cJSON* animales_array = cJSON_GetObjectItem(root, "animales");
    if (animales_array == NULL) {
        printf("No se pudo obtener el arreglo 'animales' del JSON.\n");
        cJSON_Delete(root);
        free(json_data);
        fclose(file);
        exit(1);
    }

    // Obtener el número de elementos en el arreglo
    int num_animales = cJSON_GetArraySize(animales_array);
    Animal* a = (Animal *)malloc(num_animales * sizeof(Animal));

    //Obtener los datos de cada campo
    for(int i = 0; i < num_animales; i++){
        cJSON* animal_obj = cJSON_GetArrayItem(animales_array, i);
        strncpy(a[i].name, cJSON_GetObjectItem(animal_obj, "name")->valuestring, sizeof(a[i].name) - 1);
        a[i].name[sizeof(a[i].name) - 1] = '\0'; // Asegurar que el array de caracteres está terminado correctamente

        strncpy(a[i].especie, cJSON_GetObjectItem(animal_obj, "especie")->valuestring, sizeof(a[i].especie) - 1);
        a[i].especie[sizeof(a[i].especie) - 1] = '\0'; // Asegurar que el array de caracteres está terminado correctamente

        strncpy(a[i].date, cJSON_GetObjectItem(animal_obj, "nacimiento")->valuestring, sizeof(a[i].date) - 1);
        a[i].date[sizeof(a[i].date) - 1] = '\0'; // Asegurar que el array de caracteres está terminado correctamente

        if(cJSON_GetObjectItem(animal_obj, "tamaño")->valueint == 1){
            a[i].t = CHICO;
        }
        else if(cJSON_GetObjectItem(animal_obj, "tamaño")->valueint == 2){
            a[i].t = MEDIANO;
        }
        else if(cJSON_GetObjectItem(animal_obj, "tamaño")->valueint == 3){
            a[i].t = GRANDE;
        }

        a[i].id = cJSON_GetObjectItem(animal_obj, "id")->valueint;

    }
    a[num_animales].id = -1;

    cJSON_Delete(root);
    free(json_data);

    return a;
}

void Refugio_Load(Refugio* r, Animal** a){
    int i = 0;
    while((*a)[i].id != -1){
        strncpy(r->animals[i].name, (*a)[i].name, sizeof((*a)[i].name) - 1);
        strncpy(r->animals[i].especie, (*a)[i].especie, sizeof((*a)[i].especie) - 1);
        strncpy(r->animals[i].date, (*a)[i].date, sizeof((*a)[i].date) - 1);
        r->animals[i].id = (*a)[i].id;
        r->animals[i].t = (*a)[i].t;
        r->cursor_a += 1;
        ++i;
    }
    free(*a);
    *a = NULL;
}

void Refugio_Registro(Refugio* r){
    char opcionStr[10];
    int option;
    printf("\nYa está registrado? [ 1)si | 2)no ] : ");
    fgets(opcionStr, sizeof(opcionStr), stdin);
    option = atoi(opcionStr);
    if(option == 1){
        char idStr[10];
        int id;
        printf("\nIngrese su ID para identificarlo: ");
        fgets(idStr, sizeof(idStr), stdin);
        id = atoi(idStr);
        User u;
        bool res = Refugio_Search(r, id, &u);
        if(res == true){
            printf("Efectivamente, usted ya está registrado");
            Animal_Registro(&r->animals[r->cursor_a]);
            printf("ID de la mascota: %ld", r->animals[r->cursor_a].id);
            r->cursor_a += 1;
        }
        else {
            printf("Usted no está registrado");
            User_Registro(&r->users[r->cursor_u]);
            printf("ID del usuario: %ld", r->users[r->cursor_u].id);
            Animal_Registro(&r->animals[r->cursor_a]);
            printf("ID de la mascota: %ld", r->animals[r->cursor_a].id);
            r->cursor_a += 1;
            r->cursor_u += 1;
        }
    }
    else if(option == 2){
        User_Registro(&r->users[r->cursor_u]);
        printf("ID del usuario: %ld", r->users[r->cursor_u].id);
        Animal_Registro(&r->animals[r->cursor_a]);
        printf("ID de la mascota: %ld", r->animals[r->cursor_a].id);
        r->cursor_a += 1;
        r->cursor_u += 1;
    }
    Animal_Serialize(r, "animales.json");
}

void Refugio_Delete(Refugio **r){
    assert(*r);

    Refugio *refugio = *r; // para simplificar la notación

    Graph_Delete(&(refugio->sucursales));

    free(refugio->animals);
    free(refugio->users);
    free(refugio);
	*r = NULL;
}

static void Animal_Print(Refugio* r){
    for(int i = 0; i < r->cursor_a; ++i){
        printf("\nNAME: %s.", r->animals[i].name);
        printf(" ESPECIE: %s.", r->animals[i].especie);
        printf(" NACIMIENTO: %s.", r->animals[i].date);
        printf(" ID: %ld", r->animals[i].id);
    }
}

static void Refugio_Disponibilidad(Refugio* r){
    printf("\nLas mascotas disponibles son:");
    Animal_Print(r);

    printf("\nLas sucursales disponibles son:");
    bfs(r->sucursales, 1);
    for(int i = 0; i < Graph_GetLen(r->sucursales); ++i ){
		Vertex* v = Graph_GetVertexByIndex(r->sucursales, i );
		printf( "\n[POS = %d] (Sucursal ID = %d) -- Distancia por recorrer: %d", 
 			i,
            Vertex_GetData( v ),
            Vertex_GetDistance( v ) );
	}

    char opcionStr[10];
    int option;
    printf("\nIngrese el ID (válido) de la mascota que desea: ");
    fgets(opcionStr, sizeof(opcionStr), stdin);
    option = atoi(opcionStr);
    Animal a;
    for(int i = 0; i < r->cursor_a; ++i){
        if(r->animals[i].id == option){
            strncpy(a.name, r->animals[i].name, 32);
        }
    }
    printf("\nIngrese el ID de la sucursal que desea. ");
    printf("Sugerencia, considere la más cercana: ");
    fgets(opcionStr, sizeof(opcionStr), stdin);
    option = atoi(opcionStr);
    Sucursal s = Sucursales_Get(option);

    printf("\nLa mascota %s será adoptada en la sucursal %s", a.name, s.ubication);
}

static void Refugio_Options(Refugio* r, int option){
    switch (option){
        case 1:
            Sucursales_Set(r);
            Refugio_Disponibilidad(r);
            break;
        case 2:
            Animal* a = Animal_Deserialize("animales.json");
            Refugio_Load(r, &a);
            printf("\nArchivo cargado");
            break;
        case 3:
            Refugio_Registro(r);
            break;
        case 4:
            Productos_Set(r->inventory);
            Product_Sort(r);
            Product_Print(r->inventory);
            break;
        case 5:
            Refugio_Delete(&r);
            break;
        default:
            break;
    }
}

void Refugio_Menu(Refugio* r){
    char opcionStr[10];
    int option;
    
    do {
        printf("\nBienvenido, cómo podemos ayudarle:");
        printf("\n  1) Buscar una mascota para adoptar");
        printf("\n  2) Cargar archivo de mascotas disponibles");
        printf("\n  3) Quiero donar una mascota");
        printf("\n  4) Mostrar productos");
        printf("\n  5) Salir");
        
        printf("\n >> Opción: ");
        fgets(opcionStr, sizeof(opcionStr), stdin);
        option = atoi(opcionStr);

        Refugio_Options(r, option);

    } while(option != 5);
}
