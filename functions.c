#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "functions.h"

#define N 30 // Tamanho da matriz

void inicializar_matriz(Celula matriz[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            matriz[i][j].x = i;
            matriz[i][j].y = j;
            matriz[i][j].estado = '-';
            pthread_mutex_init(&matriz[i][j].lock, NULL);
            pthread_cond_init(&matriz[i][j].cond, NULL);
        }
    }
}

void imprimir_matriz(Celula matriz[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%c ", matriz[i][j].estado);
        }
        printf("\n");
    }
}

void* sensor_thread(void* arg) {
    Celula* celula = (Celula*)arg;
    while (1) {
        pthread_mutex_lock(&celula->lock);
        while (celula->estado != '@') {
            pthread_cond_wait(&celula->cond, &celula->lock);
        }
        printf("Incêndio detectado na célula (%d, %d)\n", celula->x, celula->y);
        // Propagar mensagem para vizinhos ou thread central
        if (celula->x == 0 || celula->x == N-1 || celula->y == 0 || celula->y == N-1) {
            // Enviar mensagem para a central
            pthread_mutex_lock(&monitor);
            printf("Incêndio na borda na célula (%d, %d) reportado à central\n", celula->x, celula->y);
            pthread_mutex_unlock(&monitor);
        } else {
            // Propagar para vizinhos
            int dx[] = {-1, 1, 0, 0};
            int dy[] = {0, 0, -1, 1};
            for (int i = 0; i < 4; i++) {
                int nx = celula->x + dx[i];
                int ny = celula->y + dy[i];
                pthread_mutex_lock(&matriz[nx][ny].lock);
                if (matriz[nx][ny].estado == '-') {
                    matriz[nx][ny].estado = '@';
                    pthread_cond_signal(&matriz[nx][ny].cond);
                }
                pthread_mutex_unlock(&matriz[nx][ny].lock);
            }
        }
        pthread_mutex_unlock(&celula->lock);
        sleep(1);
    }
    return NULL;
}

void* central_thread(void* arg) {
    while (1) {
        // Receber mensagens de sensores na borda
        // Registrar eventos de incêndio e iniciar combate ao fogo
        sleep(1);
    }
    return NULL;
}

void* gerador_incendios_thread(void* arg) {
    while (1) {
        int x = rand() % N;
        int y = rand() % N;
        pthread_mutex_lock(&matriz[x][y].lock);
        if (matriz[x][y].estado == '-') {
            matriz[x][y].estado = '@';
            printf("Incêndio gerado na célula (%d, %d)\n", x, y);
            pthread_cond_signal(&matriz[x][y].cond);
        }
        pthread_mutex_unlock(&matriz[x][y].lock);
        sleep(INTERVALO_INCENDIO);
    }
    return NULL;
}