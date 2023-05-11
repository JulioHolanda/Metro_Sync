#include <pthread.h>

struct estacao {
    int passageiros;
    pthread_mutex_t mutex;
    pthread_cond_t passageiros_cond;
    pthread_cond_t vagao_cond
};

void estacao_init(struct estacao *estacao) {
    estacao->passageiros = 0;
    pthread_mutex_init(&(estacao->mutex), NULL);
    pthread_cond_init(&(estacao->passageiros_cond),NULL);
    pthread_cond_init(&(estacao->vagao_cond), NULL);
}

void estacao_preencher_vagao(struct estacao * estacao, int assentos) {
    
    printf("vagao chegou\n");

    pthread_mutex_lock(&(estacao->mutex));
    
    while(assentos > 0 && estacao->passageiros > 0){
        
        pthread_cond_broadcast(&(estacao->passageiros_cond));
        pthread_cond_wait(&(estacao->vagao_cond), &(estacao->mutex));
        assentos--;
    }

    pthread_mutex_unlock(&(estacao->mutex));

    printf("vagao SAIU\n");

}

void estacao_espera_pelo_vagao(struct estacao * estacao) {
    
    printf("passageiro em espera\n");

    pthread_mutex_lock(&(estacao->mutex));
    
    pthread_mutex_unlock(&(estacao->mutex));

    printf("passageiro foi ao EMBARQUE\n");
}

void estacao_embarque(struct estacao * estacao) {

    pthread_mutex_lock(&estacao->mutex);
    pthread_cond_signal(&(estacao->vagao_cond));
    pthread_mutex_unlock(&estacao->mutex);

    printf("EMBRAQUE\n");

}