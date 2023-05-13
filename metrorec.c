// versao 2. seguindo a risca "embarque_estacao() informa ao vagao
// que o passageiro embarcou"

#include <pthread.h>

struct estacao {
    int vagas;
    int passageiros;
    int saiu_para_embarque;
    int embarque_em_andamento;
    pthread_mutex_t mutex;
    pthread_cond_t passageiros_cond;
    pthread_cond_t vagao_cond;
    pthread_cond_t embarque_cond;
};

void estacao_init(struct estacao *estacao) {
    estacao->embarque_em_andamento = 0;
    estacao->vagas = 0;
    estacao->passageiros = 0;
    estacao->saiu_para_embarque = 0;
    pthread_mutex_init(&(estacao->mutex), NULL);
    pthread_cond_init(&(estacao->passageiros_cond),NULL);
    pthread_cond_init(&(estacao->vagao_cond), NULL);
    pthread_cond_init(&(estacao->embarque_cond), NULL);
}

void estacao_preencher_vagao(struct estacao * estacao, int assentos) {
    
    printf("2- vagao chegou\n");

    pthread_mutex_lock(&(estacao->mutex));

    if(assentos > 0){
        estacao->vagas = assentos;
        pthread_cond_broadcast(&(estacao->passageiros_cond));

        while(assentos > 0 && estacao->passageiros > 0){
            pthread_cond_broadcast(&(estacao->embarque_cond)); // Isso pode dar bronca
            printf(" %d\n", assentos);
            pthread_cond_wait(&(estacao->vagao_cond), &(estacao->mutex));
            assentos--;
            estacao->embarque_em_andamento = 0;
        }
    }    

    printf("2- vagao SAIU\n");
    pthread_mutex_unlock(&(estacao->mutex));


}

void estacao_espera_pelo_vagao(struct estacao * estacao) {
    
    printf("2- passageiro em espera\n");

    pthread_mutex_lock(&(estacao->mutex));
    estacao->passageiros++;

    while(estacao->vagas == 0){
        pthread_cond_wait(&(estacao->passageiros_cond), &(estacao->mutex));
    }
    estacao->vagas--;
    estacao->saiu_para_embarque++;

    pthread_mutex_unlock(&(estacao->mutex));

    printf("2- passageiro foi ao EMBARQUE\n");
}

void estacao_embarque(struct estacao * estacao) {

    printf(" %d passageios na estacao\n", estacao->passageiros);
    pthread_mutex_lock(&estacao->mutex);
    
    while (estacao->embarque_em_andamento == 1){
        pthread_cond_wait(&(estacao->embarque_cond), &(estacao->mutex));
    }

    if(estacao->saiu_para_embarque > 0){
        estacao->embarque_em_andamento = 1;
        pthread_cond_signal(&(estacao->vagao_cond));
        estacao->passageiros--;
        estacao->saiu_para_embarque--;
        
    }

    printf("2- EMBRAQUE\n");
    pthread_mutex_unlock(&estacao->mutex);

}