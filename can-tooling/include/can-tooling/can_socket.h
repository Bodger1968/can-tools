#ifndef CAN_SOCKET_H
#define CAN_SOCKET_H

#include <memory>
#include <condition_variable>
#include <thread>
#include <chrono>
#include <string>
#include <functional>
#include <future>
#include <thread>
#include <numeric>

#include "dla_info.h"
#include "mesg_fifo.h"
#include "SendMesgCntl.h"
#include "dladapterrp1210.h"
#include "can_frame.h"



enum class STATE : uint8_t
{
    UNCONFIGURED = 0,
    CONFIGURED = 1,
    CONNECTED = 2,
    DISCONNECTED = 3,
    RECEIVING = 4,
    TRANSMITTING = 5,
    RECEIVING_AND_TRANSMITTING = 6,
    ERROR_STATE = 7
};

enum class OPERATION_MODE : uint8_t
{
    STOP,
    RECEIVE_ONLY,
    SEND_ONLY,
    SEND_AND_RECEIVE
};


template <typename T>
class CANSocket
{
public:
    CANSocket(DlaInfo& dla_info) : dla_info_(dla_info), connector_(dla_info)
    {
        //TODO RAII
        recv_mesg_queue_ = std::make_shared<MesgFifo<CanFrame>>();
        send_mesg_queue_ = std::make_shared<SendMesgCntl>();
        if (!connector_.Configure())
        {
            socket_state_ = STATE::ERROR_STATE;
            return;
        }
        socket_state_ = STATE::CONFIGURED;
    }

    bool Connect();
    bool Disconnect();
    bool SetOperationMode(OPERATION_MODE op_mode);
    bool StartComminucation();
    bool StopCommunication();
    void SetTxInterval(int interval_ms);
    void SetRecvMesgTimerValue(int microseconds){avg_time_for_checking_mesg_recv_freq_ = microseconds;}
    //Callbacks
    void RecvMesgCallback(std::function<void(CanFrame&)> f){recv_mesg_callback_ = f;}
    void EndOfSegmentTransferCallback(std::function<void()> f){end_of_segment_transfer_callback_ = f;}
    void SegmentTransferProgressCallback(std::function<void(int)> f){segment_transfer_progress_callback_ = f;}
    void MesgRecvAvgTimeCallback(std::function<void(int)> f){mesg_recv_avg_time_callback_ = f;}

protected:



private:
    void CalcTxSendInterval(int interval_ms);
    bool ReadMesg();
    bool WriteMesg();
    bool SetErrorMesg(std::string error_mesg);
    bool SetReceiveOnlyMode();
    bool SetReceiveSendMode();

    std::string error_mesg_;
    T connector_;
    DlaInfo dla_info_;
    STATE socket_state_ = STATE::UNCONFIGURED;

    int send_mesg_usec_ = 0;
    //Message Queues
    std::shared_ptr<MesgFifo<CanFrame>> recv_mesg_queue_ = nullptr;
    std::shared_ptr<SendMesgCntl> send_mesg_queue_ = nullptr;

    std::atomic<bool> run_recv_loop_ = true;
    std::mutex mutex_;

    //threads and loop control
    std::condition_variable read_control_cv_;
    std::future<bool> read_async_ = std::async(std::launch::async,std::bind(&CANSocket<T>::ReadMesg,this));
    std::future<bool> write_async_ = std::async(std::launch::async,std::bind(&CANSocket<T>::WriteMesg,this));

    //callback functions
    std::function<void(CanFrame&)> recv_mesg_callback_ = nullptr; //auto handling of received messages
    std::function<void()> end_of_segment_transfer_callback_ = nullptr; //signal indicating the end of a segment transfer
    std::function<void(int)> segment_transfer_progress_callback_ = nullptr; //signal to link to progress bars
    std::function<void(int)> mesg_recv_avg_time_callback_ = nullptr; //signal indicating the receive message frequency

    //parameters for estimating receive message frequency
    int avg_time_for_checking_mesg_recv_freq_ = 1000000;
    int mesg_recv_time_interval_microseconds_ = 0;

};

/*
 The connect function connects to the device but not the actual CANbus
 *
 */
template<typename T>
inline bool CANSocket<T>::Connect()
{
    if (socket_state_ == STATE::CONFIGURED || state_ == STATE::DISCONNECTED)
    {
        if (!connector_.Connect())
        {
            socket_state_ = STATE::ERROR_STATE;
            error_mesg_.append("Failed connecting to socket adapter!\n");
            error_mesg_.append(connector_.GetErrorMesg());
            return false;
        }
        socket_state_ = STATE::CONNECTED;


        return true;
    }
    error_mesg_.append("Trying to connect to a socket that is not correctly configured!\n");
    socket_state_ = STATE::ERROR_STATE;
    return false;
}
/*
 The connect function disconnects from the device
 SetCommunicationMode should be set to stop before calling this function
 */
template<typename T>
inline bool CANSocket<T>::Disconnect()
{
    if (socket_state_ != STATE::CONNECTED)
    {
        socket_state_ = STATE::ERROR_STATE;
        return false;
    }
    if (!connector_.Disconnect())
    {
        socket_state_ = STATE::ERROR_STATE;
        return false;
    }
    socket_state_ = STATE::DISCONNECTED;
    return true;
}

template<typename T>
inline bool CANSocket<T>::SetOperationMode(OPERATION_MODE op_mode)
{
    bool mode_set = false;
    switch(op_mode)
    {
    case OPERATION_MODE::RECEIVE_ONLY:
        mode_set = SetReceiveOnlyMode();
        break;
    case OPERATION_MODE::SEND_AND_RECEIVE:
        mode_set = SetReceiveSendMode();
        break;
    case OPERATION_MODE::SEND_ONLY:

        break;
    case OPERATION_MODE::STOP:

        break;
    }

    return mode_set;
}

template<typename T>
inline void CANSocket<T>::SetTxInterval(int interval_ms)
{
    CalcTxSendInterval(interval_ms);
}

template<typename T>
inline void CANSocket<T>::CalcTxSendInterval(int interval_ms)
{

}

template<typename T>
inline bool CANSocket<T>::ReadMesg()
{
    //wait for signal to start reading CAN frames
    std::vector<int> average_time_interval_micro_sec;
    std::unique_lock<std::mutex> lock(mutex_);
    read_control_cv_.wait(lock, [this](){return (socket_state_ == STATE::RECEIVING) || (socket_state_ == STATE::RECEIVING_AND_TRANSMITTING) ? 0 : 1;});
    auto start_time = std::chrono::high_resolution_clock::now();
    while(run_recv_loop_.load())
    {
        CanFrame frame;
        if(!connector_.ReadCANMesg(frame))
        {
            if(frame.error)
            {                
                std::this_thread::sleep_for(std::chrono::milliseconds(250));
                continue;//no message or corrupt message
            }
            socket_state_ = STATE::ERROR_STATE;
            return false; //Read message failure likely terminal problem like socket not configured
        }
        if(recv_mesg_callback_ == nullptr)
        {
            recv_mesg_queue_->Push(frame);
        }
        else
        {
            recv_mesg_callback_(frame);
        }
        auto current_time = std::chrono::high_resolution_clock::now();
        auto duration_micro_secs = std::chrono::duration_cast<std::chrono::microseconds>(current_time - start_time);
        average_time_interval_micro_sec.push_back(duration_micro_secs.count());
        if(duration_micro_secs.count() >  avg_time_for_checking_mesg_recv_freq_)
        {
            mesg_recv_time_interval_microseconds_ = std::accumulate(average_time_interval_micro_sec.begin(),average_time_interval_micro_sec.end(),0) / average_time_interval_micro_sec.size();
            average_time_interval_micro_sec.clear();
            start_time = current_time;
            if(mesg_recv_avg_time_callback_ != nullptr)
            {
                mesg_recv_avg_time_callback_(mesg_recv_time_interval_microseconds_);
            }
        }
    }
    //set socket state

    return true;
}

template<typename T>
inline bool CANSocket<T>::WriteMesg()
{
    return true;
}

template<typename T>
inline bool CANSocket<T>::SetReceiveOnlyMode()
{
    bool success = false;
    switch(socket_state_)
    {
    case STATE::CONFIGURED:
        success = false;
        break;
    case STATE::CONNECTED:
        socket_state_ = STATE::RECEIVING;
        success = true;
        break;
    case STATE::DISCONNECTED:
        socket_state_ = STATE::RECEIVING;
        success = true;
        break;
    case STATE::ERROR_STATE:
        success = false;
        break;
    case STATE::RECEIVING:
        success = true;
        break;
    case STATE::RECEIVING_AND_TRANSMITTING:
        socket_state_ = STATE::RECEIVING;
        success = true;
        break;
    case STATE::TRANSMITTING:
        socket_state_ = STATE::RECEIVING;
        success = true;
        break;
    case STATE::UNCONFIGURED:
        success = false;
        break;
    }

    return true;
}

template<typename T>
inline bool CANSocket<T>::SetReceiveSendMode()
{
    bool success = false;
    switch(socket_state_)
    {
    case STATE::CONFIGURED:
        success = false;
        break;
    case STATE::CONNECTED:
        socket_state_ = STATE::RECEIVING_AND_TRANSMITTING;
        success = true;
        break;
    case STATE::DISCONNECTED:
        success = false;
        break;
    case STATE::ERROR_STATE:
        success = false;
        break;
    case STATE::RECEIVING:
        socket_state_ = STATE::RECEIVING_AND_TRANSMITTING;
        success = true;
        break;
    case STATE::RECEIVING_AND_TRANSMITTING:
        success = true;
        break;
    case STATE::TRANSMITTING:
        socket_state_ = STATE::RECEIVING_AND_TRANSMITTING;
        success = true;
        break;
    case STATE::UNCONFIGURED:
        success = false;
        break;
    }

    return true;
}










#endif // CAN_SOCKET_H
