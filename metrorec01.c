// versao inicial. estacao_embarque(struct estacao * estacao) controla qtd de passageiros
// no vagao e o libera

#include <pthread.h>

struct estacao {
    int vagas;
    int passageiros;
    int passageiros_embarcados;
    pthread_mutex_t mutex;
    pthread_cond_t passageiros_cond;
    pthread_cond_t vagao_cond;
    pthread_cond_t aviso_cond;
};

void estacao_init(struct estacao *estacao) {
    
    estacao->vagas = 0;
    estacao->passageiros = 0;
    estacao->passageiros_embarcados = 0;
    pthread_mutex_init(&(estacao->mutex), NULL);
    pthread_cond_init(&(estacao->passageiros_cond), NULL);   
    pthread_cond_init(&(estacao->vagao_cond), NULL);
    pthread_cond_init(&(estacao->aviso_cond), NULL);
}

void estacao_preencher_vagao(struct estacao * estacao, int assentos) {
    
    printf("vagao chegou\n");

    if (assentos > 0){
        pthread_mutex_lock(&(estacao->mutex));
        estacao->vagas = assentos;
        pthread_cond_broadcast(&(estacao->passageiros_cond)); //avisa a todos passageiros q vagao chegou E permite embarque
        pthread_cond_wait(&(estacao->vagao_cond),&(estacao->mutex)); //espera entrarem
        pthread_mutex_unlock(&(estacao->mutex));
    }

    printf("vagao SAIU\n");

}

void estacao_espera_pelo_vagao(struct estacao * estacao) {

    pthread_mutex_lock(&(estacao->mutex));
    
    printf("passageiro esperando\n");

    estacao->passageiros++;
    while(estacao->vagas == 0){        //sem vagas -> block
        pthread_cond_wait(&(estacao->passageiros_cond),&(estacao->mutex));
    }
    estacao->vagas--; //tem vaga -> consome vaga
    estacao->passageiros--;
    estacao->passageiros_embarcados++;

    if (estacao->vagas == 0 || estacao->passageiros == 0){
        pthread_cond_signal(&(estacao->aviso_cond));
    }
    
    pthread_mutex_unlock(&(estacao->mutex));

    printf("passageiro foi ao EMBARQUE\n");

}

void estacao_embarque(struct estacao * estacao) {

    pthread_mutex_lock(&(estacao->mutex));
    estacao->passageiros_embarcados--;
    if(estacao->passageiros_embarcados == 0 || estacao->passageiros == 0)
        pthread_cond_signal(&(estacao->vagao_cond));  //avisa vagao q pode sair
    pthread_mutex_unlock(&(estacao->mutex));

    printf("EMBRAQUE\n");

}