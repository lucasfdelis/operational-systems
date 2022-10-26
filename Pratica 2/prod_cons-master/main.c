#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define MAX 1000000000
pthread_mutex_t the_mutex;
pthread_cond_t condc, condp;
int buffer[5] = {};

const clock_t MAXDELAY = 1000000; // original = 2000000

void imprimirArray(){

    int tamanho = sizeof(buffer)/sizeof(buffer[0]);

    printf("\nbuffer = [");
    for (int i = 0; i < tamanho; i++){
        printf("%d", buffer[i]);
    }
    printf("]\n");
}
void delay(clock_t ticks) { /* a "busy" delay */
  clock_t start = clock();
  do
    ; while (clock() < start + ticks);
}

void *producer(void *ptr) {
    int i;

    /* usado para sinalizacao */
    /* buffer usado entre produtor e consumidor */
    /* dados do produtor */
    for (i = 1; i <= MAX; i++) {

        if (i > 5) i = 1;

        pthread_mutex_lock(&the_mutex); /* obtem acesso exclusivo ao buffer */
        while (buffer[i-1] != 0)
            // Pode acabar a fatia de tempo?
            pthread_cond_wait(&condp, &the_mutex);
        printf("\nColocando item %d no buffer", i);
        // [i-1] para começar da posição 0, já que i = 1 
        buffer[i-1] = i; /*coloca item no buffer */     
        imprimirArray();
        delay(MAXDELAY);

        pthread_cond_signal(&condc); /* acorda consumidor */
        pthread_mutex_unlock(&the_mutex);/* libera acesso ao buffer */
    }

    pthread_exit(0);
}

void *consumer(void *ptr) {
    int i;

    /* quantos numeros produzir */
    /* dados do consumidor */
    for (i = 1; i <= MAX; i++) {

        if (i > 5) i = 1;

        pthread_mutex_lock(&the_mutex); /* obtem acesso exclusivo ao buffer */
        // [i-1] para começar da posição 0, já que i = 1 
        while (buffer[i-1] == 0) pthread_cond_wait(&condc, &the_mutex);
        printf("\nRetirando item %d no buffer", i);
        buffer[i-1] = 0; /* retira o item do buffer */

        imprimirArray();

        delay(MAXDELAY);
        /* acorda o produtor */
        pthread_cond_signal(&condp);
        pthread_mutex_unlock(&the_mutex);/* libera acesso ao buffer */
    }
    pthread_exit(0);

}

int main(int argc, char **argv)
{
    imprimirArray();
    pthread_t pro, con;
    pthread_mutex_init(&the_mutex, 0);
    pthread_cond_init(&condc, 0);
    pthread_cond_init(&condp, 0);
    pthread_create(&con, 0, consumer, 0);
    pthread_create(&pro, 0, producer, 0);
    pthread_join(pro, 0);
    pthread_join(con, 0);
    pthread_cond_destroy(&condc);
    pthread_cond_destroy(&condp);
    pthread_mutex_destroy(&the_mutex);
}