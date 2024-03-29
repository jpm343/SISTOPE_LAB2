#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>
#include <assert.h>
#include "grilla.h"

#define PI 3.1415926535

// ejecucion de prueba: $./main.o -n 10 -L 2 -X 4 -Y 4 -d 1.5 

int cant_fotones = 1;
float dist_MAX = 2;//puede ser float?
int dim_X = 2;
int dim_Y = 2;
float delta = 1;
float angulo_MAX = 2*PI;
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
                assert((dim_X%2 == 0) && "dimension X debe ser un valor par");
                break;
            case 'Y':
                dim_Y = atoi(optarg);
                assert((dim_Y%2 == 0) && "dimension Y debe ser un valor par");
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

//funcion para aproximar coordenada
//entradas: puntero a la coordenada, dimension (Y o X) y posicion relativa en la grilla
void aproximarCoordenada(int *coordenada, float dimension, float posicionRelativa) {
    float i = 0;
    int j = 0;
    while(i < dimension) {
        if(posicionRelativa == i) {
            printf("hubo aproximacion\n");
            ++*coordenada;
            return;
        }
        j++;
        i+=(j*delta);
    }
}

void *transferenciaRadiativa(){
    int hayAbsorcion; //1 para absorcion. 0 para difusion
    float posX = 0; //inicialmente en posicion 0,0
    float posY = 0;
    float r; //random para distancia
    float l; //distancia a recorrer
    float u; //direccion (vector unitario o angulo director)
    float distanciaTotal = 0;

    //coordenadas inciales de la grilla
    float inicialX = 0 - (dim_X/2)*delta;
    float inicialY = 0 - (dim_Y/2)*delta;

    //posiciones relativas al inicio de la grilla
    float relativaX = 0;
    float relativaY = 0;

    //posiciones iniciales de la grilla
    int coordX = 0;
    int coordY = 0;

    //printf("%f,%f\n", inicialX, inicialY);

    while(distanciaTotal < dist_MAX) {
        //calculo de numeros aleatorios
        hayAbsorcion = rand() % 2;
        r = (float)rand()/(float)(RAND_MAX);
        l = -log(1-r);
        u = (float)rand()/(float)(RAND_MAX/angulo_MAX);

        //se calcula la posicion en la grilla y se verifica
        float desplazamientoX = l * sin(u);
        float desplazamientoY = l * cos(u);
        posX += desplazamientoX;
        posY += desplazamientoY;

        relativaX = (posX - inicialX);
        relativaY = (posY - inicialY);

        coordX = relativaX/delta;
        coordY = relativaY/delta;

        //si cae en algun borde, es necesario redefinir la coordenada (aproximarla)
        aproximarCoordenada(&coordX, dim_X, relativaX);
        aproximarCoordenada(&coordY, dim_Y, relativaY);

        //verificar si se escapa de la grilla
        if((coordX >= dim_X || coordY >= dim_Y) || (coordX < 0 || coordY < 0)) {
            if(flag_mostrar)
                printf("foton en la casilla y:%d , x:%d. evento: foton se escapa de la grilla\n", coordY, coordX);
            return NULL;
        }        

        float distanciaTotalAux = distanciaTotal;
        distanciaTotal += l;
        if(distanciaTotal > dist_MAX) { //si nos pasamos, se recalcula l y se fuerza absorcion
            l = dist_MAX - distanciaTotalAux;
            hayAbsorcion = 1;
        }

        //caso absorcion
        if(hayAbsorcion) {
            pthread_mutex_lock(&(grilla[coordY][coordX]).lock);
            (grilla[coordY][coordX]).energia++;
            pthread_mutex_unlock(&(grilla[coordY][coordX]).lock);

            if (flag_mostrar) {
                printf("foton en la casilla y:%d , x:%d. evento: absorcion\n", coordY, coordX);
            }        
        } 
        else if (!hayAbsorcion && flag_mostrar){
            printf("foton en la casilla y:%d , x:%d. evento: difusion\n", coordY, coordX);
        }        
    }
    return NULL;
}

int main(int argc, char **argv) {
    getFlags(argc, argv); // obtiene flags
    grilla = grilla_create(dim_Y, dim_X); // crea una grilla

    //para generar numeros aleatorios
    time_t t;
    srand((unsigned) time(&t));

    int i;
    pthread_t fotones[cant_fotones];
    for(i = 0; i < cant_fotones; i++){
        pthread_create(&fotones[i], NULL, transferenciaRadiativa, NULL);
    }
    for(i = 0; i < cant_fotones; i++){
        pthread_join(fotones[i], NULL);
    }
    fprint_grilla(grilla, dim_Y, dim_X);
    printf("\ngrilla mostrada en salida.out\n");

    grilla_free(grilla, dim_Y, dim_X); // lobera memoria de la grilla
    return 0;
}
