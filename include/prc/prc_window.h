#ifndef PRC_WINDOW_H
#define PRC_WINDOW_H

#include <ncurses.h>

#include "utlprc/types.h"

#include <stdint.h>

#define PRC_ALIGN_LEFT (108U)
#define PRC_ALIGN_CENTRE (99U)
#define PRC_ALIGN_RIGHT (114U)
#define PRC_ALIGN_NONE (0U)

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
};

fnresult_t prc_create_window(
    struct prc_window *window
);

void prc_destroy_window(
    struct prc_window *window
);

fnresult_t prc_window_title(
    const struct prc_window *   window,
    const char *                title,
    const uint32_t              y,
    const uint32_t              x,
    const uint8_t               align
);

#endif /* PRC_WINDOW_H */