#ifndef DLADAPTERRP1210_H
#define DLADAPTERRP1210_H
#include <functional>
#include <string>
#include <sstream>
#include <filesystem>
#include <windows.h>
#include "rp1210_defs.h"
#include "dla_info.h"
#include "can_frame.h"
#include "array_to_types.h"

enum class STATUS : int
{
    UNCONFIGURED = 0,
    CONFIGURED = 1,
    DISCONNECTED = 2,
    CONNECTED = 3

};


class DLAdapterRP1210
{
public:
    DLAdapterRP1210(DlaInfo& dla_info);
    RP1210_VERSION GetSupportedVersion(){return rp1210_version_;}
    bool DestroyConnection();
    bool Connect();
    bool Disconnect();
    void UnloadDll();
    bool ReadCANMesg(CanFrame& frame);
    bool SendCANMesg( CanFrame& frame);
    std::string GetErrorMesg(){return error_mesg_;}
    bool SetFilter(const CAN_MESG_LEN can_mesg_len, const uint32_t mask, const uint32_t header);
    bool SetMesgIdFilter(uint32_t mesg_id);
    bool Configure();

private:
    short RP1210ClientConnect(HWND hwnd_handle, short device_id, const char* fpch_protocol, long tx_buf_size, long rx_buf_size, short app_packet_sizing_incoming_msgs);
    short RP1210ClientDisconnect(short client_id);
    short RP1210SendMessage(short client_id, char* fpch_client_mesg, short mesg_size, short notify_status_on_tx, short block_on_send);
    short RP1210ReadMessage(short client_id, char* fpch_api_mesg, short buf_size, short block_on_send);
    void RP1210ReadVersion(char* fpch_dll_maj_ver, char* fpch_dll_min_ver, char* fpch_api_maj_ver, char* fpch_api_min_ver);
    short RP1210ReadDetailedVersion(short client_id, char* fpch_api_ver_info, char* fpch_dll_ver_info, char* fpch_fw_ver_info);
    short RP1210GetErrorMsg(short err_code, char* fpch_desc);
    short RP1210GetLastErrMsg(short err_code, int* sub_err_code, char* fpch_desc, short client_id);
    short RP1210GetHardwareStatus(short client_id, char* fpch_client_info, short info_size, short block_on_request);
    short RP1210SendCommand(short cmd_num, short client_id, char* fpch_client_cmd, short mesg_size);
    short RP1210Ioctl(short client_id, long ioctl_id, void* input, void* output);
    void SetErrorMesg(short err_code);
    void SetErrorMesgText(std::string error_mesg);
    template <typename T>
    std::function<T> LoadDllFunc(const std::string& function_name, bool& found);
    bool LoadDllFile(std::filesystem::path dll_filepath);
    bool LoadMinConfig();
    void ActualVersion();
    bool ConvertRecvBuffToStruct(CanFrame& frame, short mesg_size);
    bool BuffToJ1939(CanFrame& frame, short mesg_size);
    bool BuffToCan(CanFrame& frame, short mesg_size);
    void J1939ToBuff(CanFrame& frame, short& bytes_to_send);
    void CanToBuff(CanFrame& frame, short& bytes_to_send);
    bool ClearAllFilters();
    bool SetProtocolFilters();


    HINSTANCE dll_;

    std::function<short(HWND, short, const char*, long, long, short)> pRP1210_ClientConnect = nullptr;
    std::function<short(short)> pRP1210_ClientDisconnect = nullptr;
    std::function<short(short, char*, short, short )> pRP1210_ReadMessage = nullptr;
    std::function<short(short, char*, short, short, short)> pRP1210_SendMessage = nullptr;
    std::function<short(short, short, char*, short)> pRP1210_SendCommand = nullptr;
    std::function<void(char *, char *, char *, char *)> pRP1210_ReadVersion = nullptr;
    std::function<short(short, char *)> pRP1210_GetErrorMsg = nullptr;
    std::function<short(short, int *, char *, short)> pRP1210_GetLastErrorMsg = nullptr;
    std::function<short(short, char *, short, short)> pRP1210_GetHardwareStatus = nullptr;
    std::function<short(short, char *, char *, char *)> pRP1210_ReadDetailedVersion = nullptr;
    std::function<short(short, long, void *, void *)> pRP1210_Ioctl;


    STATUS status_ = STATUS::UNCONFIGURED;
    std::string error_mesg_;
    short client_id_;
    bool echo_enabled_ = false;
    RP1210_VERSION rp1210_version_ = RP1210_VERSION::NONE;
    uint32_t dll_function_flags_ = 0;
    DlaInfo dla_info_;
    PROTOCOL protocol_ = PROTOCOL::CAN;
    unsigned char recv_buff_[1796]{ 0 };
    unsigned char send_buff_[1796]{ 0 };
};
template <typename T>
struct TypeParser {};

template <typename Ret, typename... Args>
struct TypeParser<Ret(Args...)> {
    static std::function<Ret(Args...)> CreateFunction(const FARPROC get_process_id)
    {
        return std::function<Ret(Args...)>(reinterpret_cast<Ret(__stdcall*)(Args...)>(get_process_id));
    }
};


template <typename T>
std::function<T> DLAdapterRP1210::LoadDllFunc(const std::string& function_name, bool& found)
{
    if (dll_ == nullptr)
    {
        exit(EXIT_FAILURE);
    }

    FARPROC get_process_id = GetProcAddress(dll_, function_name.c_str());
    if (get_process_id == nullptr)
    {
        found = false;
        return nullptr;
    }

    found = true;
    return TypeParser<T>::CreateFunction(get_process_id);

}
#endif // DLADAPTERRP1210_H
