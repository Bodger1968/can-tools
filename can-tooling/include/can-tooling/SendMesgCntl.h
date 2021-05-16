#ifndef SEND_MESG_H
#define SEND_MESG_H
#include <vector>
#include "can_frame.h"
#include "mesg_fifo.h"

class SendMesgCntl
{
public:
    SendMesgCntl();
    ~SendMesgCntl();   

    bool SetRepeatMesgTime(int ms);
    void AddSingleShotMesg(CanFrame& frame);
    void AddMultipleSingleShotMesgs(std::vector<std::shared_ptr<CanFrame>>& frames);
    void AddRepeatMesg(CanFrame& frame);
    void AddRepeatMesgs(std::vector<CanFrame>& frames); 
    void FillQueue() { ProcessOutMesgs(); }
    MesgFifo<CanFrame>& MesgQueue() { return out_mesg_fifo_; }
    int Size() const { return out_mesg_fifo_.Size(); }

private:
    bool ProcessOutMesgs();

    MesgFifo<CanFrame> out_mesg_fifo_;
    std::vector<CanFrame> repeat_mesgs_;
};

#endif
