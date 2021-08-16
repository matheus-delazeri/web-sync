#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

sem_t sem_wait_list, sem_connect;
int *clients_waiting;

typedef struct{
    int *id;
} client_info;

void show_wait_list() {
    printf("[Cliente] esta realizando um solicitacao de conexao!\n");
}

void * try_to_connect(void *args){
    client_info *actual_args = args;
    int client_id = *(actual_args->id);
    printf("[Cliente %i] esta realizando um solicitacao de conexao!\n", client_id);
    if(sem_trywait(&sem_wait_list)==0) { 
        sem_wait(&sem_connect);
        int use_time = (rand() % 3)+1;
        sem_post(&sem_wait_list);
        printf("[Cliente %i] estabeleceu conexao com a rede!\n", client_id);


        sem_post(&sem_connect);

    }



    free(actual_args);
}

int main(int argc, char **argv) {
    int num_wait, client_count=0, creation_time;
    pthread_t client;
    if (argc == 2 && strtol(argv[1], NULL, 10) != 0) {
        num_wait = abs(strtol(argv[1], NULL, 10));
    } else if(argc > 1 && strtol(argv[1], NULL, 10)==0) {
        printf("O numero de 'esperas' disponiveis deve ser maior que 0.\n");
        exit(0);
    }
    else {
        printf("Eh necessario passar o numero de 'esperas' disponiveis no sistema por parametro na execucao do codigo\n");
        printf("Ex.: ./<file-name> 5\n");
        exit(0);
    }
    clients_waiting = (int*)malloc(num_wait * sizeof(int));
    sem_init(&sem_wait_list, 0, num_wait);
    sem_init(&sem_connect, 0, 1);
    while(1) {
        creation_time = (rand() % 2)+1;
        client_info *args = malloc(sizeof *args);
        args->id = &client_count;
        sleep(creation_time);
        pthread_create(&client, NULL, try_to_connect, args);
        client_count++;
    }

    sem_destroy(&sem_wait_list);
    sem_destroy(&sem_connect);
    free(clients_waiting);
    
}
