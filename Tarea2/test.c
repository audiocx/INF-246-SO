#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "domino.h"
#include "comm.h"

int main()
{
    int interruptor;
    int pid = -1;

    crearPipes();
    crearHijos(&pid);

    int *turnos = crearMemComp(sizeof(int));
    *turnos = 1;

    while ((*turnos) < 4)
    {
        if (pid == -1)
        {
            interruptor = 1;
            write(pipesPX[(*turnos)][WRITE], &interruptor, 1);

            while (interruptor)
                read(pipesXP[(*turnos)][READ], &interruptor, 1);

            printf("interruptor: %i\n", interruptor);

            (*turnos)++;
        }
        else
        {
            interruptor = 0;
            while (!interruptor)
                read(pipesPX[pid][READ], &interruptor, 1);

            printf("me estoy ejecutandoooo %i\n", pid);

            interruptor = 0;
            write(pipesXP[pid][(*turnos)], &interruptor, 1);
        }
    }

    return 0;
}