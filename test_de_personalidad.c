#include <stdio.h>
#include "test_de_personalidad.h"

#define POLAR_EMOJI "\u2744\uFE0F"
#define PANDA_EMOJI "\U0001f43c"
#define PARDO_EMOJI "\U0001f43b"

const char POLAR = 'I', PANDA = 'P', PARDO = 'G';

const char LIMPIEZA = 'L', ANIME = 'A', MUSICA_POP = 'M';

const int PUNTOS_LIMPIEZA = 1, PUNTOS_ANIME = 2, PUNTOS_MUSICA_POP = 3;

const char FOCAS = 'F', BAMBU = 'B', PESCADO = 'P';

const int PUNTOS_FOCAS = 3, PUNTOS_BAMBU = 6, PUNTOS_PESCADO = 9;

const int PISO_MINIMO = 1, PISO_MAXIMO = 18;

const int POTENCIA_GRITO_MINIMA = 1, POTENCIA_GRITO_MAXIMA = 18;

const int RANGO_MINIMO_POLAR = 5, RANGO_MAXIMO_POLAR = 24;
const int RANGO_MINIMO_PANDA = 25, RANGO_MAXIMO_PANDA = 43;
const int RANGO_MINIMO_PARDO = 44, RANGO_MAXIMO_PARDO = 63;


/* PRE CONDICIONES: la variable char canal_television debe estar inicializada.
 * POST CONDICIONES: modifica el valor de la variable char canal_television con el valor ingresado por el usuario(si este no era valido). 
 */
void validar_canal_ingresado(char *canal_television) {
    while((*canal_television != LIMPIEZA) && (*canal_television != ANIME) && (*canal_television != MUSICA_POP)) {
        printf("El caracter ingresado no es valido. Porfis, ingresa una letra que este entre las opciones (%c, %c, o %c):\n", LIMPIEZA, ANIME, MUSICA_POP);
        scanf(" %c", canal_television);
    }
}
 
/* PRE CONDICIONES: -
 * POST CONDICIONES: inicializa la variable char canal_television.
 */
void elegir_canal_television(char *canal_television) {
    printf("Vas a ver televisión un rato, pones el canal de: Limpieza(%c), Anime(%c), Musica Pop(%c)\n", LIMPIEZA, ANIME, MUSICA_POP);
    scanf(" %c", canal_television);

    validar_canal_ingresado(canal_television);
}

/* PRE CONDICIONES: la variable char vianda_alimento debe estar inicializada.
 * POST CONDICIONES: modifica el valor de la variable char vianda_alimento con el valor ingresado por el usuario(si este no era valido).
 */
void validar_alimento_guardado(char* vianda_alimento) {
    while((*vianda_alimento) != FOCAS && (*vianda_alimento) != BAMBU && (*vianda_alimento) != PESCADO) {
        printf("No tenemos ningun alimento con esa letra, por favor ingresa otra (%c, %c, %c):\n", FOCAS, BAMBU, PESCADO);
        scanf(" %c", vianda_alimento);
    }
}

/* PRE CONDICIONES: -
 * POST CONDICIONES: inicializa la variable char vianda_alimento.
 */
void guardar_alimento(char* vianda_alimento) {
    printf("Solo podes guardar un alimento en tu vianda. Guardas: Focas(%c), Bambu(%c), Pescado(%c)\n", FOCAS, BAMBU, PESCADO);
    scanf(" %c", vianda_alimento);

    validar_alimento_guardado(vianda_alimento);
}

/* PRE CONDICIONES: la variable int piso_torre debe estar inicializada.
 * POST CONDICIONES: modifica el valor de la variable int piso_torre con el valor ingresado por el usuario (si este no era valido).
 */
void validar_piso_torre(int* piso_torre) {
    while(((*piso_torre) < PISO_MINIMO) || ((*piso_torre) > PISO_MAXIMO)) {
        printf("Por favor, ingresa un numero de piso que sea valido (%i-%i)\n", PISO_MINIMO, PISO_MAXIMO);
        scanf("%i", piso_torre);
    }
}

/* PRE CONDICIONES: -
 * POST CONDICIONES: inicializa la variable int piso_torre con un dato ingresado por el usuario.
 */
void elegir_piso_torre(int* piso_torre) {
    printf("Te compras una torre con tus dos hermanos de 18 pisos. ¿En que piso te gustaría vivir? (1-18)\n");
    scanf("%i", piso_torre);

    validar_piso_torre(piso_torre);
}

/* PRE CONDICIONES: la variable int potencia_grito debe estar inicializada.
 * POST CONDICIONES: modifica el valor de la variable int potencia_grito con el valor ingresado por el usuario(si este no era valido).
 */
void validar_potencia_grito(int* potencia_grito) {
    while(((*potencia_grito) < POTENCIA_GRITO_MINIMA) || ((*potencia_grito) > POTENCIA_GRITO_MAXIMA)) {
        printf("Tu potencia de grito no esta en el rango. Porfis, ingresa un numero valido (%i-%i)\n", POTENCIA_GRITO_MINIMA, POTENCIA_GRITO_MAXIMA);
        scanf("%i", potencia_grito);
    }
}

/* PRE CONDICIONES: -
 * POST CONDICIONES: Inicializa la variable int potencia_grito con un numero ingresado por el usuario.
 */
void calcular_potencia_grito(int* potencia_grito) {
    printf("¡Oh, una rata! ¿Que tan fuerte gritas del %i al %i? Siendo %i no gritar y %i desgarrarse la garganta.\n", POTENCIA_GRITO_MINIMA, POTENCIA_GRITO_MAXIMA, POTENCIA_GRITO_MINIMA, POTENCIA_GRITO_MAXIMA);
    scanf("%i", potencia_grito); 

    validar_potencia_grito(potencia_grito);
}

/* PRE CONDICIONES: los puntos del canal de television deben ser 1 (L), 2 (A) o 3 (M).
 * POST CONDICIONES: retorna el valor de los puntos del canal de television elegido por el usuario.
 */
int puntos_television(char canal_television) {
    int puntos_television;

    if(canal_television == LIMPIEZA) {
        puntos_television = PUNTOS_LIMPIEZA;

    } else if(canal_television == ANIME) {
        puntos_television = PUNTOS_ANIME;

    } else if(canal_television == MUSICA_POP) {
        puntos_television = PUNTOS_MUSICA_POP;
    } 
    return puntos_television;
}

/* PRE CONDICIONES: los puntos deben ser 3 (F), 6 (B) o 9 (P).
 * POST CONDICIONES: retorna el valor de los puntos obtenidos por el alimento elegido por el usuario.
 */
int puntos_alimento(char vianda_alimento) {
    int puntos_vianda_alimento;

    if(vianda_alimento == FOCAS) {
        puntos_vianda_alimento = PUNTOS_FOCAS;

    } else if(vianda_alimento == BAMBU) {
        puntos_vianda_alimento = PUNTOS_BAMBU;

    } else if(vianda_alimento == PESCADO) {
        puntos_vianda_alimento = PUNTOS_PESCADO;
    }
    return puntos_vianda_alimento;
}

/* PRE CONDICIONES: recibe los puntos del canal de television, el alimento guardado, el piso de la torre, la potencia de grito (previamente inicializados y con valores validos) y el puntaje total.
 * POST CONDICIONES: calcula el puntaje total del test.
                     la variable int puntaje_total tendra un valor entre 5 y 63.
 */
void calcular_puntaje_test(int puntos_television, int puntos_vianda, int puntos_torre, int puntos_grito, int* puntaje_total) {
    (*puntaje_total) = (puntos_television * puntos_vianda) + puntos_torre + puntos_grito; 
}


/* PRE CONDICIONES: el resultado puede ser POLAR, PANDA o PARDO.
 * POST CONDICIONES: imprime por pantalla el resultado del test segun los puntos obtenidos.
 */
void imprimir_resultado_test(int puntaje_total, char* personalidad_detectada) {
    printf("\n");
    if(puntaje_total >= RANGO_MINIMO_POLAR && puntaje_total <= RANGO_MAXIMO_POLAR) {
        printf("Personalidad detctada: POLAR %s \n\n", POLAR_EMOJI);
        (*personalidad_detectada) = POLAR;

    } else if(puntaje_total >= RANGO_MINIMO_PANDA && puntaje_total <= RANGO_MAXIMO_PANDA) {
        printf("Personalidad detectada: PANDA %s \n\n", PANDA_EMOJI);
        (*personalidad_detectada) = PANDA;

    } else if(puntaje_total >= RANGO_MINIMO_PARDO && puntaje_total <= RANGO_MAXIMO_PARDO) {
        printf("Personalidad detectada: PARDO %s \n\n", PARDO_EMOJI);
        (*personalidad_detectada) = PARDO;
    }
}

/* PRECONDICIONES: la variable char personalidad_detectada debe estar inicializada.
 * POSTCONDICIONES: segun la personalidad detectada al realizar el test, modifica el valor de la variable char personalidad_detectada.
 * Las personalidades seran POLAR, PANDA o PARDO. 
 */
void test_de_personalidad(char* personalidad_detectada) {

    char canal_television, vianda_alimento;
    int piso_torre, potencia_grito;
    int puntaje_total;
    
    elegir_canal_television(&canal_television);
    guardar_alimento(&vianda_alimento);
    elegir_piso_torre(&piso_torre);
    calcular_potencia_grito(&potencia_grito);

    calcular_puntaje_test(puntos_television(canal_television), puntos_alimento(vianda_alimento), piso_torre, potencia_grito, &puntaje_total);

    imprimir_resultado_test(puntaje_total, personalidad_detectada);
}

