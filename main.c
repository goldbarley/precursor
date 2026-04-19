#include "prc/prc_window.h"
#include <locale.h>

int main(void)
{
    setlocale(LC_ALL, "");

    initscr();

    struct prc_border_desc b = {0};
    
    struct prc_window w = {0};
    w.border = &b;
    w.height = 10;
    w.width = 20;
    w.x = 10;
    w.y = 10;

    uint32_t r = prc_create_window(&w);
    mvprintw(0, 0, "%d\n", r);

    prc_window_title(
        &w, "This title", 4, 5, PRC_ALIGN_NONE
    );

    refresh();
    wrefresh(w.win);

    char c;
    do
    {
        c = getch();
        refresh();
    } while(c != 'Q' && c != 'q');

    prc_destroy_window(&w);

    endwin();
}