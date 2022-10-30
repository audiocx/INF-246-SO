#ifndef __COMM_H__
#define __COMM_H__

#include <stdlib.h>
#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>

#define READ 0
#define WRITE 1

typedef int Pipe;
typedef int mensaje;

Pipe *pipesXP[4];
Pipe *pipesPX[4];

void *crearMemComp(size_t size);
void crearPipes();
void controlPipes(int id_jugador);
void crearHijos(int *id_jugador);
void cerrarPipes(int id_jugador);

#endif