#include "prc/prc_window.h"
#include "utlprc/types.h"

#include <ncurses.h>
#include <string.h>

fnresult_t prc_create_window(
    struct prc_window *     window,
    struct prc_border_desc *border,
    struct prc_pad_desc *   pad,
    enum prc_align          align
)
{
    if (window == NULL)
        return FN_INVALID_ARGUMENT;

    uint32_t ay;
    uint32_t ax;

    if (pad == NULL)
    {
        uint32_t res =
            prc_get_walginyx(stdscr, window->height, window->width, align, &ay, &ax);

        if (res == FN_FAILURE)
            return FN_FAILURE;
        else if (res == FN_SUCCESS)
        {
            window->y = ay;
            window->x = ax;
        }
    }
    else
    {
        if (prc_get_padded_wdesc(window, stdscr, pad) != FN_SUCCESS)
            return FN_FAILURE;
    }

    window->win = newwin(window->height, window->width, window->y, window->x);
    if (window->win == NULL)
        return FN_FAILURE;

    if (border == NULL)
        return FN_SUCCESS;

    if (wborder(
        window->win,
        border->ls, border->rs,
        border->ts, border->bs,
        border->tl, border->tr,
        border->bl, border->br
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
    WINDOW *restrict    win,
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

    switch(align)
    {
        case PRC_ALIGN_TOPLEFT:
            *y = 0;
            *x = 0;

            break;
        
        case PRC_ALIGN_TOP:
            *y = 0;
            *x = (wmx >> 1) - (tl >> 1);
            
            break;

        case PRC_ALIGN_TOPRIGHT:
           *y = 0; 
            *x = wmx - tl;

            break;

        case PRC_ALIGN_LEFT:
            *y = wmy >> 1;
            *x = 0;

            break;

        case PRC_ALIGN_CENTRE:

            *y = wmy >> 1;
            *x = (wmx >> 1) - (tl >> 1);

            break;

        case PRC_ALIGN_RIGHT:

            *y = wmy >> 1;
            *x = wmx - tl;

            break;

        case PRC_ALIGN_BOTTOMLEFT:
            *y = my;
            *x = 0;

            break;

        case PRC_ALIGN_BOTTOM:

            *y = my;
            *x = (wmx >> 1) - (tl >> 1);

            break;

        case PRC_ALIGN_BOTTOMRIGHT:
            *y = my;
            *x = wmx - tl;

            break;

        default:
            return FN_NO_ARGS;
    }

    return FN_SUCCESS;
}

fnresult_t prc_get_walginyx(
    WINDOW *restrict    basewin,
    uint32_t            height,
    uint32_t            width,
    enum prc_align      align,
    uint32_t *          y,
    uint32_t *          x
)
{
    if (basewin == NULL || y == NULL || x == NULL)
        return FN_INVALID_ARGUMENT;

    uint32_t bmy;
    uint32_t bmx;
    getmaxyx(basewin, bmy, bmx);

    switch(align)
    {
        case PRC_ALIGN_TOPLEFT:
            *y = 0;
            *x = 0;

            break;

        case PRC_ALIGN_TOP:
            *y = 0;
            *x = (bmx >> 1) - (width >> 1);

            break;

        case PRC_ALIGN_TOPRIGHT:
            *y = 0;
            *x = bmx - width;

            break;

        case PRC_ALIGN_LEFT:
            *y = (bmy >> 1) - (height >> 1);
            *x = 0;

            break;

        case PRC_ALIGN_CENTRE:
            *y = (bmy >> 1) - (height >> 1);
            *x = (bmx >> 1) - (width >> 1);

            break;

        case PRC_ALIGN_RIGHT:
            *y = (bmy >> 1) - (height >> 1);
            *x = bmx - width;

            break;

        case PRC_ALIGN_BOTTOMLEFT:
            *y = bmy - height;
            *x = 0;

            break;

        case PRC_ALIGN_BOTTOM:
            *y = bmy - height;
            *x = (bmx >> 1) - (width >> 1);

            break;

        case PRC_ALIGN_BOTTOMRIGHT:
            *y = bmy - height;
            *x = bmx - width;

            break;

        default:
            return FN_NO_ARGS;
    }

    return FN_SUCCESS;
}

fnresult_t prc_get_padded_wdesc(
    struct prc_window *     window,
    WINDOW *restrict        basewin,
    struct prc_pad_desc *   pad
)
{
    if (window == NULL || basewin == NULL || pad == NULL)
        return FN_INVALID_ARGUMENT;

    uint32_t wh;
    uint32_t ww;
    uint32_t pl = pad->left;
    uint32_t pr = pad->right;
    uint32_t pt = pad->top;
    uint32_t pb = pad->bottom;

    getmaxyx(basewin, wh, ww);
    if (ww <= (pl + pr) || wh <= (pt + pb))
        return FN_FAILURE;

    window->x = (uint32_t) getbegx(basewin) + pl;
    ww -= pl + pr;

    window->y = (uint32_t) getbegy(basewin) + pt;
    wh -= pt + pb;

    window->height = wh;
    window->width = ww; 
    
    return FN_SUCCESS;
}