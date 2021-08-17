#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

sem_t sem_wait_list, sem_connect, sem_manage_var;
int *clients_waiting, wait_list_index=0, wait_list_size;

typedef struct{
    int *id;
} client_info;

void show_wait_list() {
    int is_empty;
    printf("\nClientes em espera:\n");
    printf("---------------------\n");
    for(int i=0; i<wait_list_size; i++){
        is_empty = clients_waiting[i]==0 ? 1 : 0;
        if(is_empty==1){
            printf("Slot [%i] - \n", i);
        } else {
            printf("Slot [%i] - Cliente %i\n", i, clients_waiting[i]);
        }
    }
    printf("-------------------------------------------------------\n");
}

void * try_to_connect(void *args){
    client_info *actual_args = args;
    int client_id = *(actual_args->id);
    if(sem_trywait(&sem_wait_list)==0) { 
        sem_wait(&sem_manage_var);
        clients_waiting[wait_list_index] = client_id;
        if(wait_list_index<wait_list_size){
            wait_list_index++;
        }
        sem_post(&sem_manage_var);

        printf("\n-------------------------------------------------------\n");
        printf("[Cliente %i] esta realizando uma solicitacao de conexao!\n", client_id);
        show_wait_list();
        sem_wait(&sem_connect);
        int use_time = (rand() % 3)+2;
        if(wait_list_index!=0){
            wait_list_index--;
        }
        for(int i=0; i<wait_list_size-1; i++){
            clients_waiting[i] = clients_waiting[i+1];
            clients_waiting[i+1] = 0;
        }
        printf("\n-------------------------------------------------------\n");
        printf("[Cliente %i] estabeleceu conexao com a rede!\n", client_id);
        show_wait_list();
        sem_post(&sem_wait_list);

        sleep(use_time);
        sem_post(&sem_connect);
    }else{
        printf("[Cliente %i] nao conseguiu entrar na fila pois ela estava cheia!\n", client_id);
    }

    free(actual_args);
}

int main(int argc, char **argv) {
    int num_wait, client_count=0, creation_time;
    pthread_t client;
    if (argc == 2 && strtol(argv[1], NULL, 10) != 0) {
        wait_list_size = abs(strtol(argv[1], NULL, 10));
    } else if(argc > 1 && strtol(argv[1], NULL, 10)==0) {
        printf("O numero de 'esperas' disponiveis deve ser maior que 0.\n");
        exit(0);
    }
    else {
        printf("Eh necessario passar o numero de 'esperas' disponiveis no sistema por parametro na execucao do codigo\n");
        printf("Ex.: ./<file-name> 5\n");
        exit(0);
    }
    printf("\n-------------------------------------------------------\n");
    printf("\n               Rede Wi-Fi do shopping XYZ              \n");
    printf("\n- Maximo de solicitacoes em espera: %i\n", wait_list_size);
    printf("\n-------------------------------------------------------\n");
    clients_waiting = (int*)malloc(wait_list_size * sizeof(int));
    sem_init(&sem_wait_list, 0, wait_list_size);
    sem_init(&sem_connect, 0, 1);
    sem_init(&sem_manage_var, 0, 1);
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
