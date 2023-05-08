#include <pthread.h>

struct estacao {
    int vagas;
    int passageiros;
    int passageiros_embarcados;
    pthread_mutex_t mutex;
    pthread_cond_t passageiros;
    pthread_cond_t vagao;
    //pthread_cond_t aviso;
};

void estacao_init(struct estacao *estacao) {
    
    estacao->vagas = 0;
    estacao->passageiros = 0;
    estacao->passageiros_embarcados = 0;
    pthread_mutex_init(&(estacao->mutex), NULL);
    pthread_cond_init(&(estacao->passageiros), NULL);   
    pthread_cond_init(&(estacao->vagao), NULL);
    //pthread_cond_init(&(estacao->aviso), NULL);
}

void estacao_preecher_vagao(struct estacao * estacao, int assentos) {
    
    pthread_mutex_lock(&(estacao->mutex));
    estacao->vagas = assentos;
    pthread_cond_broadcast(&(estacao->passageiros)); //avisa a todos passageiros q vagao chegou
    pthread_cond_wait(&(estacao->vagao),&(estacao->mutex)); //espera entrarem
    pthread_mutex_unlock(&(estacao->mutex));

    return NULL;
}

void estacao_espera_pelo_vagao(struct estacao * estacao) {

    pthread_mutex_lock(&(estacao->mutex));
    estacao->passageiros++;
    while(estacao->vagas == 0){        //sem vagas -> block
        pthread_cond_wait(&(estacao->passageiros),&(estacao->mutex));
    }
    estacao->vagas--; //tem vaga -> consome vaga
    estacao->passageiros--;
    estacao->passageiros_embarcados++;
    
    pthread_mutex_unlock(&(estacao->mutex));

    return NULL;    
}

void estacao_embarque(struct estacao * estacao) {

    pthread_mutex_lock(&(estacao->mutex));
    estacao->passageiros_embarcados--;
    if(estacao->passageiros_embarcados == 0 || estacao->passageiros == 0)
        pthread_cond_signal(&(estacao->vagao));  //avisa vagao q pode sair
    pthread_mutex_unlock(&(estacao->mutex));

    return NULL;

}