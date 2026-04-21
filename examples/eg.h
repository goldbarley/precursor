#ifndef PRC_EG_H
#define PRC_EG_H

#include "prc/prc_window.h"

/* eg_single_window.c */
fnresult_t eg_single_window(
    struct prc_window * win,
    struct prc_context *ctx,
    bool                borders,
    bool                align,
    bool                pad
);

void eg_destroy_single_window(
    struct prc_window * win,
    struct prc_context *ctx
);

/* eg_derived_window.c */
fnresult_t eg_create_derwin(
    struct prc_window *basewin,
    struct prc_window *dervwin,
    struct prc_context *ctx,
    bool bborder, bool dborder,
    bool balign, bool dalign,
    bool bpad, bool dpad
);


#endif /* PRC_EG_H */