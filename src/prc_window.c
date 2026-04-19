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
    enum prc_align              align
)
{
    if (window == NULL || title == NULL)
        return FN_INVALID_ARGUMENT;

    uint32_t ty;
    uint32_t tx;

    if (prc_get_talginyx(window->win, strlen(title), align, &ty, &tx)
        == FN_NO_ARGS)
    {
        mvwaddstr(window->win, y, x, title);
        return FN_SUCCESS;
    }

    mvwaddstr(window->win, ty, tx, title);
    return FN_SUCCESS;
}

fnresult_t prc_get_talginyx(
    WINDOW *            win,
    uint32_t            tl,
    enum prc_align      align,
    uint32_t *          y,
    uint32_t *          x
)
{
    if (win == NULL || y == NULL || x == NULL)
        return FN_INVALID_ARGUMENT;

    uint32_t wmy;
    uint32_t wmx;
    getmaxyx(win, wmy, wmx);

    uint32_t my = wmy - 1;
    uint32_t mx = wmx - 1;

    switch(align)
    {
        case PRC_ALIGN_TOPLEFT:
            *y = 0;
            *x = 0;

            break;
        
        case PRC_ALIGN_TOP:
        {
            *y = 0;
            *x = (wmx >> 1) - (tl >> 1);
            
            break;
        }

        case PRC_ALIGN_TOPRIGHT:
        {
            *y = 0; 
            *x = wmx - tl;

            break;
        }

        case PRC_ALIGN_LEFT:
        {
            *y = wmy >> 1;
            *x = 0;

            break;
        }

        case PRC_ALIGN_CENTRE:
        {

            *y = wmy >> 1;
            *x = (wmx >> 1) - (tl >> 1);

            break;
        }

        case PRC_ALIGN_RIGHT:
        {

            *y = wmy >> 1;
            *x = wmx - tl;

            break;
        }

        case PRC_ALIGN_BOTTOMLEFT:
        {
            *y = my;
            *x = 0;

            break;
        }

        case PRC_ALIGN_BOTTOM:
        {
            *y = my;
            *x = (wmx >> 1) - (tl >> 1);

            break;
        }

        case PRC_ALIGN_BOTTOMRIGHT:
        {
            *y = my;
            *x = wmx - tl;

            break;
        }

        default:
            return FN_NO_ARGS;
    }

    return FN_SUCCESS;
}