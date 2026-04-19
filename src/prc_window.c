#include "prc/prc_window.h"
#include "utlprc/types.h"

#include <string.h>

fnresult_t prc_create_window(struct prc_window *window)
{
    if (window == NULL)
        return FN_INVALID_ARGUMENT;

    window->win = newwin(window->height, window->width, window->y, window->x);
    if (window->win == NULL)
        return FN_FAILURE;

    if (window->border == NULL)
        return FN_SUCCESS;

    if (wborder(
        window->win,
        window->border->ls, window->border->rs,
        window->border->ts, window->border->bs,
        window->border->tl, window->border->tr,
        window->border->bl, window->border->br
    ) != OK)
    {
        delwin(window->win);
        return FN_FAILURE;
    }

    return FN_SUCCESS;
}

void prc_destroy_window(struct prc_window *window)
{
    delwin(window->win);
}

fnresult_t prc_window_title(
    const struct prc_window *   window,
    const char *                title,
    const uint32_t              y,
    const uint32_t              x,
    const uint8_t               align
)
{
    if (window == NULL || title == NULL)
        return FN_INVALID_ARGUMENT;

    switch(align)
    {
        case PRC_ALIGN_LEFT:
            mvwaddstr(window->win, 0, 1, title);
            break;
        
        case PRC_ALIGN_CENTRE:
        {
            __attribute__((__unused__)) uint32_t wmy; 
            uint32_t wmx;
            getmaxyx(window->win, wmy, wmx);

            uint32_t tl = strlen(title);
            mvwaddstr(window->win, 0, (wmx >> 1) - (tl >> 1), title);
            break;
        }

        case PRC_ALIGN_RIGHT:
        {
            __attribute__((__unused__)) uint32_t wmy;
            uint32_t wmx;
            getmaxyx(window->win, wmy, wmx);
            
            uint32_t tl = strlen(title);
            mvwaddstr(window->win, 0, wmx - tl - 1, title);
            break;
        }

        default:
            mvwaddstr(window->win, y, x, title);
            break;
    }

    return FN_SUCCESS;
}