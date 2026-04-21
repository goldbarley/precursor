#include <ncurses.h>

#include "prc/prc_window.h"
#include "prc/prc_context.h"
#include "utlprc/types.h"

#include <string.h>


static inline fnresult_t _prc_get_window_info(
    struct prc_window *     window,
    struct prc_pad_desc *   pad,
    enum prc_align          align,
    struct prc_context *    ctx
)
{
    if (window == NULL)
        return FN_INVALID_ARGUMENT;

    if (pad == NULL)
    {
        uint32_t res =
            prc_get_walginyx(ctx, window->height, window->width,
                align, &window->y, &window->x);

        if (res == FN_FAILURE)
            return FN_FAILURE;
    }
    else
    {
        if (prc_get_padded_wdesc(window, ctx, pad) != FN_SUCCESS)
            return FN_FAILURE;
    }

    return FN_SUCCESS;
}

static inline fnresult_t _prc_draw_window_border(
    struct prc_window *window,
    struct prc_border_desc *border
)
{
    if (border == NULL || window == NULL)
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

fnresult_t prc_create_window(
    struct prc_window *     window,
    struct prc_border_desc *border,
    struct prc_pad_desc *   pad,
    enum prc_align          align,
    struct prc_context *    ctx
)
{
    if (window == NULL)
        return FN_INVALID_ARGUMENT;

    if (_prc_get_window_info(window, pad, align, ctx) != FN_SUCCESS)
        return FN_FAILURE;

    window->win = newwin(window->height, window->width, window->y, window->x);
    if (window->win == NULL)
        return FN_FAILURE;

    prc_change_context_focus(window, ctx);

    return _prc_draw_window_border(window, border);
}

fnresult_t prc_create_focused_derwin(
    struct prc_window *     window,
    struct prc_border_desc *border,
    struct prc_pad_desc *   pad,
    enum prc_align          align,
    struct prc_context *    ctx
)
{
    if (window == NULL || ctx == NULL)
        return FN_INVALID_ARGUMENT;

    if (_prc_get_window_info(window, pad, align, ctx) != FN_SUCCESS)
        return FN_FAILURE;

    window->win =
        derwin(ctx->fwin->win, 
            window->height, window->width, window->y, window->x);
    
    prc_change_context_focus(window, ctx);

    return _prc_draw_window_border(window, border);
}

fnresult_t prc_create_derwin(
    struct prc_window *     window,
    struct prc_window *     parent,
    struct prc_border_desc *border,
    struct prc_pad_desc *   pad,
    enum prc_align          align,
    struct prc_context *    ctx
)
{
    if (window == NULL || ctx == NULL)
        return FN_INVALID_ARGUMENT;

    if (_prc_get_window_info(window, pad, align, ctx) != FN_SUCCESS)
        return FN_FAILURE;

    window->win =
        derwin(parent->win, 
            window->height, window->width, window->y, window->x);
    
    prc_change_context_focus(window, ctx);

    return _prc_draw_window_border(window, border);
}

void prc_destroy_window(
    struct prc_window * window,
    struct prc_context *ctx
)
{
    if (window == NULL)
        return;

    delwin(window->win);

    if (ctx == NULL)
        return;

    if (ctx->fwin == window)
        ctx->fwin = NULL;

    if (ctx->pwin == window)
        ctx->pwin = NULL;
}

fnresult_t prc_window_title(
    struct prc_window *         window,
    const char *                title,
    const uint32_t              y,
    const uint32_t              x,
    enum prc_align              align,
    struct prc_context *        ctx
)
{
    if (window == NULL || title == NULL)
        return FN_INVALID_ARGUMENT;

    uint32_t ty;
    uint32_t tx;

    if (prc_get_talginyx(window, strlen(title), align, &ty, &tx)
        == FN_NO_ARGS)
    {
        mvwaddstr(window->win, y, x, title);
        return FN_SUCCESS;
    }

    mvwaddstr(window->win, ty, tx, title);

    prc_change_context_focus(window, ctx);

    return FN_SUCCESS;
}

fnresult_t prc_get_talginyx(
    struct prc_window *restrict     window,
    uint32_t                        tl,
    enum prc_align                  align,
    uint32_t *                      y,
    uint32_t *                      x
)
{
    if (window == NULL || y == NULL || x == NULL)
        return FN_INVALID_ARGUMENT;

    uint32_t wmy;
    uint32_t wmx;
    getmaxyx(window->win, wmy, wmx);

    uint32_t my = wmy - 1;

    if (align == PRC_ALIGN_NONE)
        return FN_NO_ARGS;

    if (align & PRC_ALIGN_TOP)
        *y = 0;
    else if (align & PRC_ALIGN_BOTTOM)
        *y = my;
    else
        *y = wmy >> 1;

    if (align & PRC_ALIGN_LEFT)
        *x = 0;
    else if (align & PRC_ALIGN_RIGHT)
        *x = wmx - tl;
    else
        *x = (wmx - tl) >> 1;

    return FN_SUCCESS;
}

fnresult_t prc_get_walginyx(
    struct prc_context *restrict    basewin,
    uint32_t                        height,
    uint32_t                        width,
    enum prc_align                  align,
    uint32_t *                      y,
    uint32_t *                      x
)
{
    if (basewin == NULL || y == NULL || x == NULL)
        return FN_INVALID_ARGUMENT;

    uint32_t bmy;
    uint32_t bmx;
    getmaxyx(basewin->fwin->win, bmy, bmx);

    if (width > bmx && height > bmy)
        return FN_FAILURE;

    if (align == PRC_ALIGN_NONE)
        return FN_NO_ARGS;

    if (align & PRC_ALIGN_TOP)
        *y = 0;
    else if (align & PRC_ALIGN_BOTTOM)
        *y = bmy - height;
    else
        *y = (bmy - height) >> 1;

    if (align & PRC_ALIGN_LEFT)
        *x = 0;
    else if (align & PRC_ALIGN_RIGHT)
        *x = bmx - width;
    else
        *x = (bmx - width) >> 1;

    return FN_SUCCESS;
}

fnresult_t prc_get_padded_wdesc(
    struct prc_window *             window,
    struct prc_context *restrict    basewin,
    struct prc_pad_desc *           pad
)
{
    if (window == NULL || basewin == NULL || pad == NULL)
        return FN_INVALID_ARGUMENT;

    const WINDOW *fwin = basewin->fwin->win;
    const uint32_t pl = pad->left;
    const uint32_t pr = pad->right;
    const uint32_t pt = pad->top;
    const uint32_t pb = pad->bottom;
    uint32_t wh;
    uint32_t ww;

    getmaxyx(fwin, wh, ww);
    if (ww < (pl + pr) || wh < (pt + pb))
        return FN_FAILURE;

    window->x = (uint32_t) getbegx(fwin) + pl;
    ww -= pl + pr;

    window->y = (uint32_t) getbegy(fwin) + pt;
    wh -= pt + pb;

    window->height = wh;
    window->width = ww; 
    
    return FN_SUCCESS;
}