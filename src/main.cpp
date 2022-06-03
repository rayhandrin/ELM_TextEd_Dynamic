#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <iostream>
#include <conio.h>
#include <windows.h>

#include "nrdll.hpp"
#include "editor.hpp"

using namespace std;

int main()
{
    list L, Lc;
    infotype I;
    addr P, Pe;
    POS cp;
    int ed_line;
    char replace_text[15], name_file[15];
    bool middle, new_line, is_first;

    /* Setting console buffer and window size by 40 * 80 */
    set_consoleprop(80, 40, 280, 460);

    cp.X = 0;
    cp.Y = 1;
    ed_line = 1;
    middle = false;
    new_line = false;
    is_first = true;

    crlt(&L);
    crlt(&Lc);

    printf(" File  Edit  Help        Ln          Col         Ttl ch : \n");

    for (;;)
    {
        I = getch();

        if (is_std_in(I) == true)
        {
            if (middle == true)
            {
                set_csr_pos(cp.Y, cp.X);
                insaf(&L, I, &P);
            }
            else
            {
                insla(&L, I);
                P = last(L);
                line(P) = ed_line;
            }
            cp.X += 1;
            is_first = false;

            clear();
            rfrsh_line(&L);
            cout << endl;
            printli(L);
        }

        if (is_return_in(I) == true)
        {
            I = '\n';
            if (middle == true)
            {
                insaf(&L, I, &P);
                ed_line = line(P);
                rfrsh_line(&L);
            }
            else
            {
                insla(&L, I);
                ed_line = ed_line + 1;
                rfrsh_line(&L);
            }
            cp.X = 0;
            cp.Y += 1;

            clear();
            cout << endl;
            printli(L);
        }

        if (is_tab_in(I) == true)
        {
            I = '\t';
            if (middle == true)
            {
                insaf(&L, I, &P);
            }
            else
            {
                insla(&L, I);
            }
            cp.X += 1;

            clear();
            cout << endl;
            printli(L);
        }

        if (is_backspace_in(I) == true)
        {
        	if(P != Nil){
        		if (middle == true)
	            {
					P = prev(P);
					delp(&L, next(P)); 
					cp.X -= 1;
				}
	            else
	            {
	                della(&L);
	                P = last(L);
	                cp.X -= 1;
	            }
			}   
            
            clear();
            cout << endl;
            printli(L);
        }

        if (is_ctrl_in(I))
        {
            switch (getch())
            {
            /* Cursor Up */
            case CSR_UP_KEY:
                if (is_bglist(L, P) == true || is_frstlist(L, P) == true)
                {
                    break;
                }
                else if (is_linecurr_bgr_thn_lineprv(L, P) == true)
                {
                    csr_up_only_line(&P);
                    P = prev(P);
                    cp.Y -= 1;
                    cp.X = ttlnode_l(L, P) - 1;
                }
                else if (is_linecurr_sml_thn_lineprv(L, P) == true)
                {
                    cp.Y -= 1;
                    csr_up(L, &P);
                    P = next(P);
                }

                if (is_enline(L, P) == true)
                {
                    middle = false;
                }
                else
                {
                    middle = true;
                }
                break;

            /* Cursor Down */
            case CSR_DOWN_KEY:
                if (is_enlist(L, P) == true || is_lstlist(L, P) == true)
                {
                    break;
                }
                else if (is_linecurr_bgr_thn_linenxt(L, P) == true)
                {
                    csr_down_spcl(L, &P);
                    cp.X = ttlnode_l(L, P);
                    cp.Y += 1;
                }
                else if (is_linecurr_sml_thn_linenxt(L, P) == true)
                {
                    cp.Y += 1;
                    csr_down(L, &P);
                    P = prev(P);
                }

                if (is_enline(L, P) == true)
                {
                    middle = false;
                }
                else
                {
                    middle = true;
                }
                break;

            /* Cursor Right */
            case CSR_RIGHT_KEY:
                if (is_enlist(L, next(P)) == true)
                {
                    cp.X = ttlnode_l(L, P);
                    middle = false;
                    break;
                }

                if ((is_enline(L, next(P)) == true))
                {
                    P = next(P);
                    if (P != last(L))
                    {
                        cp.X = 0;
                        cp.Y += 1;
                    }
                }
                else
                {
                    P = next(P);
                    cp.X += 1;
                }

                if (is_enlist(L, P) == true)
                {
                    middle = false;
                }
                else
                {
                    middle = true;
                }
                break;

            /* Cursor Left */
            case CSR_LEFT_KEY:
                if (is_bglist(L, P) == true)
                {
                    cp.X = 1;
                	P = first(L);
                    break;
                }

                if (is_bgline(L, P) == true)
                {
                    P = prev(P);
                    cp.X = ttlnode_l(L, P) - 1;
                    cp.Y -= 1;
                }
                else
                {
                    P = prev(P);
                    cp.X -= 1;
                }

                if (is_enlist(L, next(P)) == true)
                {
                    middle = false;
                }
                else
                {
                    middle = true;
                }
                break;

            /* Delete */
            case DELETE_KEY:
                if (middle == true)
                {
                    delaf(&L, P);
                }

                clear();
                set_csr_pos(1, 0);
                printli(L);
                break;

            /* F1 */
            case F1_KEY:
                menu_bar(L, Lc, P, replace_text, name_file);
                break;
            }
        }

        if (is_open_in(I) == true) // CTRL + O
        {
            open(&L);
            printli(L);
        }

        if (is_copy_in(I) == true) // CTRL + C
        {
            copy(Lc, L);
            printli(L);
        }

        if (is_paste_in(I) == true) // CTRL + V
        {
            paste(Lc, L, P);
            printli(L);
        }

        if (is_find_in(I) == true) // CTRL + F
        {
            find(L);
            printli(L);
        }

        if (is_replace_in(I) == true) // CTRL + R
        {
            boxes(1, 0, "Kata untuk diganti : ");
            fflush(stdin);
            gets(replace_text);

            replace(replace_text, strlen(replace_text), L);
            printli(L);
        }

        if (is_save_in(I) == true) // CTRL + S
        {
            save(L);
            printli(L);
        }

        if (is_saveas_in(I) == true) // CTRL + X
        {
            boxes(1, 0, "Nama file : ");
            scanf("%s", name_file);

            save_as(L, name_file);
            printli(L);
        }

        if (is_new_in(I) == true) // CTRL + N
        {
            newfl(L, P);
        }

        if (is_quit_in(I) == true) // CTRL + Q
        {
            clste(L, Lc);
        }

        if (is_first == false)
        {
            list_stat(L, P, cp.X);
        }

        set_csr_pos(cp.Y, cp.X);
    }

    return 0;
}
