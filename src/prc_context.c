#include "prc/prc_context.h"
#include <stddef.h>
#include <ncurses.h>

fnresult_t prc_get_term_info(struct prc_context *ctx)
{
    if (ctx == NULL)
        return FN_FAILURE;

    ctx->cwin = stdscr;
    getmaxyx(stdscr, ctx->term_y, ctx->term_x);
    ctx->term_has_color = has_colors();
    ctx->term_change_color = can_change_color();

    return FN_SUCCESS;
}