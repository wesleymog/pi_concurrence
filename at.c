#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <math.h>
#include "timer.h"

int nthreads;
long int n;

void *tarefa(void *arg){
    long int id = (long int) arg;
    double *somaLocal;
    long int tamBloco = n/nthreads;
    long int ini = id*tamBloco;
    long int fim;
    double aux;


    somaLocal = (double*) malloc(sizeof(double));
    if(somaLocal == NULL){
        fprintf(stderr, "ERRO = MALLOC\n");
        exit(1);
    }
    *somaLocal = 0;
    if(id == nthreads-1)
        fim = n;
    else
        fim = ini+tamBloco;

    //soma de pi
    aux = ini;
    if((int)aux%2==0)
        aux+=1;
    for(long int i=ini;i<=fim;i++){
        if(i%2!=0)
        *somaLocal-=4/aux;
        else
        *somaLocal+=4/aux;
        aux=aux+2;
    }
    pthread_exit((void *) somaLocal);
}

int main(int argc, char *argv[]){
    double somaSeq  = 0;
    double somaConc  = 0;
    double ini, fim;
    pthread_t *tid;
    double *retorno;
    double aux1;

    if(argc < 3){
        fprintf(stderr, "Digite: %s <tamanho de n> <numero de threads>\n", argv[0]);
        return 1;
    }
    n = atoi(argv[1]);
    nthreads = atoi(argv[2]);
    //soma sequencial
    GET_TIME(ini);
    aux1 = 1;

    for(long int i=1;i<=n;i++){
        if(i%2!=0)
        somaSeq=somaSeq+4/aux1;
        else
        somaSeq=somaSeq-4/aux1;
        aux1=aux1+2;
    }
    //Soma não seq
    GET_TIME(fim);
    printf("Tempo sequencial: %lf\n" , fim-ini);
    GET_TIME(ini);
    tid  =(pthread_t *) malloc(sizeof(pthread_t) *nthreads);
    if(tid == NULL){
        fprintf(stderr, "ERRO - MALLOC\n");
        return 2;
    }
    for(long int i = 0;i<nthreads; i++){
        if(pthread_create(tid+i, NULL, tarefa, (void*) i)){
            fprintf(stderr, "ERRO - pthread create");
            return 3;
        }
    }
    for(long int i = 0;i<nthreads; i++){
        if(pthread_join(*(tid+i),(void **) &retorno)){
            fprintf(stderr, "ERRO - pthread join");
            return 3;
        }
        somaConc += *retorno;
        free(retorno);
    }
    GET_TIME(fim);
    printf("Tempo concorrente: %lf\n" , fim-ini);
    
    //resultados
    printf("Soma sequencial: %.15lf\n", somaSeq);
    printf("Soma concorrent: %.15lf\n", somaConc);
    printf("Valor de pi do math pi: %.15lf\n", M_PI);

    free(tid);
    return 0;
}