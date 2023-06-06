#ifndef  GRAPH_INC
#define  GRAPH_INC

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>

#include "List.h"
#include "Queue.h"

// Aunque en este ejemplo estamos usando tipos básicos, vamos a usar al alias |Item| para resaltar
// aquellos lugares donde estamos hablando de DATOS y no de índices.
typedef int Item;

/**
 * @brief Colores para los vértices.
 */
typedef enum
{
	WHITE, ///< vértice
	GRAY,  ///< vértice
	BLACK, ///< vértice
} eGraphColors;

//----------------------------------------------------------------------
//                           Vertex stuff:
//----------------------------------------------------------------------

/**
 * @brief Declara lo que es un vértice.
 */
typedef struct
{
	Item data;
	List *neighbors;

	int predecessor;
	int distance;
	eGraphColors color;

	int discovery_time;
	int finish_time;
} Vertex;


/**
 * @brief Indica si las aristas tienen un sentido definido
 */
typedef enum{
	eGraphType_UNDIRECTED, ///< grafo no dirigido
	eGraphType_DIRECTED	   ///< grafo dirigido (digraph)
} eGraphType;

/**
 * @brief Declara lo que es un grafo.
 */
typedef struct{
	Vertex *vertices; ///< Lista de vértices
	int size;		  ///< Tamaño de la lista de vértices

	/**
	 * Número de vértices actualmente en el grafo. Como esta versión no
	 * borra vértices, lo podemos usar como índice en la función de inserción.
	*/
	int len;

	eGraphType type; ///< tipo del grafo, UNDIRECTED o DIRECTED
} Graph;

void Vertex_Start(Vertex *v);
void Vertex_Next(Vertex *v);
bool Vertex_End(const Vertex *v);
void Vertex_SetColor(Vertex *v, eGraphColors color);
eGraphColors Vertex_GetColor(Vertex *v);
void Vertex_SetDistance(Vertex *v, int distance);
int Vertex_GetDistance(const Vertex *v);
void Vertex_SetPredecessor(Vertex *v, int predecessor_idx);
int Vertex_GetPredecessor(const Vertex *v);
int Vertex_GetData(const Vertex *v);

Graph *Graph_New(int size, eGraphType type);
void Graph_Delete(Graph **g);
void Graph_Print(Graph *g, int depth);
void Graph_AddVertex(Graph *g, int data);
int Graph_GetSize( Graph* g );
bool Graph_AddEdge(Graph *g, int start, int finish);
int Graph_GetLen( const Graph* g );
Item Graph_GetDataByIndex(const Graph *g, int vertex_idx);
Vertex *Graph_GetVertexByIndex(const Graph *g, int vertex_idx);
Vertex *Graph_GetVertexByKey(const Graph *g, Item key);
int Graph_Size(Graph *g);
Data Vertex_GetNeighborIndex(const Vertex *v);
void bfs( Graph* g, int start );
void Vertex_SetDiscovery_time(Vertex *v, int discovery_time);
int Vertex_GetDiscovery_time(Vertex *v);
void Vertex_SetFinish_time(Vertex *v, int finish_time);
int Vertex_GetFinish_time(Vertex *v);
int dfs_traverse(Graph *g, int start, int time);
void dfs(Graph *g, int start);

#endif