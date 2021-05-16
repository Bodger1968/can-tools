#include "can-tooling/SendMesgCntl.h"

SendMesgCntl::SendMesgCntl()
{
}

SendMesgCntl::~SendMesgCntl()
{
}

bool SendMesgCntl::SetRepeatMesgTime(int ms)
{
    return false;
}

void SendMesgCntl::AddSingleShotMesg(CanFrame& frame)
{
    out_mesg_fifo_.Push(frame);
}

void SendMesgCntl::AddMultipleSingleShotMesgs(std::vector<std::shared_ptr<CanFrame>>& frames)
{
    for (auto frame : frames)
    {
        out_mesg_fifo_.Push(*frame);
    }    
}

void SendMesgCntl::AddRepeatMesg(CanFrame& frame)
{
    repeat_mesgs_.push_back(frame);
}

void SendMesgCntl::AddRepeatMesgs(std::vector<CanFrame>& frames)
{
    for (CanFrame& frame : frames)
    {
        repeat_mesgs_.push_back(frame);
    }
}

bool SendMesgCntl::ProcessOutMesgs()
{
    if (repeat_mesgs_.empty())
    {
        return false;
    }
    for (CanFrame& frame : repeat_mesgs_)
    {
        out_mesg_fifo_.Push(frame);
    }
    return true;
}
