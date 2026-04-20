#include "prc/prc_window.h"
#include <locale.h>

int main(void)
{
    setlocale(LC_ALL, "");

    initscr();

#ifdef PRC_ECHO
    if (!PRC_ECHO)
        noecho();
#endif /* ECHO */

    struct prc_border_desc b = {0};
    
    struct prc_window w = {0};
    w.height = 10;
    w.width = 20;
    w.x = 10;
    w.y = 10;

    struct prc_pad_desc pad = {0};
    pad.left = 20;
    pad.right = 20;
    pad.top = 10;
    pad.bottom = 10;

    uint32_t r = prc_create_window(&w, &b, NULL, PRC_ALIGN_BOTTOMRIGHT);
    mvprintw(0, 0, "%d\n", r);

    prc_window_title(
        &w, "This title", 4, 5,
        PRC_ALIGN_TOP
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
