#include <ncurses.h>
#include <menu.h>

#include "prc/prc_context.h"
#include "prc/prc_window.h"
#include "utlprc/types.h"

/* Will cause segfault of mother is removed. */
struct _prc_mother_info
{
    struct prc_window mwin;
    bool init;
};

static struct _prc_mother_info _mother = {
    .mwin = {0},
    .init = FALSE
};

static struct _prc_mother_info _grandmother = {
    .mwin = {0},
    .init = FALSE
};

void _prc_init_mother(void)
{
    if (_mother.init)
        return;

    _mother.mwin.win = initscr();
    getmaxyx(_mother.mwin.win, _mother.mwin.height, _mother.mwin.width);
    _mother.mwin.y = 0;
    _mother.mwin.x = 0;

    _mother.init = true;
}

void _prc_resize_mother(void)
{
    _grandmother = _mother;
    getmaxyx(_mother.mwin.win, _mother.mwin.height, _mother.mwin.width);
    _mother.mwin.y = 0;
    _mother.mwin.x = 0;
}

fnresult_t prc_get_context(struct prc_context *ctx)
{
    if (ctx == NULL)
        return FN_FAILURE;

    _prc_init_mother();

    ctx->cwin = &_mother.mwin;
    ctx->fwin = &_mother.mwin;
    ctx->pwin = NULL;
    ctx->term_y = _mother.mwin.height;
    ctx->term_x = _mother.mwin.width;

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

    if (window == ctx->fwin)
        return FN_SUCCESS;

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

    _prc_resize_mother();
        
    ctx->term_y = _mother.mwin.height;
    ctx->term_x = _mother.mwin.width;

    return FN_SUCCESS;
}

__attribute__((__unused__))
static fnresult_t _prc_change_mother(struct prc_window *new_mom)
{
    if (new_mom == NULL)
        return FN_INVALID_ARGUMENT;

    /* Cheap fix. TODO: Change this later for a proper copy. */
    _mother.mwin = *new_mom;

    return FN_SUCCESS;
}

fnresult_t prc_chto_focus_mother(struct prc_context *ctx)
{
    if (ctx == NULL)
        return FN_INVALID_ARGUMENT;

    prc_change_context_focus(&_mother.mwin, ctx);

    return FN_SUCCESS;
}

void prc_destroy_context(struct prc_context *ctx)
{
    if (ctx == NULL)
        return;

    struct prc_window *target[3] = {
        ctx->cwin,
        ctx->fwin,
        ctx->pwin
    };

    for (uint8_t i = 0; i < 3; ++i)
    {
        if (target[i] == NULL)
            continue;

        if (target[i] == &_mother.mwin)
            continue;

        prc_destroy_window(target[i], ctx);
        
    }
}

fnresult_t prc_get_mother(struct prc_window *mother)
{
    if (mother == NULL)
        return FN_INVALID_ARGUMENT;

    *mother = _mother.mwin;

    return FN_SUCCESS;
}

fnresult_t prc_get_grandmother(struct prc_window *grandmother)
{
    if (grandmother == NULL)
        return FN_INVALID_ARGUMENT;

    grandmother = &_grandmother.mwin;

    return FN_SUCCESS;
}

void prc_kill_mother(void)
{
    endwin();
}