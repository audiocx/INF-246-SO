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
    int interruptor;

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

    int *ganador = crearMemComp(sizeof(int)); // creamos una variable global del ganador

    srand(time(NULL));
    int turno = rand() % 4; // jugador = 0; bot1 = 1; bot2 = 2, bot3 = 3

    if (turno == 0)
        printf("Eres el jugador %i, tu empiezas el juego!\n", turno);
    else
        printf("BOT %i empieza el juego!\n", turno);

    int pid = -1; // padre = -1, hijos = 0, 1, 2, 3
    crearPipes();
    crearHijos(&pid);
    controlPipes(pid);

    while (!(*ganador = hayGanador(*globalH[0], *globalH[1], *globalH[2], *globalH[3], *saltos))) // verificamos ganador
    {
        if (pid == -1) // entramos en el proceso padre
        {
            printf("===========================================================================\n");

            interruptor = 1;
            write(pipesPX[turno][WRITE], &interruptor, 1); // mando a que el hijo ejecute su turno

            while (interruptor) // espero a que termine su turno
                read(pipesXP[turno][READ], &interruptor, 1);

            turno = (turno + 1) % 4; // calculo el turno siguiente

            sleep(WAIT_TIME); // espero, mas que nada para que no sea incomodo en la terminal
        }
        else // entramos en los procesos hijo
        {
            interruptor = 0;

            while (!interruptor && !(*ganador)) // espero que me manden para ejecutar mi turno
                read(pipesPX[pid][READ], &interruptor, 1);

            if (*ganador) // esto termina efectivamente el proceso si ya tenemos ganador
                break;

            printf("Turno: %i | Saltos: %i\n", pid, *saltos);
            printMesa(*globalTable);

            if (pid == 0)
            {
                printf("Es tu turno!\n");
                printMano(*globalH[pid]);
                int opcion, swap, validez, lado;

                printf("* Selecciona uno de los dominos de tu mano: ");
                scanf("%i", &opcion);
                if (opcion == 0)
                {
                    printf("Has saltado tu turno!\n");
                    (*saltos)++;
                }
                else if (opcion <= globalH[pid]->size)
                {
                    printf("* ¿Girar este domino? si: <1>, no: <0>: ");
                    scanf("%i", &swap);
                    if (swap == 1)
                    {
                        printf("Domino girado!\n");
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
                        (*saltos)++;
                    }
                    else
                    {
                        printf("Domino agregado!\n");
                        (*saltos) = 0;
                        sacarDeMano(globalH[pid], opcion - 1);
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
                    printf("BOT %i ha agregado una pieza!\n", pid);
                }
                else
                {
                    (*saltos)++;
                    printf("BOT %i ha saltado su turno\n", pid);
                }
            }

            interruptor = 0;
            write(pipesXP[pid][WRITE], &interruptor, 1); // notifico que ya termine mi turno
        }
    }

    if (pid == -1) // presentamos los resultados
    {
        printf("===========================================================================\n");
        printf("El juego ha terminado, jugador %i ha ganado!\n", *ganador - 1);
        for (int i = 0; i < N_JUG; i++)
        {
            printf("* Mano final jugador %i: ", i);
            printManoSimple(*globalH[i]);
            printf("\n");
            free(globalH[0]); // liberamos la memoria de las manos
        }

        free(globalTable); // liberamos la memoria de la mesa
        free(saltos);
        free(ganador);
    }

    cerrarPipes(pid);
    return 0;
}