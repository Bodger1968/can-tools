#include "can-tooling/dla_info.h"


DlaInfo::DlaInfo()
{

}

DlaInfo::~DlaInfo()
{

}

void DlaInfo::SetRP1210DeviceId(short device_id)
{
    rp1210_device_id_ = device_id;
}

bool DlaInfo::SetRP1210DllFilename(std::string filename)
{
    if(std::filesystem::exists(filename))
    {
        rp1210_dll_path_ = filename;
        return true;
    }
    return false;
}

void DlaInfo::SetRP1210Protocol(std::string protocol)
{
    rp1210_protocol_ = protocol;
}

void DlaInfo::SetRP1210TxBufSize(long tx_buf_size)
{
    rp1210_tx_buf_size_ = tx_buf_size;
}

void DlaInfo::SetRP1210RxBufSize(long rx_buf_size)
{
    rp1210_rx_buf_size_ = rx_buf_size;
}
