#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "domino.h"
#include "comm.h"

#define N_JUG 4     // numero de jugadores
#define WAIT_TIME 1 // tiempo en segundos entre turnos de los bots

mesa table;
mano h[4];

int main()
{
    mensaje msg;

    initMesa(&table);                      // iniciamos la mesa
    initManos(&h[0], &h[1], &h[2], &h[3]); // iniciamos las manos

    mesa *globalTable = crearMemComp(sizeof(table)); // creamos la memoria compartida de la mesa
    memcpy(globalTable, &table, sizeof(table));

    mano *globalH[4];
    for (int i = 0; i < N_JUG; i++) // creamos las memorias compartidas de las manos
    {
        globalH[i] = crearMemComp(sizeof(h[i]));
        memcpy(globalH[i], &h[i], sizeof(h[i]));
    }

    int *saltos = crearMemComp(sizeof(int)); // creamos una variable global de la cantidad de saltos
    *saltos = 0;                             // iniciada inicialmente en 0

    int *lado_ult_mov = crearMemComp(sizeof(int));
    *lado_ult_mov = 0;

    srand(time(NULL));
    int turno = rand() % 4; // jugador = 0; bot1 = 1; bot2 = 2, bot3 = 3

    if (turno == 0)
        printf("¡Eres el jugador %i, tu empiezas el juego!\n", turno);
    else
        printf("¡BOT %i empieza el juego!\n", turno);

    int pid = -1; // padre = -1, hijos = 0, 1, 2, 3
    crearPipes();
    crearHijos(&pid);
    controlPipes(pid);

    int flag = 4;
    while (flag) // verificamos ganador
    {
        if (pid == -1) // entramos en el proceso padre
        {
            msg.ganador = hayGanador(*globalH[0], *globalH[1], *globalH[2], *globalH[3], *saltos);
            msg.interruptor = 1;

            write(pipesPX[turno][WRITE], &msg, sizeof(msg)); // mando a que el hijo ejecute su turno

            if (msg.ganador)
            {
                printf("===========================================================================\n");
                printf("* Mano final jugador %i: ", turno);
                printManoSimple(*globalH[turno]);
                printf("\n");
                flag--;
            }

            while (msg.interruptor) // espero a que termine su turno
                read(pipesXP[turno][READ], &msg.interruptor, 1);

            turno = (turno + 1) % 4; // calculo el turno siguiente

            sleep(WAIT_TIME); // espero, mas que nada para que no sea incomodo en la terminal
        }
        else // entramos en los procesos hijo
        {
            msg.ganador = 0;
            msg.interruptor = 0;

            while (!msg.interruptor && !msg.ganador) // espero que me manden para ejecutar mi turno
                read(pipesPX[pid][READ], &msg, sizeof(msg));

            if (msg.ganador)
            {
                msg.interruptor = 0;
                write(pipesXP[pid][WRITE], &msg.interruptor, 1);
                break;
            }

            printf("===========================================================================\n");
            printf("Turno: %i | Dominos en mano: %i | Saltos: %i\n", pid, globalH[pid]->size, *saltos);
            printMesa(*globalTable, *lado_ult_mov);
            if (pid == 0)
            {
                printf("Es tu turno, tienes estos dominos:\n");
                printMano(*globalH[pid]);
                int opcion, swap, validez, lado;

                printf("* Selecciona uno de los dominos de tu mano: ");
                scanf("%i", &opcion);
                if (opcion == 0)
                {
                    printf("¡Has saltado tu turno!\n");
                    (*saltos)++;
                }
                else if (opcion <= globalH[pid]->size)
                {
                    printf("* ¿Girar este domino? si: <1>, no: <0>: ");
                    scanf("%i", &swap);
                    if (swap == 1)
                    {
                        printf("¡Domino girado!\n");
                        swapDmn(&globalH[pid]->h[opcion - 1]);
                    }

                    printf("* Agregar este domino a la izquierda: <0>, derecha: <1>: ");
                    scanf("%i", &lado);
                    if (lado != 0 && lado != 1)
                    {
                        printf("Opcion invalida, cerrando programa...\n");
                        exit(1);
                    }

                    char LR;
                    if (lado == 0)
                        LR = 'L';
                    if (lado == 1)
                        LR = 'R';

                    validez = insertarDmn(globalTable, globalH[pid]->h[opcion - 1], LR);

                    if (!validez)
                    {
                        printf("Entrada no valida, saltando turno...\n");
                        (*lado_ult_mov) = 0;
                        (*saltos)++;
                    }
                    else
                    {
                        printf("¡Domino agregado!\n");
                        sacarDeMano(globalH[pid], opcion - 1);
                        (*lado_ult_mov) = lado + 1;
                        (*saltos) = 0;
                    }
                }
                else
                {
                    printf("\nOpcion invalida, cerrando programa...\n");
                    exit(1);
                }
            }
            else // NPCs
            {
                int dummy = movDummy(globalTable, globalH[pid]);

                if (dummy) // si se pudo realizar el movimiento
                {
                    (*saltos) = 0;
                    printf("¡BOT %i ha agregado una pieza!\n", pid);
                }
                else
                {
                    (*saltos)++;
                    printf("¡BOT %i ha saltado su turno\n", pid);
                }

                (*lado_ult_mov) = dummy;
            }

            msg.interruptor = 0;
            write(pipesXP[pid][WRITE], &msg.interruptor, 1); // notifico que ya termine mi turno
        }
    }

    if (pid == -1)
    {
        printf("===========================================================================\n");
        printf("* ¡El juego ha terminado, jugador %i ha ganado!\n", msg.ganador - 1);
    }

    cerrarPipes(pid);

    printf("* Cerrando proceso %i\n", pid);

    return 0;
}