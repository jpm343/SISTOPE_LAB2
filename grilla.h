//suponiendo: grilla, x e y como variables globales

//Estrcutra casilla. Base para estructura grilla. Cada Casilla tiene su propio mutex
typedef struct Casilla {
    pthread_mutex_t lock;
    int energia;
}Casilla;

//entradas: medidas de la matriz en X e Y
void inicializar_grilla () {
    int i, j;
    for (i=0; i<dim_Y; i++) {
        for(j=0; j<dim_X; j++) {
            //se inicializa la casilla por cada posicion de la grilla
            Casilla *actual = (Casilla*) malloc(sizeof(Casilla));
            pthread_mutex_init(actual -> lock, NULL);
            actual -> energia = 0;

            //se agrega la casilla a la grilla
            grilla[i][j] = actual;
        } 
    }
}