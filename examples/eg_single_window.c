#include "eg.h"
#include "prc/prc_window.h"

fnresult_t eg_single_window(
    struct prc_window * win,
    struct prc_context *ctx,
    bool                _border,
    bool                _align,
    bool                _pad
)
{
    prc_get_context(ctx);

    struct prc_border_desc _this_border = {0};
    struct prc_pad_desc _this_pad = {
        .left = 10,
        .right = 10,
        .top = 5,
        .bottom = 5
    };

    struct prc_border_desc *this_border = NULL;
    struct prc_pad_desc *this_pad = NULL;
    enum prc_align align = PRC_ALIGN_NONE;

    if (_border)
        this_border = &_this_border;
    if (_align)
        align = PRC_ALIGN_CENTRE;
    if (_pad)
        this_pad = &_this_pad;

    if (prc_create_window(win, this_border, this_pad,
            align, ctx) != FN_SUCCESS)
    {
        printf("Error: Failed to create window.");
        return FN_FAILURE;
    }

    return FN_SUCCESS;
    
}