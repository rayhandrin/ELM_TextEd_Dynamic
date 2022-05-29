#ifndef nrdll_hpp
#define nrdll_hpp

/* modules   = 18                       */
/* functions = 8                        */
/* Total     = modules + functions = 26 */

#define Nil NULL
#define info(P) (P)->info
#define line(P) (P)->line
#define next(P) (P)->next
#define prev(P) (P)->prev
#define first(L) (L).first
#define last(L) (L).last

typedef char infotype;
typedef int index;
typedef struct NODE *addr;
typedef struct NODE
{
    infotype info;
    index line;
    addr prev;
    addr next;
} node;

typedef struct
{
    addr first;
    addr last;
} list;

/* Manajemen Memori */
addr alct(infotype X); /* Allocate */
void dlct(addr *P);    /* Dealocate */

/* Create List */
void crlt(list *L); /* Create List */

/* Validasi */
bool ckliempty(list &L); /* Check List Empty */

/* Insert Value */
void insfi(list *L, infotype X);
void insaf(list *L, infotype X, addr *Paft);
void insaf_or(list *L, infotype X, addr *Paft);
void insla(list *L, infotype X);

/* Delete Value */
void delfi(list *L);
void della(list *L);
void delbe(list *L, addr Pbef);
void delaf(list *L, addr Paft);
void delp(list *L, addr Pdel);
void delli(list *L);

/* Proses Insert */
void insfi_pr(list *L, addr P);
void insaf_pr(list *L, addr P, addr Prec);
void insla_pr(list *L, addr P);

/* Proses Delete */
void delfi_pr(list *L, addr *P);
void della_pr(list *L, addr *P);

/* Search */
bool fsearch(list L, addr P);
addr search(list L, infotype X);
addr itr_search(list L, int &col_pos);
addr searchprec(list L, infotype X);
addr search_by_addr(list L, addr X);

/* Print List */
void printli(list &L);

/* Menghitung jumlah node dalam list */
int ttlnode(list &L);

#endif
