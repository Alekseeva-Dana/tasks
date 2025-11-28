#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define N 5 
#define LEFT (i-1+N)%N 
#define RIGHT (i+1)%N

#define THINKING 0
#define HUNGRY 1
#define EATING 2

int state[N] = {0};

sem_t mutex;
sem_t s[N];

void Think(int i) {
    printf("Философ %d размышляет...\n", i);
    sleep(rand() % 3 + 1);
}

void Eat(int i) {
    printf("Философ %d ест спагетти\n", i);
    sleep(rand() % 2 + 1);
}

void Test(int i) {
    if(state[i] == HUNGRY &&
       state[LEFT] != EATING &&
       state[RIGHT] != EATING) {
        state[i] = EATING;
        sem_post(&s[i]);
    }
}

void TakeForks(int i) {
    sem_wait(&mutex);
    state[i] = HUNGRY;
    printf("Философ %d голоден\n", i);
    Test(i);
    sem_post(&mutex);
    sem_wait(&s[i]);
}

void PutForks(int i) {
    sem_wait(&mutex);
    state[i] = THINKING;
    printf("Философ %d закончил есть и положил вилки\n", i);
    Test(LEFT);
    Test(RIGHT);
    sem_post(&mutex);
}

void* Philosopher(void* arg) {
    int i = *(int*)arg;
    
    while(1) {
        Think(i);
        TakeForks(i);
        Eat(i);
        PutForks(i);
    }
    return NULL;
}

int main() {
    pthread_t philosophers[N];
    int ids[N];
    
    srand(time(NULL));
   
    sem_init(&mutex, 0, 1);
    for(int i = 0; i < N; i++) {
        sem_init(&s[i], 0, 0);
    }
    
    for(int i = 0; i < N; i++) {
        ids[i] = i;
        pthread_create(&philosophers[i], NULL, Philosopher, &ids[i]);
    }
    
    for(int i = 0; i < N; i++) {
        pthread_join(philosophers[i], NULL);
    }
    
    sem_destroy(&mutex);
    for(int i = 0; i < N; i++) {
        sem_destroy(&s[i]);
    }
    
    return 0;
}
