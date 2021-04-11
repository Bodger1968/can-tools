#ifndef CAN_FRAME_H
#define CAN_FRAME_H
#include <stdint.h>
#include <vector>
#include "rp1210_defs.h"

struct CanFrame
{
    bool error;
    bool extended;
    PROTOCOL protocol;
    uint8_t mesg_priority;
    uint8_t sa;
    uint8_t da;
    uint32_t time;
    uint32_t mesg_id;
    uint16_t dlc;
    uint32_t pgn;
    std::vector<uint8_t> data;
};

#endif // CAN_FRAME_H
