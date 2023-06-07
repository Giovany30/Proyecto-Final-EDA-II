#include <stdio.h>
#include "Graph.h"

#ifndef DBG_HELP
#define DBG_HELP 0
#endif

#if DBG_HELP > 0
#define DBG_PRINT( ... ) do{ fprintf( stderr, "DBG:" __VA_ARGS__ ); } while( 0 )
#else
#define DBG_PRINT( ... ) ;
#endif 

/**
 * @brief Hace que cursor libre apunte al inicio de la lista de vecinos. Se debe
 * de llamar siempre que se vaya a iniciar un recorrido de dicha lista.
 *
 * @param v El vértice de trabajo (es decir, el vértice del cual queremos obtener
 * la lista de vecinos).
 */
void Vertex_Start(Vertex *v){
    assert(v);
	List_Cursor_front(v->neighbors);
}

/**
 * @brief Mueve al cursor libre un nodo adelante.
 *
 * @param v El vértice de trabajo.
 *
 * @pre El cursor apunta a un nodo válido.
 * @post El cursor se movió un elemento a la derecha en la lista de vecinos.
 */
void Vertex_Next(Vertex *v){
	List_Cursor_next(v->neighbors);
}

/**
 * @brief Indica si se alcanzó el final de la lista de vecinos.
 *
 * @param v El vértice de trabajo.
 *
 * @return true si se alcanazó el final de la lista; false en cualquier otro
 * caso.
 */
bool Vertex_End(const Vertex *v){
    return List_Cursor_end(v->neighbors);
}

// Le asignamos un color al vértice (utilizado para los recorridos de grafos).
void Vertex_SetColor(Vertex *v, eGraphColors color){
	v->color = color;
}

// Obtenemos el color de un vértice (utilizado para los recorridos de grafos).
eGraphColors Vertex_GetColor(Vertex *v){
	return v->color;
}

// Le asignamos una distancia para llegar al vértice (utilizado para los recorridos de grafos).
void Vertex_SetDistance(Vertex *v, int distance){
	v->distance = distance;
}

// Obtenemos la distancia para llegar a un vértice (utilizado para los recorridos de grafos).
int Vertex_GetDistance(const Vertex *v){
	return v->distance;
}

// Asignamos el vértice predecesor al vértice de trabajo (utilizado para los recorridos de grafos).
void Vertex_SetPredecessor(Vertex *v, int predecessor_idx){
	v->predecessor = predecessor_idx;
}

// Obtenemos el vértice predecesor al vértice de trabajo (utilizado para los recorridos de grafos).
int Vertex_GetPredecessor(const Vertex *v){
	return v->predecessor;
}

// Devuelve el elemento que estaba guardado en el vértice.
int Vertex_GetData(const Vertex *v){
	return v->data;
}

//----------------------------------------------------------------------
//                     Funciones privadas
//----------------------------------------------------------------------

// vertices: lista de vértices
// size: número de elementos en la lista de vértices
// key: valor a buscar
// ret: el índice donde está la primer coincidencia; -1 si no se encontró
static int find(Vertex vertices[], int size, int key){
	for(int i = 0; i < size; ++i){
		if (vertices[i].data == key)
			return i;
	}

	return -1;
}

// busca en la lista de vecinos si el índice del vértice vecino ya se encuentra ahí
static bool find_neighbor(Vertex *v, int index){
	if (v->neighbors){
		return List_Find(v->neighbors, index);
	}
	return false;
}

// vertex: vértice de trabajo
// index: índice en la lista de vértices del vértice vecino que está por insertarse
static void insert(Vertex *vertex, int index, float weigth){
	// crear la lista si no existe!

	if (!vertex->neighbors){
		vertex->neighbors = List_New();
	}

	if (vertex->neighbors && !find_neighbor(vertex, index)){
		List_Push_back(vertex->neighbors, index, weigth);
		DBG_PRINT("insert():Inserting the neighbor with idx:%d\n", index);
	}
	else
		DBG_PRINT("insert: duplicated index\n");
}

//----------------------------------------------------------------------
//                     Funciones públicas
//----------------------------------------------------------------------

/**
 * @brief Crea un nuevo grafo.
 *
 * @param size Número de vértices que tendrá el grafo. Este valor no se puede
 * cambiar luego de haberlo creado.
 *
 * @return Un nuevo grafo.
 *
 * @pre El número de elementos es mayor que 0.
 */
Graph *Graph_New(int size, eGraphType type){
	assert(size > 0);

	Graph *g = (Graph *)malloc(sizeof(Graph));
	if (g){
		g->size = size;
		g->len = 0;
		g->type = type;

		g->vertices = (Vertex *)calloc(size, sizeof(Vertex));

		if (!g->vertices){
			free(g);
			g = NULL;
		}
	}

	return g;
	// el cliente es responsable de verificar que el grafo se haya creado correctamente
}

void Graph_Delete(Graph **g){
	assert(*g);

	Graph *graph = *g; // para simplificar la notación
	
	for (int i = 0; i < graph->size; ++i){
		Vertex *vertex = &graph->vertices[i];
		// para simplificar la notación.
		// La variable |vertex| sólo existe dentro de este for.

		if (vertex->neighbors){
			List_Delete(&(vertex->neighbors));
		}
	}

	free(graph->vertices);
	free(graph);
	*g = NULL;
}

/**
 * @brief Imprime un reporte del grafo
 *
 * @param g     El grafo.
 * @param depth Cuán detallado deberá ser el reporte (0: lo mínimo)
 */
void Graph_Print(Graph *g, int depth)
{
	for (int i = 0; i < g->len; ++i)
	{
		Vertex *vertex = &g->vertices[i];
		// para simplificar la notación.

		printf("[%d]%d=>", i, vertex->data);
		if (vertex->neighbors)
		{
			for (List_Cursor_front(vertex->neighbors);
				 !List_Cursor_end(vertex->neighbors);
				 List_Cursor_next(vertex->neighbors))
			{

				Data d = List_Cursor_get(vertex->neighbors);
				int neighbor_idx = d.index;

				printf("%d->", g->vertices[neighbor_idx].data);
			}
		}
		printf("Nil\n");
	}
	printf("\n");
}

/**
 * @brief Crea un vértice a partir de los datos reales.
 *
 * @param g     El grafo.
 * @param data  Es la información.
 */
void Graph_AddVertex(Graph *g, int data){
	assert(g->len < g->size);

	Vertex *vertex = &g->vertices[g->len];
	// para simplificar la notación

	vertex->data = data;
	vertex->neighbors = NULL;

	++g->len;
}

int Graph_GetSize( Graph* g )
{
   return g->size;
}

/**
 * @brief Inserta una relación de adyacencia del vértice |start| hacia el vértice |finish|.
 *
 * @param g      El grafo.
 * @param start  Vértice de salida (el dato)
 * @param finish Vertice de llegada (el dato)
 *
 * @return false si uno o ambos vértices no existen; true si la relación se creó con éxito.
 *
 * @pre El grafo no puede estar vacío.
 */
bool Graph_AddEdge(Graph *g, int start, int finish){
	assert(g->len > 0);

	// obtenemos los índices correspondientes:
	int start_idx = find(g->vertices, g->size, start);
	int finish_idx = find(g->vertices, g->size, finish);

	DBG_PRINT("AddEdge(): from:%d (with index:%d), to:%d (with index:%d)\n", start, start_idx, finish, finish_idx);

	if (start_idx == -1 || finish_idx == -1)
		return false;
	// uno o ambos vértices no existen

	insert(&g->vertices[start_idx], finish_idx, 0.0);
	// insertamos la arista start-finish

	if (g->type == eGraphType_UNDIRECTED)
		insert(&g->vertices[finish_idx], start_idx, 0.0);
	// si el grafo no es dirigido, entonces insertamos la arista finish-start

	return true;
}

int Graph_GetLen( const Graph* g )
{
   return g->len;
}

/**
 * @brief Devuelve la información asociada al vértice indicado.
 *
 * @param g          Un grafo.
 * @param vertex_idx El índice del vértice del cual queremos conocer su información.
 *
 * @return La información asociada al vértice vertex_idx.
 */
Item Graph_GetDataByIndex(const Graph *g, int vertex_idx){
	assert(0 <= vertex_idx && vertex_idx < g->len);

	return g->vertices[vertex_idx].data;
}

/**
 * @brief Devuelve una referencia al vértice indicado.
 *
 * Esta función puede ser utilizada con las operaciones @see Vertex_Start(), @see Vertex_End(), @see Vertex_Next().
 *
 * @param g          Un grafo
 * @param vertex_idx El índice del vértice del cual queremos devolver la referencia.
 *
 * @return La referencia al vértice vertex_idx.
 */
Vertex *Graph_GetVertexByIndex(const Graph *g, int vertex_idx){
	assert(0 <= vertex_idx && vertex_idx < g->len);

	return &(g->vertices[vertex_idx]);
}

/**
 * @brief Devuelve una referencia al vértice indicado.
 *
 * Esta función puede ser utilizada con las operaciones @see Vertex_Start(), @see Vertex_End(), @see Vertex_Next().
 *
 * @param g   Un grafo
 * @param key Llave de búsqueda (esto es, el |dato|) del vértice del cual queremos devolver la referencia.
 *
 * @return La referencia al vértice que coincida con key (esto es, con el |dato|).
 */
Vertex *Graph_GetVertexByKey(const Graph *g, Item key){
	for (int i = 0; i < Graph_GetLen(g); ++i){
		if (g->vertices[i].data == key){
			return &(g->vertices[i]);
		}
	}

	return NULL;
}

int Graph_Size(Graph *g){
	return g->size;
}

/**
 * @brief Devuelve el índice del vecino al que apunta actualmente el cursor en la lista de vecinos
 * del vértice |v|.
 *
 * @param v El vértice de trabajo (del cual queremos conocer el índice de su vecino).
 *
 * @return El índice del vecino en la lista de vértices.
 *
 * @pre El cursor debe apuntar a un nodo válido en la lista de vecinos.
 *
 * Ejemplo
 * @code
   Vertex* v = Graph_GetVertexByKey( grafo, 100 );
   for( Vertex_Start( v ); !Vertex_End( v ); Vertex_Next( v ) )
   {
		int index = Vertex_GetNeighborIndex( v );

		Item val = Graph_GetDataByIndex( g, index );

		// ...
   }
   @endcode
   @note Esta función debe utilizarse únicamente cuando se recorra el grafo con las funciones
   Vertex_Start(), Vertex_End() y Vertex_Next().
 */
Data Vertex_GetNeighborIndex(const Vertex *v){
	return List_Cursor_get(v->neighbors);
}

//Recorrido en anchura.
void bfs(Graph *g, int start){
	//Inicializamos los respectivos campos de todos los vértices del grafo.
	for(int i = 0; i < Graph_GetLen(g); ++i){
		Vertex *v = Graph_GetVertexByIndex(g, i);
		Vertex_SetColor(v, WHITE);    //No descubierto.
		Vertex_SetDistance(v, 0);     //Sin distancia.
		Vertex_SetPredecessor(v, -1); //Sin predecesor.
	}

	Vertex_SetColor(Graph_GetVertexByKey(g, start), GRAY);

	Queue *q = Queue_New(50);

	Queue_Enqueue(q, start);
	while (!Queue_IsEmpty(q)){
		int val = Queue_Dequeue(q);
		Vertex *v = Graph_GetVertexByKey(g, val);

		//fprintf(stderr, "Processing vertex: %d: ", Vertex_GetData(v));

		for (Vertex_Start(v); !Vertex_End(v); Vertex_Next(v)){
			Vertex *w = Graph_GetVertexByIndex(g, Vertex_GetNeighborIndex(v).index);

			if (Vertex_GetColor(w) == WHITE){
				fprintf(stderr, "%d->", Vertex_GetData(w));

				Vertex_SetColor(w, GRAY);
				Vertex_SetDistance(w, Vertex_GetDistance(v) + 1);
				Vertex_SetPredecessor(w, Vertex_GetData(v));

				Queue_Enqueue(q, Vertex_GetData(w));
			}
		}
		//fprintf(stderr, "\n");

		Vertex_SetColor(v, BLACK);
	}

	Queue_Delete(&q);
}

void Vertex_SetDiscovery_time(Vertex *v, int discovery_time){
	v->discovery_time = discovery_time;
}

int Vertex_GetDiscovery_time(Vertex *v){
	return v->discovery_time;
}

void Vertex_SetFinish_time(Vertex *v, int finish_time){
	v->finish_time = finish_time;
}

int Vertex_GetFinish_time(Vertex *v){
	return v->finish_time;
}

void dfs(Graph *g, int start){
	//Inicializamos los respectivos campos de todos los vértices del grafo.
	for (int i = 0; i < Graph_GetLen(g); ++i){
		Vertex *v = Graph_GetVertexByIndex(g, i);

		Vertex_SetColor(v, WHITE);       //No descubierto.
		Vertex_SetPredecessor(v, -1);    //Sin predecesor.
		Vertex_SetDiscovery_time(v, 0);  //Sin tiempo de descubrimiento.
		Vertex_SetFinish_time(v, 0);     //Sin tiempo de finalización.
	}

	fprintf(stderr, "Visiting vertex: %d\n", start);

	int time = 0;

	Vertex *v = Graph_GetVertexByKey(g, start);
	Vertex_SetDiscovery_time(v, time + 1); //Ponemos tiempo de descubrimiento.

	time = dfs_traverse(g, start, time);
}

int dfs_traverse(Graph *g, int start, int time)
{
	++time;

	Vertex *v = Graph_GetVertexByKey(g, start);

	Vertex_SetDiscovery_time(v, time);
	Vertex_SetColor(v, GRAY);

	for (Vertex_Start(v); !Vertex_End(v); Vertex_Next(v))
	{
		Vertex *w = Graph_GetVertexByIndex(g, Vertex_GetNeighborIndex(v).index);

		if (Vertex_GetColor(w) == WHITE)
		{
			fprintf(stderr, "Visiting vertex: %d\n", Vertex_GetData(w));

			Vertex_SetPredecessor(w, Vertex_GetData(v));

			time = dfs_traverse(g, Vertex_GetData(w), time);
		}
	}

	Vertex_SetColor(v, BLACK);
	++time;
	Vertex_SetFinish_time(v, time);

	return time;
}