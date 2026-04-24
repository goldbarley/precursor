#include "prc/prc_event.h"
#define _XOPEN_SOURCE 500

fnresult_t eg_text_box(void)
{
    struct prc_window window;
    struct prc_context ctx;

    prc_get_context(&ctx);
    noecho();
    raw();

    struct prc_border_desc border = {0};
    struct prc_pad_desc pad = {
        .left = 10,
        .right = 10,
        .top = 5,
        .bottom = 5
    };

    enum prc_align align = PRC_ALIGN_NONE;

    if (prc_create_window(&window, &border, &pad,
            align, &ctx) != FN_SUCCESS)
    {
        printf("Error: Failed to create window.");
        return FN_FAILURE;
    }
    prc_window_title(&window, "Example: Input Handling",
        0, 0, PRC_ALIGN_TOP, &ctx);
    refresh();

    prc_init_evt_buffer();
    // struct prc_generic_event levt = {0};
    struct prc_generic_event fevt = {0};
    
    fnresult_t res = FN_SUCCESS;
    uint32_t wy = 1;
    uint32_t wx = 1;

    if (nodelay(window.win, TRUE) != OK)
        return FN_FAILURE;

    while (TRUE)
    {

        if (prc_poll_for_event(&window) != FN_SUCCESS)
        {
            res = FN_FAILURE;
            break;
        }

        while (prc_get_first_event(&fevt) == FN_SUCCESS)
        {
            if (fevt.detail == 'q' || fevt.detail == 'Q')
            {
                res = FN_SUCCESS;
                goto cleanup;
            }

            if (fevt.detail == '\n' || fevt.detail == '\r' || fevt.detail == 10 || fevt.detail == 13)
            {
                wx = 1;
                wy++;
            }
            else 
            {
                if (wx >= window.width - 1)
                {
                    wx = 1;
                    wy++;
                }

                if (wy >= window.height - 1) 
                    break;

                mvwaddch(window.win, wy, wx++, fevt.detail);
            }

            prc_use_event();
        }

        wrefresh(window.win);
        wtimeout(window.win, 10);
    }

    cleanup:
        prc_destroy_context(&ctx);
        prc_kill_mother();

    return res;
}