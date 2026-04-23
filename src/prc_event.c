#include "prc/prc_event.h"
#include "utlprc/types.h"

struct _prc_event_buffer
{
    struct prc_generic_event buffer[PRC_MAX_EVENT_COUNT];
    uint16_t head;
    uint16_t tail;
    uint8_t init : 1;
};

static struct _prc_event_buffer _event_buffer;

void _prc_init_evt_buffer(void)
{
    if (_event_buffer.init)
        return;

    _event_buffer.head = 0;
    _event_buffer.init = TRUE;
}

fnresult_t prc_update_evt_buffer(void)
{
    if (((_event_buffer.head + 1) & (PRC_MAX_EVENT_COUNT - 1))
        == _event_buffer.tail)
        return FN_OUT_OF_MEMORY;
    
    _event_buffer.tail = (_event_buffer.tail + 1) & (PRC_MAX_EVENT_COUNT - 1);

    return FN_SUCCESS;
}

fnresult_t _prc_get_evt_buffer_status(void)
{
    if (((_event_buffer.head + 1) & (PRC_MAX_EVENT_COUNT - 1))
        == _event_buffer.tail)
        return FN_OUT_OF_MEMORY;
    
    if (_event_buffer.head == _event_buffer.tail)
        return FN_NO_ARGS;

    return FN_SUCCESS;
}

fnresult_t prc_wait_for_event(struct prc_window *window)
{
    if (window == NULL)
        return FN_FAILURE;

#ifdef NO_DELAY
    if (nodelay(window.win, TRUE) != OK)
        return FN_FAILURE;
#endif /* NO_DELAY */

    int32_t keydtl  = wgetch(window->win);
    if (keydtl == ERR)
        return FN_FAILURE;

    if (_prc_get_evt_buffer_status() != FN_OUT_OF_MEMORY)
    {
        _event_buffer.buffer[_event_buffer.head].type = PRC_EVENT_KEY_PRESS;
        _event_buffer.buffer[_event_buffer.head].detail = keydtl;
        _event_buffer.head = (_event_buffer.head + 1) & (PRC_MAX_EVENT_COUNT - 1);
    }

    return FN_SUCCESS;
}

fnresult_t prc_get_first_event(struct prc_generic_event *evt)
{
    if (evt == NULL)
        return FN_INVALID_ARGUMENT;

    *evt = _event_buffer.buffer[_event_buffer.tail];

    return FN_SUCCESS;
}

fnresult_t prc_get_last_event(struct prc_generic_event *evt)
{
    if (evt == NULL)
        return FN_INVALID_ARGUMENT;

    *evt = _event_buffer.buffer[_event_buffer.head];

    return FN_SUCCESS;
}