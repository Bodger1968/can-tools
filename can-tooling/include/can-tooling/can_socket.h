#ifndef CAN_SOCKET_H
#define CAN_SOCKET_H
#include "dla_info.h"
#include "mesg_fifo.h"



enum class STATE : uint8_t
{
    UNCONFIGURED = 0,
    CONFIGURED = 1,
    CONNECTED = 2,
    DISCONNECTED = 3,
    ERROR_STATE = 4
};e


template <typename T>
class CANSocket
{
public:
    CANSocket(DlaInfo& dla_info)
    {
        //TODO RAII
    }
    bool Connect();
    bool Disconnect();
    bool StartListen();
    bool StartTransmit();
    void SetTxInterval(int interval_ms);

protected:



private:

    T connector_;
    DlaInfo dla_info_;

};






#endif // CAN_SOCKET_H
