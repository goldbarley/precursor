#include <ncurses.h>

#include "prc/prc_context.h"
#include "prc/prc_window.h"

#include <stddef.h>

/* Will cause segfault of mother is removed. */
static struct prc_window _mother_win;

void _prc_init_mother()
{
    _mother_win.win = stdscr;
    getmaxyx(_mother_win.win, _mother_win.height, _mother_win.width);
    _mother_win.y = 0;
    _mother_win.x = 0;
}

fnresult_t prc_get_context(struct prc_context *ctx)
{
    if (ctx == NULL)
        return FN_FAILURE;

    _prc_init_mother();

    ctx->cwin = &_mother_win;
    ctx->fwin = &_mother_win;
    ctx->pwin = NULL;
    ctx->term_y = _mother_win.height;
    ctx->term_x = _mother_win.width;

    ctx->term_has_color = has_colors();
    ctx->term_change_color = can_change_color();

    return FN_SUCCESS;
}

fnresult_t prc_change_context_focus(
    struct prc_window * window,
    struct prc_context *ctx
)
{
    if (window == NULL || ctx == NULL || window->win == NULL)
        return FN_INVALID_ARGUMENT;

    ctx->pwin = ctx->fwin;
    ctx->fwin = window;

    return FN_SUCCESS;
}

fnresult_t prc_chto_previous_focus(
    struct prc_context *ctx
)
{
    if (ctx == NULL || ctx->pwin == NULL || ctx->fwin == NULL)
        return FN_INVALID_ARGUMENT;

    struct prc_window *tmp = ctx->fwin;
    ctx->fwin = ctx->pwin;
    ctx->pwin = tmp;

    return FN_SUCCESS;
}

fnresult_t prc_resize_context(struct prc_context *ctx)
{
    if (ctx == NULL)
        return FN_INVALID_ARGUMENT;

    _prc_init_mother();
    ctx->term_y = _mother_win.height;
    ctx->term_x = _mother_win.width;

    return FN_SUCCESS;
}

fnresult_t prc_change_mother(struct prc_window *new_mom)
{
    if (new_mom == NULL)
        return FN_INVALID_ARGUMENT;

    /* Cheap fix. TODO: Change this later for a proper copy. */
    _mother_win = *new_mom;

    return FN_SUCCESS;
}

fnresult_t prc_chto_focus_mother(struct prc_context *ctx)
{
    if (ctx == NULL)
        return FN_INVALID_ARGUMENT;

    prc_change_context_focus(&_mother_win, ctx);

    return FN_SUCCESS;
}