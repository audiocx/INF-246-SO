#include "comm.h"

int pipe0P[2];
int pipe1P[2];
int pipe2P[2];
int pipe3P[2];

int pipeP0[2];
int pipeP1[2];
int pipeP2[2];
int pipeP3[2];

Pipe *pipesXP[] = {pipe0P, pipe1P, pipe2P, pipe3P}; // pipes dirigidos de jugador X a Padre
Pipe *pipesPX[] = {pipeP0, pipeP1, pipeP2, pipeP3}; // pipes dirigidos de Padre a jugador X

/*
Nombre: crearMemComp
Descripcion: crea un bloque de memoria compartida con los demas procesos
Inputs:
    - size_t size: tamano del bloque de memoria
Output: puntero void al bloque de memoria
 */
void *crearMemComp(size_t size)
{
    int protection = PROT_READ | PROT_WRITE;
    int visibilty = MAP_SHARED | MAP_ANONYMOUS;
    return mmap(NULL, size, protection, visibilty, -1, 0);
}

/*
Nombre: crearPipes
Descripcion: crea los pipes necesarios para la comunicacion entre procesos
Inputs:
    -nada
Output: nada
 */
void crearPipes()
{
    for (int i = 0; i < 4; i++)
    {
        pipe(pipesXP[i]);
        pipe(pipesPX[i]);
    }
}

/*
Nombre: controlPipes
Descripcion: cierra los pipes que no son usados
Inputs:
    - int id_jugador: dependiendo del proceso, se cierran los pipes relacionados a este
Output: nada
 */
void controlPipes(int id_jugador)
{
    if (id_jugador == -1)
    {
        for (int i = 0; i < 4; i++)
        {
            close(pipesXP[i][WRITE]);
            close(pipesPX[i][READ]);
        }
    }
    else if (id_jugador == 0)
    {
        close(pipe0P[READ]);
        close(pipeP0[WRITE]);
    }
    else if (id_jugador == 1)
    {
        close(pipe1P[READ]);
        close(pipeP1[WRITE]);
    }
    else if (id_jugador == 2)
    {
        close(pipe2P[READ]);
        close(pipeP2[WRITE]);
    }
    else if (id_jugador == 3)
    {
        close(pipe3P[READ]);
        close(pipeP3[WRITE]);
    }
}

/*
Nombre: cerrarPipes
Descripcion: cierra las pipes restantes
Inputs:
    - int id_jugador: dependiendo del proceso, se cierran los pipes relacionados a este
Output: nada
 */
void cerrarPipes(int id_jugador)
{
    if (id_jugador == -1)
    {
        for (int i = 0; i < 4; i++)
        {
            close(pipesXP[i][READ]);
            close(pipesPX[i][WRITE]);
        }
    }
    else if (id_jugador == 0)
    {
        close(pipe0P[WRITE]);
        close(pipeP0[READ]);
    }
    else if (id_jugador == 1)
    {
        close(pipe1P[WRITE]);
        close(pipeP1[READ]);
    }
    else if (id_jugador == 2)
    {
        close(pipe2P[WRITE]);
        close(pipeP2[READ]);
    }
    else if (id_jugador == 3)
    {
        close(pipe3P[WRITE]);
        close(pipeP3[READ]);
    }
}

/*
Nombre: crearHijos
Descripcion: crea los procesos hijos con fork, cambiando el id que entra por referencia
Inputs:
    - int *id_jugador: dependiendo del proceso, cambia esta id por referencia
Output: nada
 */
void crearHijos(int *id_jugador)
{
    for (int i = 0; i < 4; i++)
    {
        if (fork() == 0)
        {
            *id_jugador = i;
            break;
        }
    }
}