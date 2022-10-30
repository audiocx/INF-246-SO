#ifndef __DOMINO_H__
#define __DOMINO_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

typedef struct Domino
{
    int L, R;
} dmn;

typedef struct Mano
{
    dmn h[7];
    int size;
} mano;

typedef struct Mesa
{
    dmn m[28];
    int size;
} mesa;

void initMesa(mesa *t);
int initManos(mano *h0, mano *h1, mano *h2, mano *h3);
void sacarDeMano(mano *h, int d);
void printMano(mano h);
void printManoSimple(mano h);
void printMesa(mesa t);
int insertarDmn(mesa *t, dmn d, char p);
void swapDmn(dmn *d);
int hayGanador(mano h0, mano h1, mano h2, mano h3, int saltos);
int movDummy(mesa *t, mano *h);

#endif