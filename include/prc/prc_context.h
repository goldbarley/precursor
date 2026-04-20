#ifndef PRC_CONTEXT_H
#define PRC_CONTEXT_H

#include <ncurses.h>

#include "utlprc/types.h"

#include <stdint.h>
#include <stdbool.h>

struct prc_context
{
    WINDOW *            cwin;
    struct prc_window * fwin;
    uint32_t            term_y;
    uint32_t            term_x;
    bool                term_has_color;
    bool                term_change_color;
};

fnresult_t prc_get_term_info(
    struct prc_context *ctx
);

#endif /* PRC_CONTEXT_H */