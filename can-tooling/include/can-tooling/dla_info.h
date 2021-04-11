#ifndef DLA_INFO_H
#define DLA_INFO_H
#include <stdint.h>
#include <filesystem>
#include <string>

class DlaInfo
{
public:
    DlaInfo();
    ~DlaInfo();
    //RP1210 parameters
    void SetRP1210DeviceId(short device_id);
    bool SetRP1210DllFilename(std::string filename);
    void SetRP1210Protocol(std::string protocol);
    void SetRP1210TxBufSize(long tx_buf_size);
    void SetRP1210RxBufSize(long rx_buf_size);
    void SetRP1210AppPackSizeInMesg(short pack_mesg);
    short RP1210DeviceId() { return rp1210_device_id_; }
    std::filesystem::path RP1210DllFilename(){return rp1210_dll_path_;}
    std::string RP1210Protocol() { return rp1210_protocol_; }
    long RP1210TxBufSize() { return rp1210_tx_buf_size_; }
    long RP1210RxBufSize() { return rp1210_rx_buf_size_; }
    short RP1210AppPackSize() { return app_packet_sizing_incoming_msgs_; }

    //PCAN parameters

    //vector parameters

private:
    std::filesystem::path rp1210_dll_path_;
    short rp1210_device_id_ = 0;
    std::string rp1210_protocol_;
    long rp1210_tx_buf_size_ = 0;
    long rp1210_rx_buf_size_ = 0;
    short app_packet_sizing_incoming_msgs_ = 1;

};



#endif // DLA_INFO_H
