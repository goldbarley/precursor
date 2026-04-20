#ifndef PRC_WINDOW_H
#define PRC_WINDOW_H

#include <ncurses.h>

#include "prc/prc_context.h"
#include "utlprc/types.h"

#include <stdint.h>

struct prc_border_desc
{
    uint32_t ls;
    uint32_t rs;
    uint32_t ts;
    uint32_t bs;
    uint32_t tl;
    uint32_t tr;
    uint32_t bl;
    uint32_t br;
};

struct prc_pad_desc
{
    uint32_t left;
    uint32_t right;
    uint32_t top;
    uint32_t bottom;
};

struct prc_window
{
    WINDOW *                win;
    uint32_t                height;
    uint32_t                width;
    uint32_t                x;
    uint32_t                y;
    uint8_t                 align;
};

fnresult_t prc_create_window(
    struct prc_window *     window,
    struct prc_border_desc *border,
    struct prc_pad_desc *   pad,
    enum prc_align          align,
    struct prc_context *    ctx
);

void prc_destroy_window(
    struct prc_window *window
);

fnresult_t prc_window_title(
    struct prc_window *         window,
    const char *                title,
    const uint32_t              y,
    const uint32_t              x,
    enum prc_align              align
);

fnresult_t prc_get_talginyx(
    struct prc_window *restrict     win,
    uint32_t                        tl,
    enum prc_align                  align,
    uint32_t *                      y,
    uint32_t *                      x
);

fnresult_t prc_get_walginyx(
    struct prc_context *restrict    basewin,
    uint32_t                        height,
    uint32_t                        width,
    enum prc_align                  align,
    uint32_t *                      y,
    uint32_t *                      x
);

fnresult_t prc_get_padded_wdesc(
    struct prc_window *             window,
    struct prc_context *restrict    basewin,
    struct prc_pad_desc *           pad
);

#endif /* PRC_WINDOW_H */