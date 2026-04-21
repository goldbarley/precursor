#ifndef PRC_TYPES_H
#define PRC_TYPES_H

typedef enum fnresult
{
    FN_SUCCESS,
    FN_FAILURE,
    FN_INVALID_ARGUMENT,
    FN_NO_ARGS
} fnresult_t;

enum prc_align
{
    PRC_ALIGN_NONE          = 0x000000FF,
    PRC_ALIGN_LEFT          = 0x00000001,
    PRC_ALIGN_RIGHT         = 0x00000002,
    PRC_ALIGN_TOP           = 0x00000004,
    PRC_ALIGN_BOTTOM        = 0x00000008,
    PRC_ALIGN_CENTRE        = 0x00000000,
    PRC_ALIGN_TOPLEFT       = 0x00000005,
    PRC_ALIGN_TOPRIGHT      = 0x00000006,
    PRC_ALIGN_BOTTOMLEFT    = 0x00000009,
    PRC_ALIGN_BOTTOMRIGHT   = 0x0000000A
};

#endif /* PRC_TYPES_H */