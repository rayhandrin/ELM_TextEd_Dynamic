#include <cstdio>
#include <cctype>
#include <conio.h>
#include <windows.h>

#include <string>
#include <fstream>
#include <iostream>
using namespace std;

#include "nrdll.hpp"
#include "editor.hpp"

void set_consoleprop(int w_window, int h_window, int w_buffer, int h_buffer)
{
    HANDLE H;
    COORD C;
    SMALL_RECT R;

    // https://stackoverflow.com/questions/12900713/reducing-console-size
    // https://docs.microsoft.com/en-us/windows/console/setconsolescreenbuffersize
    // https://docs.microsoft.com/en-us/windows/console/setconsolewindowinfo
    H = GetStdHandle(STD_OUTPUT_HANDLE);

    C.X = w_buffer;
    C.Y = h_buffer;

    R.Top = 0;
    R.Left = 0;
    R.Bottom = h_window - 1;
    R.Right = w_window - 1;

    SetConsoleScreenBufferSize(H, C);
    SetConsoleWindowInfo(H, TRUE, &R);
}

bool is_bglist(list &L, addr &P)
{
    if (P == first(L) || P == Nil || ckliempty(L) == true)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool is_enlist(list &L, addr &P)
{
    if (P == last(L))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool is_frstlist(list &L, addr &P)
{
    if (line(P) == line(first(L)))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool is_lstlist(list &L, addr &P)
{
    if (line(P) == line(last(L)))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool is_enline(list &L, addr &P)
{
    if (line(P) == 1)
    {
        if (line(P) == ttl_line(L))
        {
            return is_enlist(L, P);
        }
        else if (line(P) < ttl_line(L))
        {
            if (info(P) == '\n')
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }
    else if (line(P) > 1)
    {
        if (line(P) == ttl_line(L))
        {
            return is_enlist(L, P);
        }
        else if (line(P) < ttl_line(L))
        {
            if (info(P) == '\n')
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }
}

bool is_bgline(list &L, addr &P)
{
    if (info(P) == '\n')
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool is_linecurr_bgr_thn_lineprv(list &L, addr &P)
{
    if (cnt_col_csr_up(L, P) >= ttlnode_l(L, to_preline(L, P)))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool is_linecurr_sml_thn_lineprv(list &L, addr &P)
{
    if (cnt_col_csr_up(L, P) < ttlnode_l(L, to_preline(L, P)))
    {
        return true;
    }
    else
    {
        return false;
    }
}

int ttl_line(list &L)
{
    addr P;
    int total;

    P = first(L);
    total = 1;
    while (next(P) != Nil)
    {
        if (info(P) == '\n')
        {
            total = total + 1;
        }
        P = next(P);
    }

    return total;
}

void to_bgline(addr &Pcurr)
{
    if (line(Pcurr) == 1)
    {
        while (prev(Pcurr) != Nil)
        {
            Pcurr = prev(Pcurr);
        }
    }
    else if (line(Pcurr) > 1)
    {
        while (info(prev(Pcurr)) != '\n')
        {
            Pcurr = prev(Pcurr);
        }
    }
}

void to_enline(list &L, addr &Pcurr)
{
    if (line(Pcurr) < ttl_line(L))
    {
        while (info(Pcurr) != '\n')
        {
            Pcurr = next(Pcurr);
        }
    }
    else if (line(Pcurr) == ttl_line(L))
    {
        while (next(Pcurr) != Nil)
        {
            Pcurr = next(Pcurr);
        }
    }
}

addr to_line(list &L, addr &Pcurr, int goto_line)
{
    addr P;

    P = Pcurr;
    if ((goto_line >= 1) && (goto_line <= ttl_line(L)))
    {
        if (goto_line > line(P))
        {
            while (line(P) < goto_line)
            {
                P = next(P);
            }
            to_bgline(P);

            return P;
        }
        else if (goto_line < line(P))
        {
            while (line(P) > goto_line)
            {
                P = prev(P);
            }
            to_bgline(P);

            return P;
        }
        else if (goto_line == line(P))
        {
            to_bgline(P);

            return P;
        }
    }
    else
    {
        return P;
    }
}

int ttlnode_l(list &L, addr Pstart)
{
    addr P;
    int total;

    P = Pstart;
    to_bgline(P);
    total = 1;
    if (line(P) < ttl_line(L))
    {
        to_bgline(P);
        while (info(P) != '\n')
        {
            P = next(P);
            total = total + 1;
        }
    }
    else if (line(P) == ttl_line(L))
    {
        to_bgline(P);
        while (next(P) != Nil)
        {
            P = next(P);
            total = total + 1;
        }
    }

    return total;
}

addr to_preline(list &L, addr &Pcurr)
{
    addr P;

    P = Pcurr;
    if (line(P) <= ttl_line(L))
    {
        while (line(P) > line(Pcurr) - 1)
        {
            P = prev(P);
        }
        to_bgline(P);

        return P;
    }
    else if (line(P) == 1)
    {
        to_bgline(P);

        return P;
    }
}

addr to_nexline(list &L, addr &Pcurr)
{
    addr P;

    P = Pcurr;
    if (line(P) < ttl_line(L))
    {
        while (line(P) < line(Pcurr) + 1)
        {
            P = next(P);
        }
        to_bgline(P);

        return P;
    }
    else if (line(P) == ttl_line(L))
    {
        to_bgline(P);

        return P;
    }
}

void printfr(addr P)
{
    printf("%c", info(P));
    while (next(P) != Nil)
    {
        P = next(P);
        printf("%c", info(P));
    }
}

void printfr_ln(list &L, addr P)
{
    if (line(P) >= 1)
    {
        if (line(P) < ttl_line(L))
        {
            printf("%c", info(P));
            while (info(P) != '\n')
            {
                P = next(P);
                printf("%c", info(P));
            }
        }
        else if (line(P) == ttl_line(L))
        {
            printf("%c", info(P));
            printfr(P);
        }
    }
}

void print_ln(list &L, addr P)
{
    if (is_bgline(L, P) == true)
    {
        printfr_ln(L, P);
    }
    else
    {
        to_bgline(P);

        printfr_ln(L, P);
    }
}

void rfrsh_line(list *L)
{
    addr P;
    int line;

    line = 1;

    P = first(*L);
    line(P) = line;
    while (P != last(*L))
    {
        P = next(P);
        line(P) = line;
        if (info(P) == '\n')
        {
            line++;
        }
    }
}

void set_csr_pos(int line, int column)
{
    HANDLE hStdout;
    COORD pos;

    pos.X = (short)column;
    pos.Y = (short)line;

    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hStdout, pos);
}

int cnt_col_csr_up(list &L, addr &temp)
{
    int count;
    addr prev;
    prev = temp;
    to_bgline(temp);
    count = 1;

    while (temp != prev)
    {
        temp = next(temp);
        count++;
    }

    return count;
}

void csr_up(list &L, addr *cursor)
{
    addr P;

    P = *cursor;
    while (line(*cursor) > line(P) - 1)
    {
        *cursor = prev(*cursor);
    }

    for (int i = 0; i <= (ttlnode_l(L, *cursor) - cnt_col_csr_up(L, P)); i++)
    {
        *cursor = prev(*cursor);
    }
}

void csr_up_only_line(addr *cursor)
{
    to_bgline(*cursor);
    *cursor = prev(*cursor);
}

int cnt_col_csr_dwn(list &L, addr &temp)
{
    addr prv_line;
    int count;

    count = 1;
    prv_line = temp;
    to_bgline(temp);
    while (temp != prv_line)
    {
        temp = next(temp);
        count++;
    }
    return count;
}

void csr_down(list &L, addr *cursor)
{
    addr P;

    P = *cursor;
    while (line(*cursor) < line(P) + 1)
    {
        *cursor = next(*cursor);
    }

    for (int i = 0; i < cnt_col_csr_dwn(L, P); i++)
    {
        *cursor = next(*cursor);
    }
}

void csr_down_spcl(list &L, addr *cursor)
{
    addr P;

    P = *cursor;
    while (line(*cursor) < line(P) + 1)
    {
        *cursor = next(*cursor);
    }

    for (int i = 1; i < ttlnode_l(L, *cursor); i++)
    {
        *cursor = next(*cursor);
    }
}

bool is_linecurr_bgr_thn_linenxt(list &L, addr &P)
{
    if (cnt_col_csr_dwn(L, P) > ttlnode_l(L, to_nexline(L, P)))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool is_linecurr_sml_thn_linenxt(list &L, addr &P)
{
    if (cnt_col_csr_dwn(L, P) < ttlnode_l(L, to_nexline(L, P)))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void csr_left(list &L, addr *cursor)
{
}

void csr_right(list &L, addr *cursor, POS &cp)
{
}

void csr_fst_ll(list &L, addr &P)
{
    P = first(L);
}

void csr_lst_ll(list &L, addr &P)
{
    P = last(L);
}

bool is_std_in(infotype &buffer)
{
    if (isprint(buffer))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void std_in(list &L, addr &P, infotype &buffer)
{
    if (is_enlist(L, P) == true)
    {
        insla(&L, buffer);
    }
    else
    {
        insaf(&L, buffer, &P);
    }
}

bool is_backspace_in(infotype &buffer)
{
    if (buffer == BACKSPACE_KEY)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool is_delete_in(infotype &buffer)
{
    if (buffer == DELETE)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool is_return_in(infotype &buffer)
{
    if (buffer == RETURN_KEY)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool is_tab_in(infotype &buffer)
{
    if (buffer == TAB_KEY)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void backspace_in(list &L, addr &P, infotype &buffer)
{
    if (P != first(L))
    {
        delp(&L, P);
    }
}

bool is_ctrl_in(infotype &buffer)
{
    if (buffer == CONTROL_1_KEY || buffer == CONTROL_2_KEY || buffer == CONTROL_3_KEY)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool is_save_in(infotype &buffer)
{
    if (buffer == CTRL_S_KEY)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool is_saveas_in(infotype &buffer)
{
    if (buffer == CTRL_X_KEY)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool is_open_in(infotype &buffer)
{
    if (buffer == CTRL_O_KEY)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool is_new_in(infotype &buffer)
{
    if (buffer == CTRL_N_KEY)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool is_quit_in(infotype &buffer)
{
    if (buffer == CTRL_Q_KEY)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool is_copy_in(infotype &buffer)
{
    if (buffer == CTRL_C_KEY)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool is_paste_in(infotype &buffer)
{
    if (buffer == CTRL_V_KEY)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool is_find_in(infotype &buffer)
{
    if (buffer == CTRL_F_KEY)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool is_replace_in(infotype &buffer)
{
    if (buffer == CTRL_R_KEY)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void delete_in(list &L, addr &P, infotype &buffer)
{
    delaf(&L, P);
}

void return_in(list &L, addr &P, infotype &buffer)
{
    buffer = '\n';
    if (is_enlist(L, P) == true)
    {
        insla(&L, buffer);
    }
    else
    {
        insaf(&L, buffer, &P);
    }
}

void tab_in(list &L, addr &P, infotype &buffer)
{
    buffer = '\t';
    if (is_enlist(L, P) == true)
    {
        insla(&L, buffer);
    }
    else
    {
        insaf(&L, buffer, &P);
    }
}

index get_current_lnpos(addr &P)
{
    return line(P);
}

index get_current_colpos(list &L, addr &cursor)
{
    addr temp;
    int count;

    count = 0;
    temp = cursor;

    while ((line(temp) == line(cursor)) && (first(L) != temp))
    {
        temp = prev(temp);
        count++;
    }

    if (first(L) == temp)
    {
        count++;
    }

    return count;
}

void paste(list &L_source, list &main_text, addr last_cursor)
{
    /* Kamus Data */
    addr P_copy;

    P_copy = first(L_source);

    /* Algoritma */
    if (next(last_cursor) != Nil)
    {
        while (P_copy != Nil)
        {
            if (next(P_copy) != Nil)
            {
                insaf_or(&main_text, info(P_copy), &last_cursor);
            }
            else
            {
                insaf_or(&main_text, info(P_copy), &last_cursor);
            }
            P_copy = next(P_copy);
            (last_cursor) = next(last_cursor);
        }
    }
    else
    {
        while (P_copy != Nil)
        {
            if (next(P_copy) != Nil)
            {
                insla(&main_text, info(P_copy));
            }
            else
            {
                insla(&main_text, info(P_copy));
            }
            P_copy = next(P_copy);
            (last_cursor) = next(last_cursor);
        }
    }
}

void save(list &L)
{
    addr P;
    ofstream myfile;
    ifstream check;
    char FileName[25];

    P = first(L);
    boxes(1, 0, "Nama file : ");
    cin >> FileName;
    check.open(FileName);
    if (check)
    {
        myfile.open(FileName, ios::out);
        if (myfile.is_open())
        {
            while (P != Nil)
            {
                myfile << info(P);
                P = next(P);
            }
            boxes(1, 0, "Pembaruan sudah disimpan!");
        }
    }
    else
    {
        myfile.open(FileName);
        if (myfile.is_open())
        {
            while (P != Nil)
            {
                myfile << info(P);
                P = next(P);
            }
            boxes(1, 0, "File telah dibuat!");
        }
    }
    myfile.close();

    getch();
    clear();
    set_csr_pos(1, 0);
}

void save_as(list &L, char *destination_filename)
{
    addr P;
    FILE *destination;

    P = first(L);
    destination = fopen(destination_filename, "w");
    if (destination == Nil)
    {
        perror("File tidak ditemukan.");
        clearerr(destination);
        fclose(destination);
    }
    else
    {
        while (P != Nil)
        {
            fprintf(destination, "%c", info(P));
            P = next(P);
        }
        printf("File %s telah disimpan.", destination_filename);
        fclose(destination);
    }

    clear();
    set_csr_pos(1, 0);
}

void clear()
{
    /* Origin : https://docs.microsoft.com/en-us/windows/console/clearing-the-screen */
    /* Dengan penyesuaian */
    HANDLE hStdout;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    SMALL_RECT scrollRect;
    COORD scrollTarget;
    CHAR_INFO fill;

    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

    // Get the number of character cells in the current buffer.
    if (!GetConsoleScreenBufferInfo(hStdout, &csbi))
    {
        return;
    }

    // Scroll the rectangle of the entire buffer.
    scrollRect.Left = 0;
    scrollRect.Top = 1;
    scrollRect.Right = csbi.dwSize.X;
    scrollRect.Bottom = csbi.dwSize.Y;

    // Scroll it upwards off the top of the buffer with a magnitude of the entire height.
    scrollTarget.X = 0;
    scrollTarget.Y = (SHORT)(0 - csbi.dwSize.Y);

    // Fill with empty spaces with the buffer's default text attribute.
    fill.Char.UnicodeChar = TEXT(' ');
    fill.Attributes = csbi.wAttributes;

    // Do the scroll
    ScrollConsoleScreenBuffer(hStdout, &scrollRect, NULL, scrollTarget, &fill);

    // Move the cursor to the top left corner too.
    csbi.dwCursorPosition.X = 0;
    csbi.dwCursorPosition.Y = 0;

    SetConsoleCursorPosition(hStdout, csbi.dwCursorPosition);
}

void highlight(int Xpos, int Ypos, int nLen)
{
    HANDLE hStdout;
    COORD coord;
    DWORD dwWritten;

    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    coord = {(SHORT)Xpos, (SHORT)Ypos};
    FillConsoleOutputAttribute(hStdout, COMMON_LVB_GRID_HORIZONTAL | BACKGROUND_INTENSITY, (DWORD)nLen, coord, &dwWritten);
}

void boxes(int Ypos, int Xpos, char str[])
{
    set_csr_pos(Ypos, Xpos);

    printf("\xDA");
    for (int i = 0; i < 50; i++)
    {
        printf("\xC4");
    }
    printf("\xBF\n");

    printf("\xB3%s", str);
    for (int i = 0; i < 50 - strlen(str); i++)
    {
        printf(" ");
    }
    printf("\xB3\n");

    printf("\xC0");
    for (int i = 0; i < 50; i++)
    {
        printf("\xC4");
    }
    printf("\xD9\n");

    set_csr_pos((Ypos + 1), (strlen(str) + 1));
}

void help()
{
    system("cls");
    fstream myFile;
    myFile.open("help.txt");
    string line;
    char buffer;

    while (getline(myFile, line))
    {
        cout << line << endl;
    }
    myFile.close();

    buffer = getch();
    if (buffer == 25)
    {
        system("cls");
    }
}

void arrow(int realPosition, int arrowPosition)
{
    if (realPosition == arrowPosition)
    {
        printf(">");
    }
    else
    {
        printf(" ");
    }
}

void menu_file(list &L, list &Lc, addr P, char name_file[])
{
    int post = 1;
    int keyPressed = 0;
    while (keyPressed != 13)
    {
        clear();
        set_csr_pos(1, 0);
        printli(L);
        set_csr_pos(1, 0);

        printf("\xDA\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xBF\n");
        printf("\xB3 ");
        arrow(1, post);
        printf("New      \xB3\n");
        printf("\xB3 ");
        arrow(2, post);
        printf("Open     \xB3\n");
        printf("\xC3\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xB4\n");
        printf("\xB3 ");
        arrow(3, post);
        printf("Save     \xB3\n");
        printf("\xB3 ");
        arrow(4, post);
        printf("Save As  \xB3\n");
        printf("\xC3\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xB4\n");
        printf("\xB3 ");
        arrow(5, post);
        printf("Exit     \xB3\n");
        printf("\xC0\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xD9\n");

        keyPressed = getch();
        if (keyPressed == 80 && post != 5)
        {
            post++;
        }
        else if (keyPressed == 72 && post != 1)
        {
            post--;
        }
        else if (keyPressed == 77 || keyPressed == 75)
        {
            return;
        }
        else
        {
            post = post;
        }

        if (post == 1 && keyPressed == 13)
        {
            clear();
            newfl(L, P);
        }
        else if (post == 2 && keyPressed == 13)
        {
            clear();
            open(&L);
            printli(L);
        }
        else if (post == 3 && keyPressed == 13)
        {
            clear();
            save(L);
            printli(L);
        }
        else if (post == 4 && keyPressed == 13)
        {
            clear();
            boxes(1, 0, "Nama file : ");
            scanf("%s", name_file);

            save_as(L, name_file);
            printli(L);
        }
        else if (post == 5 && keyPressed == 13)
        {
            clste(L, Lc);
        }
    }
}

void menu_edit(list &L, list &Lc, char replace_text[], addr P)
{
    int post = 1;
    int keyPressed = 0;
    while (keyPressed != 13)
    {
        clear();
        set_csr_pos(1, 0);
        printli(L);
        set_csr_pos(1, 0);

        printf("      \xDA\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xBF\n");
        printf("      \xB3 ");
        arrow(1, post);
        printf("Copy     \xB3\n");
        printf("      \xB3 ");
        arrow(2, post);
        printf("Paste    \xB3\n");
        printf("      \xC3\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xB4\n");
        printf("      \xB3 ");
        arrow(3, post);
        printf("Find     \xB3\n");
        printf("      \xB3 ");
        arrow(4, post);
        printf("Replace  \xB3\n");
        printf("      \xC0\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xD9\n");

        keyPressed = getch();
        if (keyPressed == 80 && post != 4)
        {
            post++;
        }
        else if (keyPressed == 72 && post != 1)
        {
            post--;
        }
        else if (keyPressed == 77 || keyPressed == 75)
        {
            return;
        }
        else
        {
            post = post;
        }

        if (post == 1 && keyPressed == 13)
        {
            clear();
            copy(Lc, L);
            printli(L);
        }
        if (post == 2 && keyPressed == 13)
        {
            clear();
            paste(Lc, L, P);
            printli(L);
        }
        if (post == 3 && keyPressed == 13)
        {
            clear();
            find(L);
            printli(L);
        }
        if (post == 4 && keyPressed == 13)
        {
            clear();
            boxes(1, 0, "Kata untuk diganti : ");
            gets(replace_text);

            replace(replace_text, strlen(replace_text), L);
            printli(L);
        }
    }
}

void menu_bar(list &L, list &Lc, addr P, char replace_text[], char name_file[])
{
    int post = 1;
    int buffer = 0;

    while (1)
    {
        clear();

        if (post == 1)
        {
            set_csr_pos(0, 0);
            printf(" FILE  Edit  Help ");
            highlight(0, 0, 6);
        }
        if (post == 2)
        {
            set_csr_pos(0, 0);
            printf(" File  EDIT  Help ");
            highlight(6, 0, 6);
        }
        if (post == 3)
        {
            set_csr_pos(0, 0);
            printf(" File  Edit  HELP ");
            highlight(12, 0, 6);
        }

        buffer = getch();
        if (buffer == 77 && post != 3)
        {
            post++;
        }
        else if (buffer == 75 && post != 1)
        {
            post--;
        }
        else
        {
            post = post;
        }
        if (buffer == 27)
        {
            set_csr_pos(0, 0);
            printf(" File  Edit  Help ");
            return;
        }
        else if (buffer == 13 && post == 1)
        {
            menu_file(L, Lc, P, name_file);
        }
        else if (buffer == 13 && post == 2)
        {
            menu_edit(L, Lc, replace_text, P);
        }
        else if (buffer == 13 && post == 3)
        {
            help();
        }
    }
}

void open(list *L)
{
    addr P;
    index CL;
    ifstream MyFile;
    char FileName[25];
    char buffer;
    bool is_newline;

    P = last(*L);
    CL = 1;
    is_newline = false;

    delli(L);
    boxes(1, 0, "Nama file : ");
    cin >> FileName;
    MyFile.open(FileName);
    while (MyFile.get(buffer))
    {
        insla(L, buffer);
        P = last(*L);

        if (buffer == '\n')
        {
            is_newline = true;
        }
        else
        {
            is_newline = false;
        }

        if (is_newline == true)
        {
            line(P) = CL;
            CL = CL + 1;
            is_newline = false;
        }
        else
        {
            line(P) = CL;
        }
    }
    MyFile.close();

    clear();
    set_csr_pos(1, 0);
}

void copy(list &L_copy, list &L_source)
{
    /* Kamus Data */
    list L_dest;
    addr P_src, P_dest;
    char buffer;
    int cocok, sisa, counter;
    bool loop;

    loop = true;
    cocok = 0;
    counter = 0;
    sisa = 0;

    /* Algoritma */
    crlt(&L_dest);
    delli(&L_copy);
    boxes(1, 0, "Kata untuk disalin : ");
    for (;;)
    {
        buffer = getche();

        if (isprint(buffer))
        {
            insla(&L_dest, buffer);
        }

        if (buffer == 13)
        {
            break;
        }
    }

    counter = ttlnode(L_dest);
    P_src = first(L_source);
    P_dest = first(L_dest);

    while ((P_src != Nil) && (loop == true))
    {
        if (info(P_src) == info(P_dest))
        {
            if (next(P_dest) != Nil)
            {
                cocok++;
                insla(&L_copy, info(P_dest));
                P_dest = next(P_dest);
            }
            else
            {
                cocok = counter;
                insla(&L_copy, info(P_dest));
            }

            sisa = counter - cocok;
            if ((next(P_src) == Nil) && (cocok < counter) && (sisa > 1))
            {
                delli(&L_copy);
                cocok = 0;
                loop = false;
            }
        }
        else
        {
            delli(&L_copy);
            P_dest = first(L_dest);
            cocok = 0;
        }

        if (cocok == counter)
        {
            loop = false;
        }
        else if (cocok < counter)
        {
            P_src = next(P_src);
        }
    }

    if (ckliempty(L_copy) == true)
    {
        boxes(4, 0, "Copy gagal, kata tidak ditemukan!");
        getch();
    }
    else
    {
        boxes(4, 0, "Copy sukses, kata ditemukan!");
        getch();
    }

    delli(&L_dest);

    clear();
    set_csr_pos(1, 0);
}

void find(list &Text)
{
    char find_word[20];
    addr P;
    int i = 0,
        cocok = 0,
        count = 0;
    P = first(Text);

    boxes(1, 0, "Cari : ");
    scanf("%s", &find_word);
    // int count_word = strlen(find_word);

    if (P == Nil)
    {
        printf("Text Kosong");
    }
    else
    {
        while (P != Nil)
        {
            if (info(P) == find_word[i])
            {
                i++;
                cocok++;
                if (cocok == strlen(find_word))
                {
                    i = 0;
                    cocok = 0;
                    count++;
                }
            }
            else
            {
                i = 0;
                cocok = 0;
            }
            P = next(P);
        }
    }
    boxes(4, 0, "Ditemukan sebanyak : ");
    printf("%d", count);
    getch();

    clear();
    set_csr_pos(1, 0);
}

void replace(char c[], int counter, list &Text)
{
    addr P;
    int i = 0,
        cocok = 0,
        count_input = 0;
    P = first(Text);
    char input[50], buffer;

    boxes(4, 0, "Ganti dengan : ");
    fflush(stdin);
    scanf("%[^\n]", &input);

    count_input = strlen(input);
    if (P == Nil)
    {
        boxes(9, 0, "Teks kosong!");
    }
    else
    {
        clear();
        while (P != Nil)
        {
            if (info(P) == c[i])
            {
                i++;
                cocok++;

                if (cocok == counter)
                {
                    /* PROSES REPLACE AKHIR */
                    if (next(P) == Nil)
                    {
                        for (int j = 0; j < counter; j++)
                        {
                            della(&Text);
                        }

                        P = last(Text);
                        if (!ckliempty(Text))
                        {
                            for (int k = 0; k < count_input; k++)
                            {
                                insaf_or(&Text, input[k], &P);
                                P = next(P);
                            }
                        }
                        else
                        {
                            insfi(&Text, input[0]);
                            P = first(Text);
                            for (int k = 1; k < count_input; k++)
                            {
                                insla(&Text, input[k]);
                                P = next(P);
                            }
                        }
                    }
                    else
                    {
                        P = next(P);
                        for (int j = 0; j < counter; j++)
                        {
                            delbe(&Text, P);
                        }

                        if (prev(P) == NULL)
                        {
                            insfi(&Text, input[0]);
                            P = first(Text);
                            for (int k = 1; k < count_input; k++)
                            {
                                insaf_or(&Text, input[k], &P);
                                P = next(P);
                            }
                        }
                        else
                        {
                            P = prev(P);
                            for (int k = 0; k < count_input; k++)
                            {
                                insaf_or(&Text, input[k], &P);
                                P = next(P);
                            }
                        }
                    }
                    i = 0;
                    cocok = 0;
                }
            }
            else
            {
                i = 0;
                cocok = 0;
            }
            P = next(P);
        }
    }
    getch();

    clear();
    set_csr_pos(1, 0);
}

void newfl(list &L, addr &P)
{
    delli(&L);

    clear();
    set_csr_pos(0, 1);
}

void clste(list &L, list &Lcp)
{
    char opt;

    boxes(1, 0, "Yakin untuk keluar? ");
    opt = getche();
    opt = toupper(opt);

    if (opt == 'Y')
    {
        delli(&L);
        delli(&Lcp);
        exit(0);
    }
    else
    {
        clear();
        printli(L);
    }
}

void list_stat(list &L, addr P, int col)
{
    set_csr_pos(0, 25);
    printf("Ln %3d       Col %3d      Ttl ch : %d", line(P), col, ttlnode(L));
}
