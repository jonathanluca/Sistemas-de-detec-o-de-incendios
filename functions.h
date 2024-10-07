#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#define N 30 // Tamanho da matriz

typedef struct {
    int x, y;
    char estado; // '-': área livre, 'T': sensor ativo, '@': fogo ativo, '/': queimado
    pthread_mutex_t lock;
    pthread_cond_t cond;
} Celula;

extern Celula matriz[N][N];
extern pthread_mutex_t monitor;

void inicializar_matriz(Celula matriz[N][N]);
void imprimir_matriz(Celula matriz[N][N]);
void* sensor_thread(void* arg);
void* central_thread(void* arg);
void* gerador_incendios_thread(void* arg);

#endif