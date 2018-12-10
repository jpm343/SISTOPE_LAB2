#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "grilla.h"

// ejecucion de prueba: $./main.o -n 10 -L 2 -X 4 -Y 4 -d 1.5 

int cant_fotones = 1;
int dist_MAX = 2;//puede ser float?
int dim_X = 2;
int dim_Y = 2;
float delta = 1;
int flag_mostrar = 0;
casilla_t **grilla;

typedef struct pair{
    int first;
    int second;
}pair;

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

void *transferencia_radiativa(){
    printf("foton XD\n");
    return NULL;
}

int main(int argc, char **argv) {
    getFlags(argc, argv); // obtiene flags
    grilla = grilla_create(dim_X, dim_Y); // crea una grilla

    int i;
    pthread_t fotones[cant_fotones];
    for(i = 0; i < cant_fotones; i++){
        pthread_create(&fotones[i], NULL, transferencia_radiativa, NULL);
    }
    for(i = 0; i < cant_fotones; i++){
        pthread_join(fotones[i], NULL);
    }

    grilla_free(grilla, dim_X, dim_Y); // lobera memoria de la grilla
    return 0;
}
