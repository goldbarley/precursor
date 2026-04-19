#ifndef PRC_WINDOW_H
#define PRC_WINDOW_H

#include <ncurses.h>

#include "utlprc/types.h"

#include <stdint.h>

struct prc_border_desc
{
    int32_t ls;
    int32_t rs;
    int32_t ts;
    int32_t bs;
    int32_t tl;
    int32_t tr;
    int32_t bl;
    int32_t br;
};

struct prc_window
{
    struct prc_border_desc *border;
    WINDOW *                win;
    uint32_t                height;
    uint32_t                width;
    uint32_t                x;
    uint32_t                y;
    uint8_t                 align;
};

fnresult_t prc_create_window(
    struct prc_window * window,
    enum prc_align      align
);

void prc_destroy_window(
    struct prc_window *window
);

fnresult_t prc_window_title(
    const struct prc_window *   window,
    const char *                title,
    const uint32_t              y,
    const uint32_t              x,
    enum prc_align              align
);

fnresult_t prc_get_talginyx(
    WINDOW *            win,
    uint32_t            tl,
    enum prc_align      align,
    uint32_t *          y,
    uint32_t *          x
);

fnresult_t prc_get_walginyx(
    WINDOW *        basewin,
    uint32_t        height,
    uint32_t        width,
    enum prc_align  align,
    uint32_t *      y,
    uint32_t *      x
);

#endif /* PRC_WINDOW_H */