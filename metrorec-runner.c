#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "metrorec.c"

void *passageiros_thread(void *arg)
{
	struct estacao *estacao = (struct estacao *) arg;
	estacao_espera_pelo_vagao(estacao);
	return NULL;
}

struct vagao_args {
	struct estacao *estacao;
	int assentos_livres;
};

void *vagao_thread(void *args)
{
	struct vagao_args *vargs = (struct vagao_args *) args;
	estacao_preecher_vagao(vargs->estacao, vargs->assentos_livres);
	return NULL;
}

int main(void)
{

        //
        // create one station
        //
        struct estacao station;
        estacao_init(&station);


        //
        // create a number of passengers
        // each passenger is a thread
        //
        int qtd_passageiros = 6;

        pthread_t passageiros[qtd_passageiros];
        int t[qtd_passageiros];

        for(int i=0; i<qtd_passageiros; i++){
                t[i] = pthread_create(&passageiros[i], NULL, passageiros_thread, &station);
                
                if(t[i]){
                        fprintf(stderr, "Error - pthread create() return code: %d\n", t[i]);
                        exit(EXIT_FAILURE);
                }
        }
        

        //
        // loop to create as many car as necessary to board all passengers
        //
        for (int i=0; i<3; i++){
                
                //
                // create only one car with a number of free seats
                // this car is associated to a thread
                //
                struct vagao_args vagao_struct;
                int qtd_assentos = qtd_passageiros / (1+i);

                vagao_struct.estacao = &station;
                vagao_struct.assentos_livres = qtd_assentos; 

                pthread_t vagao;
                int v = pthread_create(&vagao, NULL, &vagao_thread, &vagao_struct);
                if (v){
                        fprintf(stderr, "Error -pthread_create() return code: %d\n", v);
                        exit(EXIT_FAILURE);
                }
                

                //
                // define the number of passenger to reap
                // minimum between number of free seats and passengers at station still waiting to board
                //
                int passageiros_reap;
                if(qtd_assentos < qtd_passageiros){
                        passageiros_reap = qtd_assentos;
                        qtd_passageiros -= qtd_assentos;
                }else{
                        passageiros_reap = qtd_passageiros;
                }

                //
                // for each thread associated to a passenger that finished
                // call estacao_embarque function to let the car know that the passenger is on board
                // ATTENTION: the car can not have more passengers than the number of free seats
                //
                for(int i = 0; i<passageiros_reap; i++){
                        estacao_embarque(&station);
                }

                pthread_join(vagao, NULL);
        
        }

        return 0;
}