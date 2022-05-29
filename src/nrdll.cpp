#include <cstdio>
#include <cstdlib>

#include "nrdll.hpp"

bool ckliempty(list &L)
{
    return ((first(L) == Nil) && (last(L) == Nil));
}

void crlt(list *L)
{
    first(*L) = Nil;
    last(*L) = Nil;
}

addr alct(infotype X)
{
    addr P;

    P = (addr)malloc(sizeof(node));
    if (P != Nil)
    {
        info(P) = X;
        line(P) = 1;
        next(P) = Nil;
        prev(P) = Nil;
        return P;
    }
    else
    {
        return Nil;
    }
}

void dlct(addr *P)
{
    free(*P);
}

bool fsearch(list L, addr P)
{
    addr Pt;
    bool Found;

    Pt = first(L);
    Found = false;
    while ((Pt != Nil) && (!Found))
    {
        if (Pt == P)
        {
            Found = true;
        }
        else
        {
            Pt = next(Pt);
        }
    }
    return Found;
}

addr search(list L, infotype X)
{
    addr P;
    bool Found;

    P = first(L);
    Found = false;
    while ((P != Nil) && (!Found))
    {
        if (info(P) == X)
        {
            Found = true;
        }
        else
        {
            P = next(P);
        }
    }
    return P;
}

void insfipr(list *L, addr P)
{
    if (!ckliempty(*L))
    {
        next(P) = first(*L);
        prev(first(*L)) = P;
    }
    else
    {
        last(*L) = P;
    }
    first(*L) = P;
}

void insafpr(list *L, addr P, addr Prec)
{
    next(P) = next(Prec);
    prev(next(Prec)) = P;
    prev(P) = Prec;
    next(Prec) = P;
}

void inslapr(list *L, addr P)
{
    if (!ckliempty(*L))
    {
        prev(P) = last(*L);
        next(last(*L)) = P;
    }
    else
    {
        first(*L) = P;
    }
    last(*L) = P;
}

/* ----- Penghapusan Sebuah Elemen ----- */
void delfipr(list *L, addr *P)
{
    *P = first(*L);
    first(*L) = next(*P);

    if (first(*L) != Nil)
    {
        prev(first(*L)) = Nil;
    }
    else
    {
        last(*L) = Nil;
    }
    next(*P) = Nil;
}

void dellapr(list *L, addr *P)
{
    *P = last(*L);
    last(*L) = prev(*P);

    if (last(*L) != Nil)
    {
        next(last(*L)) = Nil;
    }
    else
    {
        first(*L) = Nil;
    }
    prev(*P) = Nil;
}

void delaf(list *L, addr Paft)
{
    addr P1, P2;

    P1 = next(Paft);
    if ((next(Paft) == last(*L)) || (Paft == last(*L)))
    {
        della(L);
    }
    else
    {
        P1 = next(next(Paft));
        P2 = P1;
        prev(P1) = Paft;

        P1 = next(Paft);
        next(P1) = Nil;
        prev(P1) = Nil;
        dlct(&P1);

        next(Paft) = P2;
    }
}

void delbe(list *L, addr Pbef)
{
    addr P1, P2;

    P1 = next(Pbef);
    if (prev(Pbef) == Nil)
    {
        della(L);
    }
    else if (first(*L) == prev(Pbef))
    {
        delfi(L);
    }
    else
    {
        P1 = prev(prev(Pbef));
        P2 = P1;

        P1 = prev(Pbef);
        next(P1) = Nil;
        prev(P1) = Nil;
        dlct(&P1);

        next(P2) = Pbef;
        prev(Pbef) = P2;
    }
}

void insfi(list *L, infotype X)
{
    addr P;

    P = alct(X);
    if (P != Nil)
    {
        insfipr(L, P);
    }
}

void insaf(list *L, infotype X, addr *Paft)
{
    addr P1, P2;
    if (last(*L) == *Paft)
    {
        insla(L, X);
    }
    else
    {
        P1 = alct(X);
        P2 = next(*Paft);
        next(*Paft) = P1;
        prev(P1) = *Paft;
        next(P1) = P2;
        prev(P2) = P1;
        *Paft = next(*Paft);
    }
}

void insaf_or(list *L, infotype X, addr *Paft)
{
    addr P1, P2;
    if (last(*L) == *Paft)
    {
        insla(L, X);
    }
    else
    {
        P1 = alct(X);
        P2 = next(*Paft);
        next(*Paft) = P1;
        prev(P1) = *Paft;
        next(P1) = P2;
        prev(P2) = P1;
    }
}

void insla(list *L, infotype X)
{
    addr P;

    P = alct(X);
    if (P != Nil)
    {
        inslapr(L, P);
    }
}

void delfi(list *L)
{
    addr P;

    delfipr(L, &P);
    dlct(&P);
}

void della(list *L)
{
    addr P;

    dellapr(L, &P);
    dlct(&P);
}

void printli(list &L)
{
    addr P;

    P = first(L);
    if (P == Nil)
    {
        printf("List kosong\n");
    }
    else
    {
        //        printf("%c(%d)", info(P),line(P));
        printf("%c", info(P));
        while (next(P) != Nil)
        {
            P = next(P);
            //            printf("%c(%d)", info(P),line(P));
            printf("%c", info(P));
        }
    }
}

addr searchprec(list L, infotype X)
{
    addr P;
    bool Found;

    P = first(L);
    Found = false;
    if (P == Nil)
    {
        return Nil;
    }
    else
    {
        while ((next(P) != Nil) && (!Found))
        {
            if (X == info(next(P)))
            {
                Found = true;
            }
            else
            {
                P = next(P);
            }
        }
    }
    if (next(P) == Nil)
    {
        return Nil;
    }
    else
    {
        return P;
    }
}

addr search_by_addr(list L, addr X)
{
    /* Kamus Lokal */
    addr P;
    bool Found;

    /* Algoritma */
    P = first(L);
    Found = false;
    if (P == Nil)
    {
        return Nil;
    }
    else
    {
        while ((next(P) != Nil) && (!Found))
        {
            if (X == P->next)
            {
                Found = true;
            }
            else
            {
                P = next(P);
            }
        }
    }
    if (next(P) == Nil)
    {
        return Nil;
    }
    else
    {
        return P;
    }
}

void delp(list *L, addr Pdel)
{
    addr P;

    if (first(*L) == Pdel)
    {
        delfi(L);
    }
    else if (last(*L) == Pdel)
    {
        della(L);
    }
    else if ((first(*L) != Pdel) && (last(*L) != Pdel))
    {
        P = prev(Pdel);
        next(P) = next(Pdel);
        prev(next(Pdel)) = P;

        next(Pdel) = Nil;
        prev(Pdel) = Nil;
        dlct(&Pdel);
    }
}

int ttlnode(list &L)
{
    addr P;
    int N;

    P = first(L);
    N = 0;
    while (P != Nil)
    {
        N = N + 1;
        P = next(P);
    }
    return N;
}

void delli(list *L)
{
    addr P;

    while (!ckliempty(*L))
    {
        delfi(L);
    }
}
