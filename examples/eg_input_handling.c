#include "prc/prc_context.h"
#include "prc/prc_event.h"
#include "prc/prc_window.h"
#include "utlprc/types.h"

fnresult_t eg_blocking_events(void)
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

    struct prc_generic_event levt = {0};
    struct prc_generic_event fevt = {0};
    
    fnresult_t res = FN_SUCCESS;

    while (TRUE)
    {
        if (prc_wait_for_event(&window) != FN_SUCCESS)
        {
            res = FN_FAILURE;
            break;
        }

        if (prc_get_last_event(&levt) != FN_SUCCESS)
        {
            res = FN_FAILURE;
            break;
        }

        if (prc_get_first_event(&fevt) != FN_SUCCESS)
        {
            res = FN_FAILURE;
            break;
        }

        mvwprintw(window.win, 1, 1, "%c,%c", fevt.detail, levt.detail);

        if (levt.detail == 'q' || levt.detail == 'Q')
        {
            res = FN_SUCCESS;
            break;
        }

        prc_use_event();

        wrefresh(window.win);
    }

    prc_destroy_context(&ctx);
    prc_kill_mother();

    return res;
}

fnresult_t eg_polling_events(void)
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

    struct prc_generic_event levt = {0};
    struct prc_generic_event fevt = {0};
    
    fnresult_t res = FN_SUCCESS;

    while (TRUE)
    {
        if (nodelay(window.win, TRUE) != OK)
            return FN_FAILURE;

        if (prc_poll_for_event(&window) != FN_SUCCESS)
        {
            res = FN_FAILURE;
            break;
        }

        if (prc_get_last_event(&levt) != FN_SUCCESS)
        {
            res = FN_FAILURE;
            break;
        }

        if (prc_get_first_event(&fevt) != FN_SUCCESS)
        {
            res = FN_FAILURE;
            break;
        }

        mvwprintw(window.win, 1, 1, "%c,%c", fevt.detail, levt.detail);

        if (levt.detail == 'q' || levt.detail == 'Q')
        {
            res = FN_SUCCESS;
            break;
        }

        prc_use_event();

        wrefresh(window.win);
    }

    prc_destroy_context(&ctx);
    prc_kill_mother();

    return res;
}