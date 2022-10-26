#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_PROCESS 20
#define QUANTUM 5
#define BUFFER_SIZE 255

const char SEPARATOR[2] = ";";

typedef int FLAG_TYPE;
#define NOT_CONSUMED 0 
#define CONSUMED 1 

typedef struct{
    char ID[5];
    int DURATION;
    FLAG_TYPE FLAG;
} Process;

Process processList[MAX_PROCESS];

void reorganizeList(Process *processList, int finalP, int size){
    Process aux = processList[finalP];
    for (int i = finalP; i < size - 1; i++){
        processList[i] = processList[i + 1];
    }
    processList[size - 1] = aux;
}

int main(int argc, char const *argv[])
{
    FILE *file;

    Process p;
    char buffer[BUFFER_SIZE];
    char *value;
    int count = 0;

    file = fopen("./processos.txt", "r");

    while (fgets(buffer, BUFFER_SIZE, file)){
        value = strtok(buffer, SEPARATOR);
        strcpy(p.ID, value);
        value = strtok(NULL, SEPARATOR);
        p.DURATION = atoi(value);
        p.FLAG = NOT_CONSUMED;
        processList[count] = p;
        count++;
    }

    fclose(file);

    int notProcessed = count;
    int currentProcIndex = 0;

    Process *currentProc;

    while (notProcessed > 0){
        currentProc = &processList[currentProcIndex];
        printf("PROCESSO ATUAL: %s, TEMPO DE EXECUÇÂO: %i SEGUNDOS.\n", currentProc->ID,currentProc->DURATION);
        if(currentProc->DURATION < QUANTUM){
            sleep(currentProc->DURATION);
        }
        else{
            sleep(QUANTUM);
        }
        if (currentProc->FLAG == NOT_CONSUMED){
            if (currentProc->DURATION <= QUANTUM){
                printf("PROCESSO %s FINALIZADO.\n\n", currentProc->ID);
                notProcessed--;
                currentProcIndex = (currentProcIndex + 1) % count;
                currentProc->FLAG = CONSUMED;
                currentProc->DURATION = 0;
            }
            else{
                printf("PROCESSO %s NÃO FOI COMPLETADO, CHAVEANDO NOVO PROCESSO...\n", currentProc->ID);
                currentProc->DURATION -= QUANTUM;
                reorganizeList(processList, currentProcIndex, count);
            }
        }
        else{
            currentProcIndex = (currentProcIndex + 1) % count;
        }
    }
    return 0;
}