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

	
        //
        // create a number of passengers
        // each passenger is a thread
        //
        
        
        //
        // loop to create as many car as necessary to board all passengers
        //

                //
                // create only one car with a number of free seats
                // this car is associated to a thread
                //

                //
                // define the number of passenger to reap
                // minimum between number of free seats and passengers at station still waiting to board
                //

                //
                // for each thread associated to a passenger that finished
                // call estacao_embarque function to let the car know that the passenger is on board
                // ATTENTION: the car can not have more passengers than the number of free seats
                //

        return 0;
}