#include "test_de_personalidad.h"
#include "osos_contra_reloj.h"
#include "utiles.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define PANDA 'P'
#define CHLOE_EMOJI "\U0001f467"
#define POLAR_EMOJI "\u2744\uFE0F"
#define PANDA_EMOJI "\U0001f43c"
#define PARDO_EMOJI "\U0001f43b"

#define IZQUIERDA "\u2B05\uFE0F"
#define DERECHA "\u27A1\uFE0F"
#define ARRIBA "\u2B06\uFE0F"
#define ABAJO "\u2B07\uFE0F"
#define LINTERNA "\U0001f526"
#define VELA "\U0001f56f\uFE0F"
#define BENGALA "\U0001f387"
#define TIEMPO "\u23F3"
const double TIEMPO_MAXIMO = 120;

int main() {
    char personalidad = PANDA;
    //test_de_personalidad(&personalidad);

    srand ((unsigned)time(NULL));
    juego_t juego;

    inicializar_juego(&juego, personalidad);
    char jugada;

    printf("\n");
    printf(" BIENVENIDOS A OSOS CONTRA RELOJ! %s  %s %s %s \n\n", POLAR_EMOJI, PANDA_EMOJI, PARDO_EMOJI, TIEMPO);
    printf(" Ya se hizo de noche y Chloe esta perdida en el bosque, sola, y hace mucho frio. ");
    printf(" Solo tenes 120 segundos para encontrarla, sino, se va a morir congelada. \n");
    printf(" Acaso la vas a dejar que se congele??? ANDA A BUSCARLA YA!!\n");

    mostrar_juego(juego);
    iniciar_cronometro();

    while(estado_juego(juego) != -1) {
        printf(" Hacia donde te queres mover? %s (A), %s (D), %s (W), %s (S)\n", IZQUIERDA, DERECHA, ARRIBA, ABAJO);
        printf(" O podes activar una herramienta: %s (L), %s  (V), %s (E)\n", LINTERNA, VELA, BENGALA);
        printf(" Sino, podes ver cuanto tiempo llevas jugando %s (T)\n", TIEMPO);
        scanf(" %c", &jugada);

        while(!es_jugada_valida(jugada)){
            printf("Ingresa una jugada valida pls (W, A, S, D, L, V, E)\n");
            scanf(" %c", &jugada);
        }
        system("clear");
        realizar_jugada(&juego, jugada);
        mostrar_juego(juego);
    }

    double tiempo_total = detener_cronometro();
    tiempo_total += juego.personaje.tiempo_perdido;

    if((tiempo_total > 0) && (tiempo_total <= TIEMPO_MAXIMO)){
        printf(" Felicitaciones, ganaste!!!\n");
        printf(" Tiempo total empleado: % .1f segundos\n", tiempo_total);
        printf(" Tiempo perdido en obstaculos: % .1f segundos\n", juego.personaje.tiempo_perdido);

    } else {
        printf("Luuuser, perdiste, tardaste % .1f segundos en encontrar a Chloe. Volve a jugar kpo ;)\n", tiempo_total);
    }
    return 0;
} 