#include "osos_contra_reloj.h"
#include "utiles.h"
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#define MAX_FILAS 20
#define MAX_COLUMNAS 30
#define MAX_ELEMENTOS 600

#define CHLOE 'C'
#define POLAR 'I'
#define PANDA 'P'
#define PARDO 'G'

#define CHLOE_EMOJI "\U0001f467"
#define POLAR_EMOJI "\u2744\uFE0F"
#define PANDA_EMOJI "\U0001f43c"
#define PARDO_EMOJI "\U0001f43b"
#define ARBOL_EMOJI "\U0001f332"
#define PIEDRA_EMOJI "\u26F0\uFE0F"
#define KOALA_EMOJI "\U0001f428"
#define PILA_EMOJI "\U0001f50b"
#define LINTERNA_EMOJI "\U0001f526"
#define VELA_EMOJI "\U0001f56f\uFE0F"
#define BENGALA_EMOJI "\U0001f387"
#define MOCHILA_EMOJI "\U0001f392"
#define VACIO_EMOJI "\u2B1B"

const char VACIO = '_';
const char ARBOL = 'A', PIEDRA = 'R', KOALA = 'K';
const char LINTERNA = 'L', VELA = 'V', BENGALA = 'E', PILA = 'B';
const int CANTIDAD_MAXIMA_ARBOLES = 350, CANTIDAD_MAXIMA_PIEDRAS = 80;
const int CANTIDAD_MAXIMA_PILAS = 30, CANTIDAD_MAXIMA_VELAS = 30, CANTIDAD_MAXIMA_BENGALAS = 10;

const int CANTIDAD_INICIAL_VELAS_POLAR = 6, CANTIDAD_INICIAL_VELAS_PANDA = 4, CANTIDAD_INICIAL_VELAS_PARDO = 4;
const int CANTIDAD_INICIAL_BENGALAS_PANDA = 2;
const int MOVIMIENTOS_VELA = 5;
const int MOVIMIENTOS_LINTERNA_PARDO = 15, MOVIMIENTOS_LINTERNA_PANDA = 10, MOVIMIENTOS_LINTERNA_POLAR = 10; 
const int MOVIMIENTOS_BENGALA = 3;
const int MOVIMIENTOS_PILA = 5;
const int NO_ENCONTRADO = -3;

const char IZQUIERDA = 'A', DERECHA = 'D', ARRIBA = 'W', ABAJO = 'S';
const char TIEMPO_ACTUAL = 'T';

const double SEGUNDOS_ARBOL = 1, SEGUNDOS_ARBOL_PARDO = 0.5;
const int SEGUNDOS_PIEDRA = 2;

const int PRIMERA_COL = 0;


/* precondiciones: -
 * postcondiciones: retorna un numero aleatorio entre 1 y 20.
 */
int fila_random() {
    int fila = rand() % 20;
    return fila;
}

/* precondiciones: -
 * postcondiciones: retorna un numero aleatorio entre 1 y 30.
 */
int columna_random() {
    int columna = rand() % 30;
    return columna;
}

/* precondiciones: la mochila a inicializar es la de POLAR.
 * postcondiciones: inicializa los campos del elemento_mochila_t mochila, cargando las herramientas con las que POLAR comienza el juego.
 */
void inicializar_mochila_polar(personaje_t* personaje){
    
    for(int i = 0; i < CANTIDAD_INICIAL_VELAS_POLAR; i++){
        personaje->mochila[i].tipo = VELA;
        personaje->mochila[i].movimientos_restantes = MOVIMIENTOS_VELA;
        personaje->cantidad_elementos++;
    }
    personaje->mochila[personaje->cantidad_elementos].tipo = LINTERNA;
    personaje->mochila[personaje->cantidad_elementos].movimientos_restantes = MOVIMIENTOS_LINTERNA_POLAR;
    personaje->cantidad_elementos++;
}

/* precondiciones: la mochila a inicializar es la de PANDA.
 * postcondiciones: inicializa los campos del elemento_mochila_t mochila, cargando las herramientas con las que PANDA comienza el juego.
 */
void inicializar_mochila_panda(personaje_t* personaje){

    for(int i = 0; i < CANTIDAD_INICIAL_VELAS_PANDA; i++){
        personaje->mochila[i].tipo = VELA;
        personaje->mochila[i].movimientos_restantes = MOVIMIENTOS_VELA;
        personaje->cantidad_elementos++;
    }

    for(int j = personaje->cantidad_elementos; j < (CANTIDAD_INICIAL_VELAS_PANDA + CANTIDAD_INICIAL_BENGALAS_PANDA); j++){
        personaje->mochila[j].tipo = BENGALA;
        personaje->mochila[j].movimientos_restantes = MOVIMIENTOS_BENGALA;
        personaje->cantidad_elementos++;
    }
    personaje->mochila[personaje->cantidad_elementos].tipo = LINTERNA;
    personaje->mochila[personaje->cantidad_elementos].movimientos_restantes = MOVIMIENTOS_LINTERNA_POLAR;
    personaje->cantidad_elementos++;
}

/* precondiciones: la mochila a inicializar es la de PARDO.
 * postcondiciones: inicializa los campos del elemento_mochila_t mochila, cargando las herramientas con las que PARDO comienza el juego.
 */
void inicializar_mochila_pardo(personaje_t* personaje){
    for(int i = 0; i < CANTIDAD_INICIAL_VELAS_PARDO; i++){
        personaje->mochila[i].tipo = VELA;
        personaje->mochila[i].movimientos_restantes = MOVIMIENTOS_VELA;
        personaje->cantidad_elementos++;
    }
    personaje->mochila[personaje->cantidad_elementos].tipo = LINTERNA;
    personaje->mochila[personaje->cantidad_elementos].movimientos_restantes = MOVIMIENTOS_LINTERNA_PARDO;
    personaje->cantidad_elementos++;
}

/* precondiciones: la variable char tipo_personaje debe ser POLAR, PANDA o PARDO. El tope debe estar inicializado.
 * postcondiciones: inicializa la mochila del personaje, segun si este es POLAR, PANDA o PARDO.
 */
void inicializar_mochila(personaje_t* personaje) {
    if(personaje->tipo == POLAR) {
        inicializar_mochila_polar(personaje);
    
    } else if(personaje->tipo == PANDA) {
        inicializar_mochila_panda(personaje);

    } else if(personaje->tipo == PARDO) {
        inicializar_mochila_pardo(personaje);
    }
}

/* precondiciones: - los topes de elemento_del_mapa_t obstaculos y herramientas deben estar inicializados.
 *                 - la fila del elemento esta entre 1 y 20 y la columna entre 1 y 30.
 * postcondiciones: - recorre todos los elementos/personajes inicializados hasta el momento de juego_t juego
 *  y retorna TRUE si la posicion del elemento ya esta ocupada.
 *                  - retora FALSE si la posicion del elemento no esta ocupada.
 */
bool posicion_esta_ocupada(juego_t* juego, int fila_elemento, int columna_elemento){
    int i = 0;
    int j = 0;
    bool posicion_ocupada = false;
    while(i < juego->cantidad_obstaculos && !posicion_ocupada){
        if(fila_elemento == juego->obstaculos[i].posicion.fil && columna_elemento == juego->obstaculos[i].posicion.col){
            posicion_ocupada = true;
        }
        i++;
    }
    while(j < juego->cantidad_obstaculos && !posicion_ocupada){
        if(fila_elemento == juego->herramientas[j].posicion.fil && columna_elemento == juego->herramientas[j].posicion.col){
            posicion_ocupada = true;
        }
        j++;
    }
    if(fila_elemento == juego->amiga_chloe.fil && columna_elemento == juego->amiga_chloe.col){
        posicion_ocupada = true;
    } 
    if(fila_elemento == juego->personaje.posicion.fil && columna_elemento == juego->personaje.posicion.col){
        posicion_ocupada = true;
    }
    return posicion_ocupada;
}

/* precondiciones: - la variable char tipo_personaje debe estar inicializada y debe ser POLAR, PANDA o PARDO.
 *                 - la fila del personaje estara entre 1 y 20 y la columna sera 0.
 * postcondiciones: - asigna la personalidad al personaje y una posicion valida.
 *                  - inicializa los campos elemento_en_uso y cantidad_elementos.
 *                  - inicializa la mochila del personaje.
 */
void inicializar_personaje(personaje_t* personaje, char tipo_personaje) {
    personaje->tipo = tipo_personaje;

    personaje->posicion.fil = fila_random();
    personaje->posicion.col = PRIMERA_COL;
    personaje->elemento_en_uso = -1;
    personaje->tiempo_perdido = 0;
    personaje->cantidad_elementos = 0;
    inicializar_mochila(personaje);
}

/* precondiciones: la fila de Chloe estara entre 1 y 20 y la columna entre 1 y 30.
 * postcondiciones: le asigna una posicion valida y aleatoria a Chloe y e inicializa su estado de visibilidad.
 */
void inicializar_chloe(juego_t* juego) {
    int fila_chloe = fila_random();
    int columna_chloe = columna_random();
    while(posicion_esta_ocupada(juego, fila_chloe, columna_chloe)){
        fila_chloe = fila_random();
        columna_chloe = columna_random();
    }
    juego->amiga_chloe.fil = fila_chloe;
    juego->amiga_chloe.col = columna_chloe;
    juego->chloe_visible = false;
}

/* precondiciones: el tope del elemento_del_mapa_t obstaculos debe estar inicializado.
 * postcondiciones: - inicializa un koala en el vector obstaculos en la ultima posicion con una posicion valida aleatoria.
 *                  - el estado de visibilidad del koala es FALSE.
 */
void inicializar_koala(juego_t* juego) {
    int fila_koala = fila_random();
    int columna_koala = columna_random();
    while(posicion_esta_ocupada(juego, fila_koala, columna_koala)){
        fila_koala = fila_random();
        columna_koala = columna_random();
    }
    juego->obstaculos[juego->cantidad_obstaculos].posicion.fil = fila_koala;
    juego->obstaculos[juego->cantidad_obstaculos].posicion.col = columna_koala;
    juego->obstaculos[juego->cantidad_obstaculos].tipo = KOALA;
    juego->obstaculos[juego->cantidad_obstaculos].visible = false; 
    juego->cantidad_obstaculos++;
}

/* precondiciones: - el tope de elemento_del_mapa_t obstaculos debe estar inicializado.
 *                 - las filas de los obstaculos estaran entre 1 y 20 y las columnas entre 1 y 30.
 *                 - el tipo de obstaculo a cargar debe ser ARBOL o PIEDRA. 
 * postcondiciones: - inicializa un obstaculo del juego con una posicion valida y aleatoria y su estado de visibilidad en FALSE.
 */
void inicializar_obstaculos(juego_t* juego, int cantidad_maxima_obstaculos, char tipo_obstaculo) {
    for(int i = juego->cantidad_obstaculos; i < cantidad_maxima_obstaculos; i++) {
        int fila_obstaculo = fila_random();
        int columna_obstaculo = columna_random();
        while(posicion_esta_ocupada(juego, fila_obstaculo, columna_obstaculo)){
            fila_obstaculo = fila_random();
            columna_obstaculo = columna_random();
        }
        juego->obstaculos[i].posicion.fil = fila_obstaculo;
        juego->obstaculos[i].posicion.col = columna_obstaculo;
        juego->obstaculos[i].tipo = tipo_obstaculo;
        juego->obstaculos[i].visible = false; 
        juego->cantidad_obstaculos++;
    }
}

/* precondiciones: - el tope de elemento_del_mapa_t herramientas debe estar inicializado.
 *                 - las filas de las herramientas estaran entre 1 y 20 y las columnas entre 1 y 30.
 *                 - el tipo de herramienta a cargar debe ser PILA, VELA o BENGALA. 
 * postcondiciones: - inicializa una herramienta del juego con una posicion valida y aleatoria y su estado de visibilidad en FALSE.
 */
void inicializar_herramientas(juego_t* juego, int cantidad_maxima_herramientas, char tipo_herramienta) {
    for(int i = juego->cantidad_herramientas; i < cantidad_maxima_herramientas; i++) {
        int fila_herramienta = fila_random();
        int columna_herramienta = columna_random();
        while(posicion_esta_ocupada(juego, fila_herramienta, columna_herramienta)){
            fila_herramienta = fila_random();
            columna_herramienta = columna_random();
        }
        juego->herramientas[i].posicion.fil = fila_herramienta;
        juego->herramientas[i].posicion.col = columna_herramienta;
        juego->herramientas[i].tipo = tipo_herramienta;
        juego->herramientas[i].visible = false; 
        juego->cantidad_herramientas++;
    }
}

/* precondiciones: la posicion del personaje debe estar inicializada.
 * postcondiciones: modifica el valor de la matriz en la posicion asignada del personaje, siendo este valor el tipo de personaje.
 */
void posicionar_personaje(char bosque[MAX_FILAS][MAX_COLUMNAS], personaje_t personaje) {
    bosque[personaje.posicion.fil][personaje.posicion.col] = personaje.tipo;
}

/* precondiciones: la posicion de Chloe y su estado de visibilidad deben estar inicializados.
 * postcondiciones: - modifica el valor de la matriz en la posicion asignada de Chloe, siendo este valor el caracter 'C'.
 *                  - solo se modifica el valor de la matriz si el estado de visibilidad de Chloe es TRUE.
 */
void posicionar_chloe(char bosque[MAX_FILAS][MAX_COLUMNAS], juego_t juego) {
    if(juego.chloe_visible == true){
        bosque[juego.amiga_chloe.fil][juego.amiga_chloe.col] = CHLOE;
    }  
}

/* precondiciones: - la posicion de los elementos y su estado de visibilidad deben estar inicializados.
 *                 - la matriz debe estar inicializada.
 *                 - los elementos a posicionar deben ser ARBOL, PIEDRA, KOALA, PILA, VELA o BENGALA.
 * postcondiciones: - modifica el valor de la matriz en la posicion de asignada a cada elemento, y el valor sera el tipo de elemento.
 *                  - solo se modifica la posicion en la matriz si el estado de visibilidad del elemento es TRUE.
 */
void posicionar_elementos(char bosque[MAX_FILAS][MAX_COLUMNAS], elemento_del_mapa_t elementos[MAX_ELEMENTOS], int cantidad_elementos) {
    for (int i = 0; i < cantidad_elementos; i++){
        if(elementos[i].visible == true){
            bosque[elementos[i].posicion.fil][elementos[i].posicion.col] = elementos[i].tipo;
        }
    }
}

/* precondiciones: -
 * postcondiciones: inicializa todas las posiciones de la matriz con VACIO (-).
 */
void inicializar_bosque(char bosque[MAX_FILAS][MAX_COLUMNAS]){
    for(int i = 0; i < MAX_FILAS; i++){
        for (int j = 0; j < MAX_COLUMNAS; j++){
            bosque[i][j] = VACIO;
        }  
    }  
}

void inicializar_juego(juego_t* juego, char tipo_personaje) {
    juego->cantidad_obstaculos = 0;
    juego->cantidad_herramientas = 0;

    inicializar_personaje(&juego->personaje, tipo_personaje);
    inicializar_chloe(juego);

    inicializar_obstaculos(juego, CANTIDAD_MAXIMA_PIEDRAS, PIEDRA);
    inicializar_obstaculos(juego, (CANTIDAD_MAXIMA_PIEDRAS + CANTIDAD_MAXIMA_ARBOLES), ARBOL);
    inicializar_koala(juego);

    inicializar_herramientas(juego, CANTIDAD_MAXIMA_PILAS, PILA);
    inicializar_herramientas(juego, (CANTIDAD_MAXIMA_PILAS + CANTIDAD_MAXIMA_VELAS), VELA);
    inicializar_herramientas(juego, (CANTIDAD_MAXIMA_PILAS + CANTIDAD_MAXIMA_VELAS + CANTIDAD_MAXIMA_BENGALAS), BENGALA);
}


/* precondiciones: elemento_mochila_t mochila debe estar inicializado.
 * postcondiciones: muestra por pantalla las herramientas de la mochila del personaje.
 */
void mostrar_mochila(elemento_mochila_t mochila[MAX_HERRAMIENTAS], int cantidad_elementos){
    printf(" Herramientas de la mochila %s   ", MOCHILA_EMOJI);
    for(int i = 0; i < cantidad_elementos; i++){
        if(mochila[i].tipo == LINTERNA){
                printf("%s ", LINTERNA_EMOJI);

        } else if(mochila[i].tipo == VELA){
                printf("%s  ", VELA_EMOJI);

        } else if(mochila[i].tipo == BENGALA){
                printf("%s ", BENGALA_EMOJI);

        } else {
            printf(" %c", mochila[i].tipo);
        }
    }
    printf("\n");
}

/* precondiciones: - elemento_mochila_t mochila debe estar inicializado.
 *                 - int elemento_en_uso debe estar inicializado.
 * postcondiciones: muestra por pantalla los movimientos restantes de la herramienta activada.
 */
void mostrar_herramienta_en_uso(elemento_mochila_t mochila[MAX_HERRAMIENTAS], int cantidad_elementos, int elemento_en_uso){
    for(int j = 0; j < cantidad_elementos; j++){

        if((mochila[j].tipo == LINTERNA) && (elemento_en_uso == j)){
            printf(" Movimientos restantes %s : %i\n", LINTERNA_EMOJI, mochila[j].movimientos_restantes);
            printf(" Posicion linterna en la mochila: %i\n", j);

        } else if((mochila[j].tipo == VELA) && (elemento_en_uso == j)){
            printf(" Movimientos restantes %s : %i\n", VELA_EMOJI, mochila[j].movimientos_restantes);
            printf(" Posicion vela en la mochila: %i\n", j);

        } else if((mochila[j].tipo == BENGALA) && (elemento_en_uso == j)){
            printf(" Movimientos restantes %s : %i\n", BENGALA_EMOJI, mochila[j].movimientos_restantes);
            printf(" Posicion bengala en la mochila: %i\n", j);    
        }
    }
}

void mostrar_juego(juego_t juego) {
    char bosque[MAX_FILAS][MAX_COLUMNAS];
    inicializar_bosque(bosque);

    posicionar_chloe(bosque, juego);
    posicionar_elementos(bosque, juego.obstaculos, juego.cantidad_obstaculos);
    posicionar_elementos(bosque, juego.herramientas, juego.cantidad_herramientas);
    posicionar_personaje(bosque, juego.personaje);

    printf("\n");
    printf("   0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29  \n");
    for(int i = 0; i < MAX_FILAS; i++) {
        printf("%2d ", i);
        for(int j = 0; j < MAX_COLUMNAS; j++) {

            if(bosque[i][j] == POLAR){
                printf("%s  ", POLAR_EMOJI);

            } else if(bosque[i][j] == PANDA){
                printf("%s ", PANDA_EMOJI);

            } else if(bosque[i][j] == PARDO){
                printf("%s ", PARDO_EMOJI);

            } else if(bosque[i][j] == CHLOE){
                printf("%s  ", CHLOE_EMOJI);

            } else if(bosque[i][j] == ARBOL){
                printf("%s ", ARBOL_EMOJI);

            } else if(bosque[i][j] == PIEDRA){
                printf("%s  ", PIEDRA_EMOJI);

            } else if(bosque[i][j] == KOALA){
                printf("%s ", KOALA_EMOJI);

            } else if(bosque[i][j] == VELA){
                printf("%s  ", VELA_EMOJI);

            } else if(bosque[i][j] == BENGALA){
                printf("%s ", BENGALA_EMOJI);

            } else if(bosque[i][j] == PILA){
                printf("%s ", PILA_EMOJI);
            }
            else if(bosque[i][j] == VACIO){
                printf("%s ", VACIO_EMOJI);
            } 
            //printf("|");
        }
        printf("\n");
    }
    printf("\n");

    mostrar_mochila(juego.personaje.mochila, juego.personaje.cantidad_elementos);
    mostrar_herramienta_en_uso(juego.personaje.mochila, juego.personaje.cantidad_elementos, juego.personaje.elemento_en_uso);
    printf(" Elemento en uso: %i\n", juego.personaje.elemento_en_uso);
}

/* precondiciones: los topes de los structs elemento_del_mapa_t obstaculos y herramientas deben estar inicializados.
 * postcondiciones: cambia el estado de visibilidad de cualquier elemento o personaje a FALSE.
 */
void limpiar_terreno(juego_t* juego){
    juego->chloe_visible = false;
    for(int i = 0; i < juego->cantidad_obstaculos; i++){
        juego->obstaculos[i].visible = false;
    }
    for(int j = 0; j < juego->cantidad_herramientas; j++){
        juego->herramientas[j].visible = false;
    }
}

/* precondiciones: la jugada debe ser ARRIBA, ABAJO, IZQUIERDA o DERECHA.
 * postcondiciones: - retorna TRUE si la coordenada se encuentra en una fila o columna limite del terreno.
 *                  - retorna FALSE en caso contrario.
 */
bool esta_en_frontera(char jugada, coordenada_t coordenada) {
    return ((jugada == ARRIBA && coordenada.fil == 0) || (jugada == ABAJO && coordenada.fil == MAX_FILAS-1) || (jugada == IZQUIERDA && coordenada.col == 0) || (jugada == DERECHA && coordenada.col == MAX_COLUMNAS-1));
}

bool es_jugada_valida(char jugada) {
    return ((jugada == ARRIBA) || (jugada == ABAJO) || (jugada == DERECHA) || (jugada == IZQUIERDA) || (jugada == VELA) || (jugada == BENGALA) || (jugada == LINTERNA) || (jugada == TIEMPO_ACTUAL)); 
}

/* precondiciones: el tope de elemento_del_mapa_t herramientas debe estar inicializado.
 * postcondiciones: - elimina una herramienta del vector herramientas.
 */
void eliminar_herramienta_del_mapa(juego_t* juego, int posicion){
    juego->herramientas[posicion] = juego->herramientas[juego->cantidad_herramientas-1];
    juego->cantidad_herramientas--;
}

/* precondiciones: el tope de elemento_del_mapa_t herramientas debe estar inicializado.
 * postcondiciones: - devuelve TRUE si la posicion actual del personaje coincide con la posicion de alguna herramienta.
 *                  - devueve FALSE en caso contrario.
 */
bool posicion_es_herramienta(juego_t* juego){
    bool es_herramienta = false;
    for(int i = 0; i < juego->cantidad_herramientas; i++){
        if((juego->personaje.posicion.fil == juego->herramientas[i].posicion.fil) && (juego->personaje.posicion.col == juego->herramientas[i].posicion.col)){
            es_herramienta = true;
        }
    }
    return es_herramienta;
}

/* precondiciones: el tope de elemento_del_mapa_t obstaculos debe estar inicializado.
 * postcondiciones: - devuelve TRUE si la posicion actual del personaje coincide con la posicion de un obstaculo.
 *                  - devueve FALSE en caso contrario.
 */
bool posicion_es_obstaculo(juego_t* juego){
    bool es_obstaculo = false;
    for(int i = 0; i < juego->cantidad_obstaculos; i++){
        if((juego->personaje.posicion.fil == juego->obstaculos[i].posicion.fil) && (juego->personaje.posicion.col == juego->obstaculos[i].posicion.col)){
            es_obstaculo = true;
        }
    }
    return es_obstaculo;
}

/* precondiciones: - la herramienta debe ser LINTERNA, VELA o BENGALA.
 *                 - elemento_mochila_t mochila debe estar inicializado.
 * postcondiciones: - retorna la posicion de la herramienta en la mochila del personaje.
 *                  - si la herramienta no esta en la mochila, retorna NO_ENCONTRADO, cuyo valor es -3.
 */
int posicion_herramienta(personaje_t personaje, char herramienta){
    int i = 0;
    int posicion = NO_ENCONTRADO;
    bool encontrada = false;
    while(i < personaje.cantidad_elementos && !encontrada){
        if(personaje.mochila[i].tipo == herramienta){
            posicion = i;
            encontrada = true;
        }
        i++;
    }
    return posicion;
}

/* precondiciones: - la herramienta debe ser PILA, VELA o BENGALA.
 *                 - la posicion actual del personaje debe coincidir con la de una herramienta.
 *                 - el tope de elemento_del_mapa_t herramientas debe estar inicializado.
 * postcondiciones: - si la herramienta es VELA o BENGALA, la agrega a la mochila del personaje en la ultima posicion.
 *                  - si la herramienta es PILA, le suma 5 movimientos a la herramienta LINTERNA.
 *                  - elimina de elemento_del_mapa_t herramientas la herramienta guardada en la mochila.
 */
void recolectar_herramienta(juego_t* juego){
    int i = 0;
    while(!((juego->herramientas[i].posicion.fil == juego->personaje.posicion.fil) && (juego->herramientas[i].posicion.col == juego->personaje.posicion.col))){
        i++;
    }
    if(juego->herramientas[i].tipo == VELA){
        juego->personaje.mochila[juego->personaje.cantidad_elementos].tipo = VELA;
        juego->personaje.mochila[juego->personaje.cantidad_elementos].movimientos_restantes = MOVIMIENTOS_VELA;
        juego->personaje.cantidad_elementos++;

    } else if(juego->herramientas[i].tipo == BENGALA){
        juego->personaje.mochila[juego->personaje.cantidad_elementos].tipo = BENGALA;
        juego->personaje.mochila[juego->personaje.cantidad_elementos].movimientos_restantes = MOVIMIENTOS_BENGALA;
        juego->personaje.cantidad_elementos++;

    } else if(juego->herramientas[i].tipo == PILA){
        juego->personaje.mochila[posicion_herramienta(juego->personaje, LINTERNA)].movimientos_restantes += MOVIMIENTOS_PILA;
    }
    eliminar_herramienta_del_mapa(juego, i);
}

/* precondiciones: el tope de elemento_mochila_t mochila debe estar inicializado.
 * postcondiciones: elimina una herramienta de la mochila del personaje.
 */
void quitar_herramienta_de_mochila(personaje_t* personaje, int posicion_herramienta){
    personaje->mochila[posicion_herramienta] = personaje->mochila[personaje->cantidad_elementos-1];
    personaje->cantidad_elementos--;
}

/* precondiciones: la posicion actual del personaje debe coincidir con la de un obstaculo.
 * postcondiciones: retorna el tipo de obstaculo cuya posicion es igual a la del personaje.
 */
char tipo_obstaculo(juego_t* juego){
    int i = 0;
    while(!((juego->personaje.posicion.fil == juego->obstaculos[i].posicion.fil) && (juego->personaje.posicion.col == juego->obstaculos[i].posicion.col))){
        i++;
    }
    return juego->obstaculos[i].tipo;
}

/* precondiciones: el obstaculo debe ser ARBOL o PIEDRA.
 * postcondiciones: suma los segundos correspondientes al tiempo perdido del personaje, dependiendo el tipo de obstaculo y el personaje.
 */
void sumar_segundos(personaje_t* personaje, char obstaculo){
    if(obstaculo == ARBOL && personaje->tipo != PARDO){
        personaje->tiempo_perdido += SEGUNDOS_ARBOL;

    } else if(obstaculo == ARBOL && personaje->tipo == PARDO){
       personaje->tiempo_perdido += SEGUNDOS_ARBOL_PARDO;

    } else if(obstaculo == PIEDRA && personaje->tipo != POLAR){
        personaje->tiempo_perdido += SEGUNDOS_PIEDRA;
    }
}

/* precondiciones: -
 * postcondiciones: modifica la posicion del personaje. La fila es aleatoria y la columna es 0. 
 */
void reubicar_personaje(int* fila, int*columna){
    (*fila) = fila_random();
    (*columna) = PRIMERA_COL;
}

/* precondiciones: - el tope del struct elemento_del_mapa_t obstaculos debe estar inicializado.
 *                 - el char ultimo_movimiento debe ser IZQUIERDA, DERECHA, ARRIBA o ABAJO. 
 * postcondiciones: - agrega un koala a la matriz.
 *                  - la posicion del koala agregado dependera de la posicion del personaje y su ultimo movimiento.
 *                  - modifica el estado de visibilidad del koala a TRUE.
 */
void agregar_koala(juego_t* juego){
    int fila_oso = juego->personaje.posicion.fil;
    int columna_oso = juego->personaje.posicion.col;
    char ultimo_movimiento = juego->personaje.ultimo_movimiento;

    inicializar_koala(juego);

    if(ultimo_movimiento == DERECHA){
        while((juego->obstaculos[juego->cantidad_obstaculos-1].posicion.fil != fila_oso) || (juego->obstaculos[juego->cantidad_obstaculos-1].posicion.col < columna_oso)){
            juego->cantidad_obstaculos--;
            inicializar_koala(juego);
        }

    } else if(ultimo_movimiento == IZQUIERDA){
        while((juego->obstaculos[juego->cantidad_obstaculos-1].posicion.fil != fila_oso) || (juego->obstaculos[juego->cantidad_obstaculos-1].posicion.col > columna_oso)){
            juego->cantidad_obstaculos--;
            inicializar_koala(juego);
        }

    } else if(ultimo_movimiento == ARRIBA){
        while((juego->obstaculos[juego->cantidad_obstaculos-1].posicion.fil > fila_oso) || (juego->obstaculos[juego->cantidad_obstaculos-1].posicion.col != columna_oso)){
            juego->cantidad_obstaculos--;
            inicializar_koala(juego);
        }

    } else if(ultimo_movimiento == ABAJO){
        while((juego->obstaculos[juego->cantidad_obstaculos-1].posicion.fil < fila_oso) || (juego->obstaculos[juego->cantidad_obstaculos-1].posicion.col != columna_oso)){
            juego->cantidad_obstaculos--;
            inicializar_koala(juego);
        }
    }
    juego->obstaculos[juego->cantidad_obstaculos-1].visible = true;
}

/* precondiciones: - el valor de int* elemento_en_uso es -1 si no hay ninguna herramienta activada o int posicion_herramienta si alguna esta activada.
 *                 - int* elemento_en_uso debe estar inicializado. 
 * postcondiciones: - si el elemento en uso es distinto de la posicion de la herramienta que se desea activar, elemento en uso modifica su valor por la posicion de la herramienta.
 *                  - si el elemento en uso es igual a la posicion de la herramienta, modifica su valor a -1.
 */
void activar_herramienta(int* elemento_en_uso, int posicion_herramienta){
    if((*elemento_en_uso) != posicion_herramienta){
        (*elemento_en_uso) = posicion_herramienta;
    } else {
        (*elemento_en_uso) = -1;
    }
}

/* precondiciones: - el valor de int elemento_en_uso es posicion_linterna.
 * postcondiciones: - cambia el estado de visibilidad a TRUE de los elementos/Chloe que esten en la misma fila
  que el personaje y su columna sea mayor a la del mismo.               
 */                 
void iluminar_derecha(juego_t* juego, int fila_oso, int columna_oso){
    if((juego->amiga_chloe.fil == fila_oso) && (juego->amiga_chloe.col > columna_oso)){
        juego->chloe_visible = true;
    }
    for(int i = 0; i < juego->cantidad_obstaculos; i++){
        if((juego->obstaculos[i].posicion.fil == fila_oso) && (juego->obstaculos[i].posicion.col > columna_oso)){
            juego->obstaculos[i].visible = true;
        } 
    }
    for(int j = 0; j < juego->cantidad_herramientas; j++){
        if((juego->herramientas[j].posicion.fil == fila_oso) && (juego->herramientas[j].posicion.col > columna_oso)){
            juego->herramientas[j].visible = true;
        } 
    }
}

/* precondiciones: - el valor de int elemento_en_uso es posicion_linterna.
 * postcondiciones: - cambia el estado de visibilidad a TRUE de los elementos/Chloe que esten en la misma fila
 * que el personaje y su columna sea menor a la del mismo.
 */  
void iluminar_izquierda(juego_t* juego, int fila_oso, int columna_oso){
    if((juego->amiga_chloe.fil == fila_oso) && (juego->amiga_chloe.col < columna_oso)){
        juego->chloe_visible = true;
    }
    for(int i = 0; i < juego->cantidad_obstaculos; i++){
        if((juego->obstaculos[i].posicion.fil == fila_oso) && (juego->obstaculos[i].posicion.col < columna_oso)){
            juego->obstaculos[i].visible = true;
        } 
    }
    for(int j = 0; j < juego->cantidad_herramientas; j++){
        if((juego->herramientas[j].posicion.fil == fila_oso) && (juego->herramientas[j].posicion.col < columna_oso)){
            juego->herramientas[j].visible = true;
        } 
    }
}

/* precondiciones: - el valor de int elemento_en_uso es posicion_linterna.
 * postcondiciones: - cambia el estado de visibilidad a TRUE de los elementos/Chloe que esten en la misma columna
  que el personaje y su fila sea mayor a la del mismo.
 */  
void iluminar_abajo(juego_t* juego, int fila_oso, int columna_oso){
    if((juego->amiga_chloe.fil > fila_oso) && (juego->amiga_chloe.col == columna_oso)){
        juego->chloe_visible = true;
    }
    for(int i = 0; i < juego->cantidad_obstaculos; i++){
        if((juego->obstaculos[i].posicion.fil > fila_oso) && (juego->obstaculos[i].posicion.col == columna_oso)){
            juego->obstaculos[i].visible = true;
        } 
    }
    for(int j = 0; j < juego->cantidad_herramientas; j++){
        if((juego->herramientas[j].posicion.fil > fila_oso) && (juego->herramientas[j].posicion.col == columna_oso)){
            juego->herramientas[j].visible = true;
        } 
    }
}

/* precondiciones: - el valor de int elemento_en_uso es posicion_linterna.
 * postcondiciones: - cambia el estado de visibilidad a TRUE de los elementos/Chloe que esten en la misma columna
  que el personaje y su fila sea menor a la del mismo.
 */ 
void iluminar_arriba(juego_t* juego, int fila_oso, int columna_oso){
    if((juego->amiga_chloe.fil < fila_oso) && (juego->amiga_chloe.col == columna_oso)){
        juego->chloe_visible = true;
    }
    for(int i = 0; i < juego->cantidad_obstaculos; i++){
        if((juego->obstaculos[i].posicion.fil < fila_oso) && (juego->obstaculos[i].posicion.col == columna_oso)){
            juego->obstaculos[i].visible = true;
        } 
    }
    for(int j = 0; j < juego->cantidad_herramientas; j++){
        if((juego->herramientas[j].posicion.fil < fila_oso) && (juego->herramientas[j].posicion.col == columna_oso)){
            juego->herramientas[j].visible = true;
        } 
    }
}

/* precondiciones: - el valor de int elemento_en_uso es posicion_linterna.
 *                 - el valor de char ultimo_movimiento debe ser ARRIBA, ABAJO, IZQUIERDA o DERECHA.
 * postcondiciones: - segun el valor de char ultimo_movimiento, se ilumina la fila o columna en la que esta el personaje.
 *                  - si los movimientos restantes de la linterna = 0, se desactiva la herramienta.
 */
void iluminar_linterna(juego_t* juego){
    int fila_oso = juego->personaje.posicion.fil;
    int columna_oso = juego->personaje.posicion.col;
    char ultimo_movimiento = juego->personaje.ultimo_movimiento;
    int posicion_linterna = juego->personaje.elemento_en_uso;

    if(juego->personaje.mochila[posicion_linterna].movimientos_restantes > 0){

        if(ultimo_movimiento == DERECHA){
            iluminar_derecha(juego, fila_oso, columna_oso);

        } else if(ultimo_movimiento == IZQUIERDA){
            iluminar_izquierda(juego, fila_oso, columna_oso);

        } else if(ultimo_movimiento == ARRIBA){
            iluminar_arriba(juego, fila_oso, columna_oso);

        } else if(ultimo_movimiento == ABAJO){
            iluminar_abajo(juego, fila_oso, columna_oso);
        }
        juego->personaje.mochila[posicion_linterna].movimientos_restantes--;
        
    } else {
        activar_herramienta(&juego->personaje.elemento_en_uso, posicion_linterna);
    }
}


/* precondiciones: - la coordenada del personaje debe estar inicializada.
 *                 - la coordenada del elemento debe estar inicializada.
 * postcondiciones: - retorna TRUE si la distancia entre la fila/columna del personaje y la del elemento es menor o igual a 1 posicion.
 *                  - retorna FALSE en caso contrario.
 */
bool es_adyacente(coordenada_t posicion_personaje, coordenada_t posicion_elemento){
    bool adyacente = false;
    if((abs(posicion_personaje.fil - posicion_elemento.fil) <= 1) && (abs(posicion_personaje.col - posicion_elemento.col) <= 1)){
        adyacente = true;
    }
    return adyacente;
}

/* precondiciones: - el valor de int elemento_en_uso es posicion_vela.
 * postcondiciones: - si la posicion del elemento/Chloe coincide con la posicion del oso, modifica el estado de visibilidad del elemento/Chloe a TRUE.
 */
void iluminar_adyacente(juego_t* juego){
    if(es_adyacente(juego->personaje.posicion, juego->amiga_chloe)){
        juego->chloe_visible = true;
    }
    for(int i = 0; i < juego->cantidad_obstaculos; i++){
        if(es_adyacente(juego->personaje.posicion, juego->obstaculos[i].posicion)){
            juego->obstaculos[i].visible = true;
        }
    }
    for(int j = 0; j < juego->cantidad_herramientas; j++){
        if(es_adyacente(juego->personaje.posicion, juego->herramientas[j].posicion)){
            juego->herramientas[j].visible = true;
        }
    }
}

/* precondiciones: - el valor de int elemento_en_uso es posicion_vela.
 * postcondiciones: - ilumina todas las posiciones de los elementos o Chloe que sean adyacentes a la posicion del personaje.
 *                  - si la vela no tiene mas movimientos, se desactiva y se la elimina de la mochila del personaje.
 */
void iluminar_vela(juego_t* juego){
    int posicion_vela = juego->personaje.elemento_en_uso;
    if(juego->personaje.mochila[posicion_vela].movimientos_restantes > 0){
        iluminar_adyacente(juego);
        juego->personaje.mochila[posicion_vela].movimientos_restantes--;

    } else {
        activar_herramienta(&juego->personaje.elemento_en_uso, posicion_vela);
        quitar_herramienta_de_mochila(&juego->personaje, posicion_vela);
    }
}

/* precondiciones: - el valor de int elemento_en_uso es posicion_bengala.
 * postcondiciones: - si la distancia entre el punto aleatorio y la posicion del elemento/Chloe es menor a int_cantidad_posiciones,
 * modifica el estado de visibilidad del elemento/Chloe a TRUE.
 */
void iluminar_manhattan(juego_t* juego, int fila_random, int columna_random, int cantidad_posiciones){
    if((abs(columna_random - juego->amiga_chloe.col) <= cantidad_posiciones) && (juego->amiga_chloe.fil == fila_random)){
        juego->chloe_visible = true;
    }
    for(int i = 0; i < juego->cantidad_obstaculos; i++){
        if((abs(columna_random- juego->obstaculos[i].posicion.col) <= cantidad_posiciones) && (juego->obstaculos[i].posicion.fil == fila_random)){
            juego->obstaculos[i].visible = true;
        }  
    }
    for(int j = 0; j < juego->cantidad_herramientas; j++){
        if((abs(columna_random - juego->herramientas[j].posicion.col) <= cantidad_posiciones) && (juego->herramientas[j].posicion.fil == fila_random)){
            juego->herramientas[j].visible = true;
        }  
    }
}

/* precondiciones: - el valor de int elemento_en_uso es posicion_bengala.
 * postcondiciones: - ilumina todos los elementos/Chloe cuyas posiciones esten a una distancia menor o igual a 3 posiciones a la redonda de un punto aleatorio.
 *                  - si la bengala no tiene mas movimientos, se elimina de la mochila del personaje.
 */
void iluminar_bengala(juego_t* juego){
    coordenada_t punto_random;
    punto_random.fil = fila_random();
    punto_random.col = columna_random();
    int posicion_bengala = juego->personaje.elemento_en_uso;

    if(juego->personaje.mochila[posicion_bengala].movimientos_restantes > 0){
        iluminar_manhattan(juego, punto_random.fil+3, punto_random.col, 0);
        iluminar_manhattan(juego, punto_random.fil+2, punto_random.col, 1);
        iluminar_manhattan(juego, punto_random.fil+1, punto_random.col, 2);
        iluminar_manhattan(juego, punto_random.fil, punto_random.col, 3);
        iluminar_manhattan(juego, punto_random.fil-1, punto_random.col, 2);
        iluminar_manhattan(juego, punto_random.fil-2, punto_random.col, 1);
        iluminar_manhattan(juego, punto_random.fil-3, punto_random.col, 0);

        juego->personaje.mochila[posicion_bengala].movimientos_restantes--;

    } else {
        activar_herramienta(&juego->personaje.elemento_en_uso, posicion_bengala);
        quitar_herramienta_de_mochila(&juego->personaje, posicion_bengala);
    }
}

/* precondiciones: int elemento_en_uso debe ser posicion de linterna, posicion de vela o posicion de bengala.
 * postcondiciones: ilumina porciones del terreno dependiendo la herramienta que esta activada. 
 */
void iluminar_terreno(juego_t* juego){

    if(juego->personaje.elemento_en_uso == posicion_herramienta(juego->personaje, LINTERNA)){
        iluminar_linterna(juego);

    } else if(juego->personaje.elemento_en_uso == posicion_herramienta(juego->personaje, VELA)){
        iluminar_vela(juego);

    } else if(juego->personaje.elemento_en_uso == posicion_herramienta(juego->personaje, BENGALA)){
        iluminar_bengala(juego);
    }
}

/* precondiciones: - char jugada debe ser DERECHA, IZQUIERDA, ARRIBA o ABAJO.
 *                 - la fila o columna del personaje puede modificar su valor en un rango de +-1 al valor de la fila/columna actual.
 * postcondiciones: - se modifica el valor de la fila o la columna del personaje dependiendo el valor de jugada. 
 */
void mover_personaje(coordenada_t* posicion_oso, char jugada) {
    if(jugada == ARRIBA){
        posicion_oso->fil--;

    } else if(jugada == ABAJO){
        posicion_oso->fil++;

    } else if(jugada == IZQUIERDA){
        posicion_oso->col--;

    } else if(jugada == DERECHA){
        posicion_oso->col++;
    }  
}

void realizar_jugada(juego_t* juego, char jugada) {
    int posicion_linterna = posicion_herramienta(juego->personaje, LINTERNA);
    int posicion_vela = posicion_herramienta(juego->personaje, VELA);
    int posicion_bengala = posicion_herramienta(juego->personaje, BENGALA);

    limpiar_terreno(juego);

    if(jugada == IZQUIERDA || jugada == DERECHA || jugada == ARRIBA || jugada == ABAJO){

        if(!esta_en_frontera(jugada, juego->personaje.posicion)){
            juego->personaje.ultimo_movimiento = jugada;

            if(juego->personaje.elemento_en_uso != -1){
                iluminar_terreno(juego);
            }
            mover_personaje(&juego->personaje.posicion, jugada);

            if(posicion_es_herramienta(juego)){
                recolectar_herramienta(juego);

            } else if(posicion_es_obstaculo(juego)){
                char obstaculo = tipo_obstaculo(juego);

                if(obstaculo == PIEDRA || obstaculo == ARBOL){
                    sumar_segundos(&juego->personaje, obstaculo);

                } else if(obstaculo == KOALA){
                    reubicar_personaje(&juego->personaje.posicion.fil, &juego->personaje.posicion.col);
                }
            }
        } else {
            printf(" No te podes ir del terreno bro\n");
        }
        
    } else if(jugada == LINTERNA){
        if(juego->personaje.elemento_en_uso != posicion_bengala){
            activar_herramienta(&juego->personaje.elemento_en_uso, posicion_linterna);

            if(juego->personaje.elemento_en_uso == posicion_linterna && juego->personaje.mochila[posicion_linterna].movimientos_restantes > 0){
                iluminar_linterna(juego);
                agregar_koala(juego);
            } else {
                printf("Ups, te quedaste sin bateria :(\n");
            }
        } else {
            iluminar_bengala(juego);
            printf("No podes activar la linterna hasta que la bengala no se apague, emocion\n");
        }
        
    } else if(jugada == VELA){
        if(posicion_vela != NO_ENCONTRADO){
            if(juego->personaje.elemento_en_uso != posicion_bengala){
                activar_herramienta(&juego->personaje.elemento_en_uso, posicion_vela);
                if(juego->personaje.elemento_en_uso == posicion_vela){
                    iluminar_vela(juego);
                }
            } else {
                iluminar_bengala(juego);
                printf("Easy bro, para prender la vela espera a que se apague la linterna\n");
            }
        } else {
            printf(" Te quedaste sin velas :(\n");
        }        

    } else if(jugada == BENGALA){
        if(posicion_bengala != NO_ENCONTRADO){
            if(juego->personaje.elemento_en_uso != posicion_bengala){
                activar_herramienta(&juego->personaje.elemento_en_uso, posicion_bengala);
                iluminar_bengala(juego);
            } else {
                iluminar_bengala(juego);
                printf("No podes desactivar la bengala, tenes que esperar a que termine\n");
            }
        } else {
            printf(" No tenes bengalas :(\n");
        }

    } else if(jugada == TIEMPO_ACTUAL){
        printf("Tiempo utilizado: % .1f segundos\n", tiempo_actual());
    }

    if((juego->personaje.tipo == PANDA) && (juego->personaje.tiempo_perdido >= 30)){
        juego->chloe_visible = true;
    }
}

int estado_juego(juego_t juego) {
    int estado_de_juego = 0;
    if((juego.personaje.posicion.fil == juego.amiga_chloe.fil) && (juego.personaje.posicion.col == juego.amiga_chloe.col)){
        estado_de_juego = -1;
    }
    return estado_de_juego;
}