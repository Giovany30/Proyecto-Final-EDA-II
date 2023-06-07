#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>

//#include "List.h"
#include "Queue.h"
#include "Graph.h"
#include "Refugio.h"

int main(){
	Refugio* r = Refugio_New();
	Refugio_Menu(r);
	return 0;
}
