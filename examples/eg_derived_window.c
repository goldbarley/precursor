#include "eg.h"
#include "prc/prc_window.h"
#include "utlprc/types.h"

fnresult_t eg_create_derwin(
    struct prc_window *basewin,
    struct prc_window *dervwin,
    struct prc_context *ctx,
    bool bborder, bool dborder,
    bool balign, bool dalign,
    bool bpad, bool dpad
)
{
    if (basewin == NULL || dervwin == NULL || ctx == NULL)
    {
        puts("Error: Invalid argument(s).");
        return FN_INVALID_ARGUMENT;
    }

    prc_get_context(ctx);

    /* Create base window or select 'stdscr'. */
    static struct prc_border_desc _this_bb = {0};
    static struct prc_pad_desc _this_bp = {
        .left = 10,
        .right = 10,
        .top = 5,
        .bottom = 5
    };

    struct prc_border_desc *this_bb = NULL;
    struct prc_pad_desc *this_bp = NULL;
    enum prc_align ba = PRC_ALIGN_NONE;

    if (bborder) 
        this_bb = &_this_bb;
    
    if (bpad)
        this_bp = &_this_bp;

    if (balign)
        ba = PRC_ALIGN_TOPLEFT;

    fnresult_t result =
        prc_create_window(basewin, this_bb, this_bp, ba, ctx);
    if (result != FN_SUCCESS)
    {
        puts("Error: Could not create base window.");
        return FN_FAILURE;
    }
    puts("Info: Base window created.");

    /* Create dervied window */
    static struct prc_border_desc _this_db = {0};
    static struct prc_pad_desc _this_dp = {
        .left = 3,
        .right = 3,
        .top = 2,
        .bottom = 2
    };
    
    struct prc_border_desc *this_db = NULL;
    struct prc_pad_desc *this_dp = NULL;
    enum prc_align da = PRC_ALIGN_NONE; 

    if (dborder)
        this_db = &_this_db;
    if (dpad)
        this_dp = &_this_dp;
    if (dalign)
        da = PRC_ALIGN_BOTTOM;

    // result = prc_chto_focus_mother(ctx);
    // if (result != FN_SUCCESS)
    // {
    //     puts("Error: Could not switch focus.");
    //     return FN_FAILURE;
    // }

    result =
        prc_create_focused_derwin(dervwin, this_db, this_dp, da, ctx);
    if (result != FN_SUCCESS)
    {
        puts("Error: Could not create dervied window.");
        return FN_FAILURE;
    }

    return FN_SUCCESS;
}