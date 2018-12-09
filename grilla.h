//suponiendo: grilla, x e y como variables globales

//Estrcutra casilla. Base para estructura grilla. Cada Casilla tiene su propio mutex
typedef struct __casilla_t {
    pthread_mutex_t lock;
    int energia;
}casilla_t;

//entradas: medidas de la matriz en X e Y
casilla_t** grilla_create(int dim_X, int dim_Y){
    casilla_t **new;
    int i, j;
    new = (casilla_t **)calloc(dim_X, sizeof(casilla_t *) ); // arreglo de punteros de casilla
    for (i=0; i<dim_X; i++) {
        new[i] = (casilla_t *)calloc(dim_Y, sizeof(casilla_t) );
        for(j=0; j<dim_Y; j++) {
            pthread_mutex_init(&new[i][j].lock, NULL);
            new[i][j].energia = 0;
        }
    }
    printf("iniciada grilla\n");
    return new;
}

void print_grilla(casilla_t **grilla, int dim_X, int dim_Y){
    int i, j;
    for(i = 0; i < dim_X; i++){
        for(j = 0; j < dim_Y; j++){
            printf("%d ", grilla[i][j].energia);
        }printf("\n");
    }
}

void grilla_free(casilla_t **grilla, int dim_X, int dim_Y){
    int i;
    for(i = 0; i < dim_X; i++){
        free(grilla[i]);
        printf("liberado grilla[%d]\n", i);
    }
    free(grilla);
    printf("liberada grilla\n");
}
