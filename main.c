#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "grilla.h"

int cant_fotones = 1;
int dist_MAX = 2;//puede ser float?
int dim_X = 2;
int dim_Y = 2;
float delta = 1;
int flag_mostrar = 0;
casilla_t **grilla;

void getFlags(int argc, char **argv){
    /*
    if( argc < 5 || argc > 6){
        printf("Error: Cantidad de argumentos invalida\n");
		return;
    }*/
    int option;
    while( (option = getopt( argc, argv, "n:L:X:Y:d:b")) != -1){
        switch (option) {
            case 'n':
				cant_fotones = atoi(optarg);
                break;
            case 'L':
				dist_MAX = atoi(optarg);
                break;
            case 'X':
                dim_X = atoi(optarg);
                break;
            case 'Y':
                dim_Y = atoi(optarg);
                break;
            case 'd':
                delta = atof(optarg);
                break;
            case 'b':
                flag_mostrar = 1;
                break;
            default:
                break;
        }
    }
}

int main(int argc, char **argv) {
    getFlags(argc, argv); // obtiene flags
    grilla = grilla_create(dim_X, dim_Y); // crea una grilla
    print_grilla(grilla, dim_X, dim_Y); // printea grilla
    grilla_free(grilla, dim_X, dim_Y); // lobera memoria de la grilla
    return 0;
}
