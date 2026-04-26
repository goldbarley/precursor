#include "prc/prc_winpool.h"
#include "utlprc/types.h"
#include <string.h>

struct _prc_winpool
{
    struct prc_window windows[PRC_MAX_WINDOW_COUNT];
    uint32_t freemask;
    uint8_t init;
};

static struct _prc_winpool _window_pool;

fnresult_t _prc_init_winpool(void)
{
    if (_window_pool.init)
        return FN_SUCCESS;

    if (memset(_window_pool.windows, 0,
        PRC_MAX_WINDOW_COUNT * sizeof(struct prc_window)) == NULL)
        return FN_FAILURE;
    
    _window_pool.freemask = UINT32_MAX;

    _window_pool.init = TRUE;

    return FN_SUCCESS;
}

struct prc_window *prc_get_freeaddr(void)
{
    _prc_init_winpool();

    uint32_t fm = _window_pool.freemask;

    if (fm == 0)
        return NULL;

    uint32_t fidx = CTZ(fm);
    PRC_MARK_USED(fm, fidx);

    _window_pool.freemask = fm;

    return &_window_pool.windows[fidx];
}

void prc_dalfrom_winpool(struct prc_window *window)
{
    if (!_window_pool.init)
        return;

    uint8_t widx = window - _window_pool.windows;

    if (widx >= PRC_MAX_WINDOW_COUNT)
        return;

    PRC_MARK_FREE(_window_pool.freemask, widx);

    memset(window, 0, sizeof(struct prc_window));
}