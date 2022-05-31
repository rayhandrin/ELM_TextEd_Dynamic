#ifndef editor_hpp
#define editor_hpp

/* modules          = 30 */
/* boolean function = 25 */
/* int function     = 4  */

#include "nrdll.hpp"

/* === KEY === */
#define BACKSPACE_KEY 8
#define DELETE_KEY 83
#define TAB_KEY 9
#define RETURN_KEY 13
#define CSR_UP_KEY 72
#define CSR_DOWN_KEY 80
#define CSR_RIGHT_KEY 77
#define CSR_LEFT_KEY 75
#define HOME_KEY 71
#define END_KEY 79
#define PG_UP_KEY 73
#define PG_DW_KEY 81
#define F1_KEY 59
#define CTRL_O_KEY 15
#define CTRL_F_KEY 6
#define CTRL_R_KEY 18
#define CTRL_S_KEY 19
#define CTRL_X_KEY 24
#define CTRL_C_KEY 3
#define CTRL_V_KEY 22
#define CTRL_N_KEY 14
#define CTRL_Q_KEY 17
#define CONTROL_1_KEY 0
#define CONTROL_2_KEY -32
#define CONTROL_3_KEY 224

/* === Variabel Global === */
typedef struct pos_var
{
    short X = 0;
    short Y = 0;
} POS;

/* ===== PENGUBAH NODE YANG DITUNJUK OLEH POINTER ===== */
void to_bgline(addr &Pcurr);                       /* Menemukan node yang ditunjuk oleh pointer ke node awal sebuah baris */
void to_enline(list &L, addr &Pcurr);              /* Memindahkan node yang ditunjuk oleh pointer ke akhir sebuah baris */
addr to_preline(list &L, addr &Pcurr);             /* Mengembalikan alamat memori node baris sebelumnya */
addr to_nexline(list &L, addr &Pcurr);             /* Mengembalikan alamat memori node baris setelahnya */
addr to_line(list &L, addr &Pcurr, int goto_line); /* Mengembalikan alamat memori node pertama dari baris yang ditunjuk */

/* ===== PENGHITUNG JUMLAH KARAKTER ===== AKA Last_Column_Memory*/
int ttlnode_l(list &L, addr Pstart); /* Menghitung jumlah karakter pada baris saat ini */

/* ===== PEMERIKSA ALAMAT MEMORI ===== */
bool is_bglist(list &L, addr &P);   /* Memeriksa apakah pointer sekarang berada pada awal dari list */
bool is_enlist(list &L, addr &P);   /* Memeriksa apakah pointer sekarang berada pada akhri dari list */
bool is_frstlist(list &L, addr &P); // NEW	/* Memeriksa apakah pointer sekarang berada pada List yang sama dengan First */
bool is_lstlist(list &L, addr &P);  // NEW	/* Memeriksa apakah pointer sekarang berada pada List yang sama dengan Last */
bool is_bgline(list &L, addr &P);   /* Memeriksa apakah pointer sekarang berada pada awal dari baris */
bool is_enline(list &L, addr &P);   /* Memeriksa apakah pointer sekarang berada pada akhir dari baris */

/* ===== PEMERIKSA JUMLAH BARIS ===== */
int ttl_line(list &L); /* Menghitung jumlah baris yang ada pada linked list */

/* ===== REFRESH SEMUA BARIS YANG TERSEDIA ===== */
void rfrsh_line(list *); /* Merubah semua line ketika ada penanda line (\n) yang dihapus */

/* ===== CURSOR ===== */
void set_csr_pos(int line, int column);           /* Memindahkan koordintat kursor pada konsol */
void csr_up(list &L, addr *cursor);               /* Membuat pointer di dalam list untuk cursor pindah ke atas */
void csr_up_only_line(addr *);                    /* Membuat pointer untuk pindah ke atas namun hanya untuk linenya saja */
int cnt_col_csr_up(list &, addr &);               /* Menghitung jumlah node yang terdapat di line yang ditunjuk dari pointer */
bool is_linecurr_bgr_thn_lineprv(list &, addr &); // NEW /* Cek apakah line yang saat ini dipijaki lebih besar daripada line (column memory) yang sebelumnya*/
bool is_linecurr_sml_thn_lineprv(list &, addr &); // NEW /* Cek apakah line yang saat ini dipijaki lebih kecil daripada line (column memory) yang sebelumnya*/
void csr_down(list &, addr *);                    /* Membuat pointer di dalam list untuk cursor pindah ke bawah */
void csr_down_spcl(list &, addr *);               /* Membuat pointer untuk berpindah langsung keujung line (last_column_memory) dibawahnya */
bool is_linecurr_bgr_thn_linenxt(list &, addr &); // NEW /* Cek apakah line yang saat ini dipijaki lebih besar daripada line (column memory) yang setelahnya*/
bool is_linecurr_sml_thn_linenxt(list &, addr &); // NEW /* Cek apakah line yang saat ini dipijaki lebih kecil daripada line (column memory) yang setelahnya*/
int cnt_col_csr_dwn(list &, addr &);              /* Menghitung jumlah node yang terdapat di line yang ditunjuk dari pointer */
void csr_left(list &L, addr *cursor);             /* Membuat pointer di dalam list untuk cursor pindah ke kiri */
void csr_right(list &L, addr *cursor);            /* Membuat pointer di dalam list untuk cursor pindah ke akhir linked list */
void csr_fst_ll(list &, addr &);                  /* Membuat pointer di dalam list untuk cursor pindah ke awal linked list */
void csr_lst_ll(list &, addr &);                  /* Membuat pointer di dalam list untuk cursor pindah ke Kanan */

/* === PENCETAKAN ISI SUBVARIABEL INFO PADA NODE === */
void printfr(addr P);             /* Mencetak isi subvar info sebuah node hingga akhir list mulai dari tempat yang bisa disesuaikan */
void printfr_ln(list &L, addr P); /* Mencetak isi subvar info sebuah node hingga akhir baris saja mulai dari tempat yang bisa disesuaikan */
void print_ln(list &L, addr P);   /* Mencetak isi subvar info sebuah node dari satu baris */

/* === SETTER DAN GETTER === */
index get_current_lnpos(addr &P);                /* Mendapatkan value dari subvariable line node */
index get_current_colpos(list &L, addr &cursor); /* Menunjuk kolom yang saat ini akan menginput */

/* === INPUT STANDAR === */
bool is_std_in(infotype &buffer);                /* Memeriksa masukan backspace keyboard */
void std_in(list &L, addr &P, infotype &buffer); /* Menerima masukan standar (alfabet + numerik + simbol standar) */

/* === INPUT NONSTANDAR === */
bool is_backspace_in(infotype &buffer);                /* Memeriksa masukan backspace keyboard */
bool is_delete_in(infotype &buffer);                   /* Meneriksa masukan delete dari keyboard */
bool is_return_in(infotype &buffer);                   /* Meneriksa masukan return (enter) dari keyboard */
bool is_tab_in(infotype &buffer);                      /* Meneriksa masukan tab dari keyboard */
bool is_ctrl_in(infotype &buffer);                     /* Meneriksa masukan Control Character dari keyboard */
bool is_save_in(infotype &buffer);                     /* Meneriksa masukan CTRL + S dari keyboard */
bool is_saveas_in(infotype &buffer);                   /* Meneriksa masukan CTRL + X dari keyboard */
bool is_open_in(infotype &buffer);                     /* Meneriksa masukan CTRL + O dari keyboard */
bool is_new_in(infotype &buffer);                      /* Meneriksa masukan CTRL + N dari keyboard */
bool is_quit_in(infotype &buffer);                     /* Meneriksa masukan CTRL + Q dari keyboard */
bool is_copy_in(infotype &buffer);                     /* Meneriksa masukan CTRL + C dari keyboard */
bool is_paste_in(infotype &buffer);                    /* Meneriksa masukan CTRL + V dari keyboard */
bool is_find_in(infotype &buffer);                     /* Meneriksa masukan CTRL + F dari keyboard */
bool is_replace_in(infotype &buffer);                  /* Meneriksa masukan CTRL + R dari keyboard */
bool is_menu_in(infotype &buffer);                     /* Meneriksa masukan F1 dari keyboard */
void backspace_in(list &L, addr &P, infotype &buffer); /* Memerima masukan backspace keyboard */
void delete_in(list &L, addr &P, infotype &buffer);    /* Menerima masukan delete dari keyboard */
void return_in(list &L, addr &P, infotype &buffer);    /* Menerima masukan return (enter) dari keyboard */
void tab_in(list &L, addr &P, infotype &buffer);       /* Menerima masukan tab dari keyboard */

/* === DISPLAY === */
void highlight(int Xpos, int Ypos, int nLen);                                 /* Memberi atribut reversed pada character/s terpilih */
void clear();                                                                 /* Mengisi cell console dengan ' ', tetapi baris pertama dibiarkan */
void set_consoleprop(int w_window, int h_window, int w_buffer, int h_buffer); /* Mengatur buffer size, window size, dan title console */
void boxes(int Xpos, int Ypos, char str[]);                                   /* Membersihkan output pada konsol, tetapai baris satu dibiarkan tetap ada */

/* === COPY DAN PASTE === */
void paste(list &L_source, list &main_text, addr last_cursor); /* Melakukan print pada text editor pada list copy yang sudah ditampung */
void copy(list &L_copy, list &L_source);

/* === FIND DAN REPLACE === */
void find(list &Text);
void replace(char c[], int counter, list &Text);

/* === OPEN === */
void open(list *L);

/* === SAVE DAN SAVE AS === */
void save(list &);
void save_as(list &L, char *destination_filename);

/* Help */
void help();

/* === NEW DAN QUIT === */
void newfl(list &L, addr &P);   /* Membuat sesi baru dengan menghapus isi list utama */
void clste(list &L, list &Lcp); /* Keluar dari program */

/* === DISPLAY LINE & COLUMN DAN TOTAL CHARACTER === */
void list_stat(list &L, addr P, int col); /* Memperlihatkan posisi baris dan kolom, total karakter */

/* === MENUBAR === */
void menu_bar(list &L, list &Lc, addr P, char replace_text[], char name_file[]);
void menu_edit(list &L, list &Lc, addr P, char replace_text[]);
void menu_file(list &L, list &Lc, addr P, char name_file[]);
void arrow(int realPosition, int arrowPosition);

#endif
