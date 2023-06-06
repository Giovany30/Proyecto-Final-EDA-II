/*Copyright (C) 
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 * 
 * 2023 - francisco dot rodriguez at ingenieria dot unam dot mx
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>

#include "Graph.h"

// 29/03/23:
// Esta versión no borra elementos
// Esta versión no modifica los datos originales


#define MAX_VERTICES 5

int main(){
	Graph* grafo = Graph_New( 
                     MAX_VERTICES,            // cantidad máxima de vértices
                     eGraphType_UNDIRECTED ); // será un grafo no dirigido


	// crea los vértices. El orden de inserción no es importante
	Graph_AddVertex( grafo, 100 );
	Graph_AddVertex( grafo, 200 );
	Graph_AddVertex( grafo, 300 );
	Graph_AddVertex( grafo, 400 );
	Graph_AddVertex( grafo, 500 );

	// crea las aristas (conexiones entre vértices):
	Graph_AddEdge( grafo, 100, 200 );
	Graph_AddEdge( grafo, 100, 400 );
	Graph_AddEdge( grafo, 200, 300 );
	Graph_AddEdge( grafo, 200, 500 );
	Graph_AddEdge( grafo, 300, 500 );
	Graph_AddEdge( grafo, 400, 500 );


	Graph_Print( grafo, 0 );
	// imprime el grafo completo (esta versión no usa al segundo argumento)


	bfs( grafo, 100 );
	// lleva a cabo el recorrido en anchura

	for( int i = 0; i < Graph_GetLen( grafo ); ++i ){
		Vertex* v = Graph_GetVertexByIndex( grafo, i );

		printf( "[%d] (%d) -- Pred: %d, Dist: %d\n", 
 			i,
            Vertex_GetData( v ),
            Vertex_GetPredecessor( v ),
            Vertex_GetDistance( v ) );
	}

	Graph_Delete( &grafo );
	assert( grafo == NULL );
	return 0;
}