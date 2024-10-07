#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "functions.h"

#define N 30 // Tamanho da matriz
#define INTERVALO_INCENDIO 3 // Intervalo para geração de incêndios

Celula matriz[N][N];
pthread_mutex_t monitor;

int main() {
    pthread_t sensores[N][N];
    pthread_t central, gerador_incendios;

    pthread_mutex_init(&monitor, NULL);

    // Inicializar matriz de sensores
    inicializar_matriz(matriz);

    // Criar threads de sensores
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            pthread_create(&sensores[i][j], NULL, sensor_thread, &matriz[i][j]);
        }
    }

    // Inicializar thread central
    pthread_create(&central, NULL, central_thread, NULL);

    // Inicializar thread de geração de incêndios
    pthread_create(&gerador_incendios, NULL, gerador_incendios_thread, NULL);

    // Loop para imprimir a matriz periodicamente
    while (1) {
        system("clear");
        imprimir_matriz(matriz);
        sleep(1);
    }

    // Esperar threads terminarem (nunca vão terminar neste exemplo)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            pthread_join(sensores[i][j], NULL);
        }
    }
    pthread_join(central, NULL);
    pthread_join(gerador_incendios, NULL);

    pthread_mutex_destroy(&monitor);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            pthread_mutex_destroy(&matriz[i][j].lock);
            pthread_cond_destroy(&matriz[i][j].cond);
        }
    }

    return 0;
}