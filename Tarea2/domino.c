#include "domino.h"

#define KYEL "\x1B[33m"
#define KPURPLE "\x1b[35m"
#define KCYAN "\x1b[36m"
#define KRESET "\x1b[0m"

/*
Nombre: initMesa
Descripcion: inicializa la mesa en 0
Inputs:
    - mesa *t: mesa a inicializar, por referencia
Output:
 */
void initMesa(mesa *t)
{
    t->size = 0;
}

/*
Nombre: initManos
Descripcion: inicializa las manos repartidas aleatoriamente
Inputs:
    - mano *hi: mano a inicializar
Output: jugador con la ficha [6|6]
 */
int initManos(mano *h0, mano *h1, mano *h2, mano *h3)
{
    h0->size = 0;
    h1->size = 0;
    h2->size = 0;
    h3->size = 0;

    srand(time(NULL));
    int r, j6_6 = -1;

    for (int i = 6; i >= 0; i--)
    {
        for (int j = i; j >= 0; j--)
        {
            r = rand() % 4;

            if (i == 6 && j == 6)
                j6_6 = r;

            while (h0->size < 7 || h1->size < 7 || h2->size < 7 || h3->size < 7)
            {
                if (r == 0 && h0->size < 7)
                {
                    h0->h[h0->size].L = i;
                    h0->h[h0->size].R = j;
                    h0->size++;
                    break;
                }
                if (r == 1 && h1->size < 7)
                {
                    h1->h[h1->size].L = i;
                    h1->h[h1->size].R = j;
                    h1->size++;
                    break;
                }
                if (r == 2 && h2->size < 7)
                {
                    h2->h[h2->size].L = i;
                    h2->h[h2->size].R = j;
                    h2->size++;
                    break;
                }
                if (r == 3 && h3->size < 7)
                {
                    h3->h[h3->size].L = i;
                    h3->h[h3->size].R = j;
                    h3->size++;
                    break;
                }
                r = (r + 1) % 4;
            }
        }
    }
    return j6_6; // jugador con la ficha [6|6]
}

/*
Nombre: sacarDeMano
Descripcion: saca el un domino de la mano del jugador
Inputs:
    - mano *h: mano de donde se sacara el domino
    - int d: domio que se sacara
Output: nada
 */
void sacarDeMano(mano *h, int d)
{
    if (d > h->size)
    {
        printf("\nDomino fuera de rango\n");
        return;
    }

    if (d < h->size - 1)
        for (int i = d; i < h->size; i++)
            h->h[i] = h->h[i + 1];
    h->size--;
}

/*
Nombre: printMano
Descripcion: printea la mano de forma elegante
Inputs:
    - mano h: mano a printear
Output: nada
 */
void printMano(mano h)
{
    for (int i = 0; i < h.size; i++)
        printf("%s[%i|%i]", KPURPLE, h.h[i].L, h.h[i].R);
    printf("[saltar]%s", KRESET);
    printf("\n%s", KCYAN);
    for (int i = 0; i < h.size; i++)
        printf(" <%i> ", i + 1);
    printf("  <0>  \n%s", KRESET);
}

/*
Nombre: printManoSimple
Descripcion: printea la mano de forma simple
Inputs:
    - mano h: mano a printear
Output: nada
 */
void printManoSimple(mano h)
{
    for (int i = 0; i < h.size; i++)
        printf("[%i|%i]", h.h[i].L, h.h[i].R);
}

/*
Nombre: insertarDmn
Descripcion: inserta en una mesa el domino especificado, en el lado izquierdo ('L') o derecho ('R')
Inputs:
    - mesa *t: mesa donde se insertara el domino
    - dmn d: domino a insertar
    - char p: lado donde insertar el domino
Output: 1 si se inserto exitosamente, 0 en otro caso
 */
int insertarDmn(mesa *t, dmn d, char p)
{
    if (t->size == 0)
    {
        t->m[0].L = d.L;
        t->m[0].R = d.R;
        t->size++;
        return 1; // insercion exitosa
    }
    if (t->size < 28)
    {
        if (p == 'L' && d.R == t->m[0].L)
        {
            for (int i = t->size; i >= 1; i--)
                t->m[i] = t->m[i - 1];
            t->m[0].L = d.L;
            t->m[0].R = d.R;
        }
        else if (p == 'R' && d.L == t->m[t->size - 1].R)
        {
            t->m[t->size].L = d.L;
            t->m[t->size].R = d.R;
        }
        else
        {
            printf("¡Movimiento invalido!, el domino no fue agregado\n");
            return 0;
        }
        t->size++;
        return 1; // insercion exitosa
    }
    return 0; // insercion fallida
}

/*
Nombre: swapDmn
Descripcion: gira el domino
Inputs:
    - dmn *d: domino a girar
Output: nada
 */
void swapDmn(dmn *d)
{
    int aux = d->L;
    d->L = d->R;
    d->R = aux;
}

/*
Nombre: printMesa
Descripcion: printea la mesa de forma simple
Inputs:
    - mesa t: mesa a printear
Output: nada
 */
void printMesa(mesa t, int lado_ult_mov)
{
    printf("Mesa: ");
    for (int i = 0; i < t.size; i++)
    {
        if (lado_ult_mov == 1 && i == 0)
            printf("%s[%i|%i]%s", KYEL, t.m[i].L, t.m[i].R, KRESET);
        else if (lado_ult_mov == 2 && i == t.size - 1)
            printf("%s[%i|%i]%s", KYEL, t.m[i].L, t.m[i].R, KRESET);
        else
            printf("[%i|%i]", t.m[i].L, t.m[i].R);
    }

    printf("\n");
}

/*
Nombre: hayGanador
Descripcion: determina si se cumplen las condiciones para establecer un ganador
Inputs:
    - mano hi: mano a analizar
    - int saltos: cantidad de saltos al momento de analizar
Output: ganador + 1
 */
int hayGanador(mano h0, mano h1, mano h2, mano h3, int saltos)
{
    if (saltos >= 4)
    {
        int suma[4], min = 1000, res = 0;
        for (int i = 0; i < 4; i++) // inicializamos las sumas
            suma[i] = 0;

        for (int i = 0; i < h0.size; i++) // sumamos los pares de h0
            suma[0] += h0.h[i].L + h0.h[i].R;
        for (int i = 0; i < h1.size; i++) // sumamos los pares de h1
            suma[1] += h1.h[i].L + h1.h[i].R;
        for (int i = 0; i < h2.size; i++) // sumamos los pares de h2
            suma[2] += h2.h[i].L + h2.h[i].R;
        for (int i = 0; i < h3.size; i++) // sumamos los pares de h3
            suma[3] += h3.h[i].L + h3.h[i].R;

        for (int i = 0; i < 4; i++) // elegimos el menor
        {
            if (suma[i] < min)
            {
                min = suma[i];
                res = i;
            }
        }

        return res + 1; // retornamos el ganador
    }

    if (h0.size == 0) // gana el jugador 1
        return 1;
    if (h1.size == 0) // gana el jugador 2
        return 2;
    if (h2.size == 0) // gana el jugador 3
        return 3;
    if (h3.size == 0) // gana el jugador 4
        return 4;

    return 0; // no hay ganador
}

/*
Nombre: movDumy
Descripcion: genera un movimiento posible del bot
Inputs:
    - mesa *t: mesa donde se hara el movimiento
    - mano *h: mano donde se hara el movimiento
Output: el lado donde se hizo la insercion del domino, 0: falla, 1: izquierda, 2: derecha
 */
int movDummy(mesa *t, mano *h)
{
    if (t->size == 0) // si la mesa esta vacia
    {
        int d, max = 0;
        for (int i = 0; i < h->size; i++) // elijo el que tenga la mayor suma de pares
        {
            if (h->h[i].L + h->h[i].R > max)
            {
                max = h->h[i].L + h->h[i].R;
                d = i;
            }
        }
        if (insertarDmn(t, h->h[d], 'L'))
        {
            sacarDeMano(h, d);
            return 1;
        }
        else
            return 0;
    }
    // en otro caso
    int extL = t->m[0].L;           // numero del domino mas a la izquierda de la mesa
    int extR = t->m[t->size - 1].R; // numero del domino mas a la derecha de la mesa

    for (int i = 0; i < h->size; i++)
    {
        if (extL == h->h[i].L) // dmn: [a|c] ext: [a|b]...
        {
            swapDmn(&h->h[i]);
            insertarDmn(t, h->h[i], 'L');
            sacarDeMano(h, i);
            return 1;
        }
        if (extL == h->h[i].R) // dmn: [b|a] ext: [a|c]...
        {
            insertarDmn(t, h->h[i], 'L');
            sacarDeMano(h, i);
            return 1;
        }
        if (extR == h->h[i].L) // ext: ...[a|b] dmn: [b|c]
        {
            insertarDmn(t, h->h[i], 'R');
            sacarDeMano(h, i);
            return 2;
        }
        if (extR == h->h[i].R) // ext: ...[a|b] dmn: [c|b]
        {
            swapDmn(&h->h[i]);
            insertarDmn(t, h->h[i], 'R');
            sacarDeMano(h, i);
            return 2;
        }
    }

    return 0;
}