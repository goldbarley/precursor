#include "eg.h"
#include "prc/prc_window.h"

int main(void)
{
    struct prc_context ctx;
    struct prc_window window;
    struct prc_window dwindow = {0};
    dwindow.height = 2;
    dwindow.width = 2;
#ifdef ECHO
    if (!ECHO)
        noecho()
#endif /* ECHO */

    fnresult_t result = eg_create_derwin(&window, &dwindow, &ctx,
        1, 1, 0, 1, 1, 0);
    // fnresult_t result = eg_single_window(&window, &ctx, 1, 0, 1);
    if (result != FN_SUCCESS)
    {
        puts("Debug: Example failed.");

        return 0;
    }
    refresh();

    uint8_t c;
    do
    {
        wmove(window.win, 1, 1);
        c = wgetch(window.win);
    } while (c != 'q' && c != 'Q');

    puts("Debug: Example succeeded.");

    prc_destroy_window(&dwindow, &ctx);
    prc_destroy_window(&window, &ctx);

    endwin();

    return 0;
}