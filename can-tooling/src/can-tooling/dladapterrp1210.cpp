#include "can-tooling/dladapterrp1210.h"

DLAdapterRP1210::DLAdapterRP1210(DlaInfo& dla_info) : dla_info_(dla_info)
{
    //TODO RAII
}

short DLAdapterRP1210::RP1210ClientConnect(HWND hwnd_handle, short device_id, const char *fpch_protocol, long tx_buf_size, long rx_buf_size, short app_packet_sizing_incoming_msgs)
{
    if(status_ != STATUS::CONFIGURED)
    {
        return -1;
    }
    short ret_val = 0;
    try
    {
        ret_val = pRP1210_ClientConnect(hwnd_handle, device_id, fpch_protocol, tx_buf_size, rx_buf_size, app_packet_sizing_incoming_msgs);
        if (ret_val < 128)
        {
           status_ = STATUS::CONNECTED;
           return ret_val;
        }
        SetErrorMesg(ret_val);

     }
     catch (std::bad_function_call& ex)
     {
        SetErrorMesgText("Bad function call in client connect!\n");
        SetErrorMesgText(ex.what());
     }

     return ret_val;

}

short DLAdapterRP1210::RP1210ClientDisconnect(short client_id)
{
    if(status_ != STATUS::CONNECTED)
    {
        return -1;
    }
    short ret_val = 0;
    try
    {
        ret_val = pRP1210_ClientDisconnect(client_id);
        if (ret_val < 128)
        {
            status_ = STATUS::DISCONNECTED;
            return ret_val;
        }
        SetErrorMesg(ret_val);
    }
    catch (std::bad_function_call& ex)
    {
        SetErrorMesgText("Bad function call in client disconnect!\n");
        SetErrorMesgText(ex.what());
    }
    return ret_val;
}

short DLAdapterRP1210::RP1210SendMessage(short client_id, char *fpch_client_mesg, short mesg_size, short notify_status_on_tx, short block_on_send)
{
    if(status_ != STATUS::CONNECTED)
    {
        return -1;
    }
    //no try catch in high through put function
    short ret_val = pRP1210_SendMessage(client_id, fpch_client_mesg, mesg_size, notify_status_on_tx, block_on_send);
    if (ret_val > 127)
    {
        SetErrorMesgText("Error in Send Message! " + std::to_string(ret_val) + "\n");
        SetErrorMesg(ret_val);
    }
    return ret_val;
}

short DLAdapterRP1210::RP1210ReadMessage(short client_id, char *fpch_api_mesg, short buf_size, short block_on_send)
{
    if(status_ != STATUS::CONNECTED)
    {
        return -1;
    }
    short ret_val = pRP1210_ReadMessage(client_id, fpch_api_mesg, buf_size, block_on_send);
    if (ret_val > 127)
    {
        SetErrorMesgText("Error in Read Message!\n");
        SetErrorMesg(ret_val);
    }
    return ret_val;
}

void DLAdapterRP1210::RP1210ReadVersion(char *fpch_dll_maj_ver, char *fpch_dll_min_ver, char *fpch_api_maj_ver, char *fpch_api_min_ver)
{
    if(status_ != STATUS::UNCONFIGURED)
    {
        try
        {
            pRP1210_ReadVersion(fpch_dll_maj_ver, fpch_dll_min_ver, fpch_api_maj_ver, fpch_api_min_ver);
        }
        catch (std::bad_function_call& ex)
        {
            SetErrorMesgText("Bad function call in Read Version!\n");
            SetErrorMesgText(ex.what());
        }
    }
}

short DLAdapterRP1210::RP1210ReadDetailedVersion(short client_id, char *fpch_api_ver_info, char *fpch_dll_ver_info, char *fpch_fw_ver_info)
{
    short ret_val = 0;
    if(status_ != STATUS::UNCONFIGURED)
    {

        try
        {
            ret_val = pRP1210_ReadDetailedVersion(client_id, fpch_api_ver_info, fpch_dll_ver_info, fpch_fw_ver_info);
            if (ret_val < 128)
            {
                return ret_val;
            }
            SetErrorMesg(ret_val);
        }
        catch (std::bad_function_call& ex)
        {
            SetErrorMesgText("Bad function call in read detailed version!\n");
            SetErrorMesgText(ex.what());
        }
    }
    return ret_val;
}

short DLAdapterRP1210::RP1210GetErrorMsg(short err_code, char *fpch_desc)
{
    short ret_val = 0;
    if(status_ != STATUS::UNCONFIGURED)
    {
        try
        {
            ret_val = pRP1210_GetErrorMsg(err_code, fpch_desc);
            if (ret_val < 128)
            {
                return ret_val;
            }
            SetErrorMesg(ret_val);
        }
        catch (std::bad_function_call& ex)
        {
            SetErrorMesgText("Bad function call in get error message!\n");
            SetErrorMesgText(ex.what());
        }
    }
    return ret_val;
}

short DLAdapterRP1210::RP1210GetLastErrMsg(short err_code, int *sub_err_code, char *fpch_desc, short client_id)
{
    short ret_val = 0;
    if(status_ != STATUS::UNCONFIGURED)
    {
        try
        {
            ret_val = pRP1210_GetLastErrorMsg(err_code, sub_err_code, fpch_desc, client_id);
            if (ret_val < 128)
            {
                return ret_val;
            }
            SetErrorMesg(ret_val);
        }
        catch (std::bad_function_call& ex)
        {
            SetErrorMesgText("Bad function call in get last error message!\n");
            SetErrorMesgText(ex.what());
        }
    }
    return ret_val;
}

short DLAdapterRP1210::RP1210GetHardwareStatus(short client_id, char *fpch_client_info, short info_size, short block_on_request)
{
    short ret_val = 0;
    if(status_ != STATUS::UNCONFIGURED)
    {
        try
        {
            ret_val = pRP1210_GetHardwareStatus(client_id, fpch_client_info, info_size, block_on_request);
            if (ret_val < 128)
            {
                return ret_val;
            }
            SetErrorMesg(ret_val);
        }
        catch (std::bad_function_call& ex)
        {
            SetErrorMesgText("Bad function call in get hardware status!\n");
            SetErrorMesgText(ex.what());
        }
    }
    return ret_val;
}

short DLAdapterRP1210::RP1210SendCommand(short cmd_num, short client_id, char *fpch_client_cmd, short mesg_size)
{
    if(status_ != STATUS::CONNECTED)
    {
        return -1;
    }
    short ret_val = 0;
    try
    {
        ret_val = pRP1210_SendCommand(cmd_num, client_id, fpch_client_cmd, mesg_size);
        if (ret_val < 128)
        {
            return ret_val;
        }
        SetErrorMesg(ret_val);
    }
    catch (std::bad_function_call& ex)
    {
        SetErrorMesgText("Bad function call in send command!\n");
        SetErrorMesgText(ex.what());
    }
    return ret_val;
}

short DLAdapterRP1210::RP1210Ioctl(short client_id, long ioctl_id, void *input, void *output)
{
    short ret_val;
    if(status_ != STATUS::UNCONFIGURED)
    {
    try
        {
            ret_val = pRP1210_Ioctl(client_id, ioctl_id, input, output);
            if (ret_val < 128)
            {
                return ret_val;
            }
            SetErrorMesg(ret_val);
        }
        catch (std::bad_function_call& ex)
        {
            SetErrorMesgText("Bad function call in ioctl!\n");
            SetErrorMesgText(ex.what());
        }
    }
    return ret_val;
}

void DLAdapterRP1210::SetErrorMesg(short err_code)
{
    std::string mesg;
    mesg.resize(128, '0');
    short ret_val = RP1210GetErrorMsg(err_code, mesg.data());
    if (ret_val < 128)
    {
        error_mesg_.append(mesg);
    }
    else
    {
        std::stringstream ss;
        ss << "Error code " << err_code << " not found!\n";
        SetErrorMesgText(ss.str());
    }
}

void DLAdapterRP1210::SetErrorMesgText(std::string error_mesg)
{
    error_mesg_.append(error_mesg);
}

bool DLAdapterRP1210::LoadDllFile(std::filesystem::path dll_filepath)
{
    dll_ = LoadLibraryW(dll_filepath.filename().c_str());
    if (!LoadMinConfig())
    {
        SetErrorMesgText("Failed loading minimum required function pointers!\n");
        return false;
    }
    bool found = false;
    pRP1210_ReadVersion = LoadDllFunc<void(char*, char*, char*, char*)>("RP1210_ReadVersion", found);
    if (found)
    {
        dll_function_flags_ += static_cast<uint32_t>(RP1210_DLL_FUNCTIONS::READ_VERSION );
    }
    else
    {
        SetErrorMesgText("RP1210_ReadVersion not loaded\n");
    }
    found = false;
    pRP1210_GetErrorMsg = LoadDllFunc<short(short, char*)>("RP1210_GetErrorMsg", found);
    if (found)
    {
        dll_function_flags_ += static_cast<uint32_t>(RP1210_DLL_FUNCTIONS::GET_ERROR_MESSAGE );
    }
    else
    {
        SetErrorMesgText("RP1210_GetErrorMsg not loaded\n");
    }
    found = false;
    pRP1210_ReadDetailedVersion = LoadDllFunc<short(short, char*, char*, char*)>("RP1210_ReadDetailedVersion", found);
    if (found)
    {
        dll_function_flags_ += static_cast<uint32_t>(RP1210_DLL_FUNCTIONS::READ_DETAILED_VERSION );
    }
    else
    {
        SetErrorMesgText("RP1210_ReadDetailedVersion not loaded\n");
    }
    found = false;
    pRP1210_GetLastErrorMsg = LoadDllFunc<short(short, int*, char*, short)>("RP1210_GetLastErrorMsg", found);
    if (found)
    {
        dll_function_flags_ += static_cast<uint32_t>(RP1210_DLL_FUNCTIONS::GET_LAST_ERROR_MESSAGE );
    }
    else
    {
        SetErrorMesgText("RP1210_GetLastErrorMsg not loaded\n");
    }
    found = false;
    pRP1210_GetHardwareStatus = LoadDllFunc<short(short, char*, short, short)>("RP1210_GetHardwareStatus", found);
    if (found)
    {
         dll_function_flags_ += static_cast<uint32_t>(RP1210_DLL_FUNCTIONS::GET_HARDWARE_STATUS );
    }
    else
    {
        SetErrorMesgText("RP1210_GetHardwareStatus not loaded\n");
    }
    found = false;
    pRP1210_Ioctl = LoadDllFunc<short(short, long, void*, void*)>("RP1210_Ioctl", found);
    if (found)
    {
        dll_function_flags_ += static_cast<uint32_t>(RP1210_DLL_FUNCTIONS::IOCTRL );
    }
    else
    {
        SetErrorMesgText("RP1210_Ioctl not loaded\n");
    }

    return true; //at least minimal functions loaded
}

bool DLAdapterRP1210::LoadMinConfig()
{
    bool found = false;
    pRP1210_ClientConnect = LoadDllFunc<short(HWND, short, const char*, long, long, short)>("RP1210_ClientConnect", found);
    if (!found)
    {      
      return false;
    }
    dll_function_flags_ += static_cast<uint32_t>(RP1210_DLL_FUNCTIONS::CLIENT_CONNECT);
    found = false;
    pRP1210_ClientDisconnect = LoadDllFunc<short(short)>("RP1210_ClientDisconnect", found);
    if (!found)
    {      
      return false;
    }
    dll_function_flags_ += static_cast<uint32_t>(RP1210_DLL_FUNCTIONS::CLIENT_DISCONNECT);
    found = false;
    pRP1210_ReadMessage = LoadDllFunc<short(short, char*, short, short)>("RP1210_ReadMessage", found);
    if (!found)
    {       
       return false;
    }
    dll_function_flags_ += static_cast<uint32_t>(RP1210_DLL_FUNCTIONS::READ_MESSAGE);
    found = false;
    pRP1210_SendMessage = LoadDllFunc<short(short, char*, short, short, short)>("RP1210_SendMessage", found);
    if (!found)
    {       
       return false;
    }
    dll_function_flags_ += static_cast<uint32_t>(RP1210_DLL_FUNCTIONS::SEND_MESSAGE);
    found = false;
    pRP1210_SendCommand = LoadDllFunc<short(short, short, char*, short)>("RP1210_SendCommand", found);
    if (!found)
    {       
       return false;
    }
    dll_function_flags_ += static_cast<uint32_t>(RP1210_DLL_FUNCTIONS::SEND_COMMAND);
    return found;
}

void DLAdapterRP1210::ActualVersion()
{
    rp1210_version_ = RP1210_VERSION::NONE;
    if((dll_function_flags_ & RP1210_MINIMUM_VERSION_MASK ) == RP1210_MINIMUM_VERSION_MASK)
    {
        if((dll_function_flags_ & RP1210_VERSION_C_MASK) == RP1210_VERSION_C_MASK)
        {
            rp1210_version_ = RP1210_VERSION::RP1210C;
        }
        else if((dll_function_flags_ & RP1210_VERSION_B_MASK) == RP1210_VERSION_B_MASK)
        {
            rp1210_version_ = RP1210_VERSION::RP1210B;
        }
        else if((dll_function_flags_ & RP1210_VERSION_A_MASK) == RP1210_VERSION_A_MASK)
        {
            rp1210_version_ = RP1210_VERSION::RP1210A;
        }
        else
        {
            rp1210_version_ = RP1210_VERSION::MINIMUM;
        }
    }    
}

bool DLAdapterRP1210::ConvertRecvBuffToStruct(CanFrame &frame, short mesg_size)
{
    switch (protocol_)
    {
    case PROTOCOL::CAN:
        if (!BuffToCan(frame, mesg_size))
        {
            return false;
        }
        break;
    case PROTOCOL::J1939:
        if (!BuffToJ1939(frame, mesg_size))
        {
            return false;
        }
        break;
    default:
        return false;
    }
    return true;
}

bool DLAdapterRP1210::BuffToJ1939(CanFrame &frame, short mesg_size)
{
    const auto min_mesg_size = 10;
    if (mesg_size < min_mesg_size)
    {
        frame.error = true;
        return false;
    }
    auto array_index = 0;
    frame.time = FourBytesMSB(&recv_buff_[array_index]);
    array_index += 4;
    if (echo_enabled_)
    {
        //add code to determine what to do with reception of echo message
        array_index++;
    }
    frame.pgn = ThreeBytesLSB(&recv_buff_[array_index]);
    array_index += 3;
    frame.mesg_priority = recv_buff_[array_index] & 7;
    array_index++;
    frame.sa = recv_buff_[array_index];
    array_index++;
    frame.da = recv_buff_[array_index];
    array_index++;
    for (array_index; array_index < mesg_size; ++array_index)
    {
        frame.data.push_back(recv_buff_[array_index]);
    }
    frame.dlc = frame.data.size();
    frame.error = false;
    return true;
}

bool DLAdapterRP1210::BuffToCan(CanFrame &frame, short mesg_size)
{
    const auto min_mesg_size = 7;
    if (mesg_size < min_mesg_size)
    {
        frame.error = true;
        return false;
    }
    auto array_index = 0;
    frame.time = FourBytesMSB(&recv_buff_[array_index]);
    array_index += 4;
    if (echo_enabled_)
    {
        //add code to determine what to do with reception of echo message
        array_index++;
    }
    uint8_t mesg_type = recv_buff_[array_index];
    array_index++;

    if (mesg_type == 0)
    {
        frame.extended = false;
        frame.mesg_id = TwoBytesMSB(&recv_buff_[array_index]);
        array_index += 2;
    }
    else if (mesg_type == 1)
    {
        frame.extended = true;
        frame.extended = false;
        frame.mesg_id = FourBytesMSB(&recv_buff_[array_index]);
        array_index += 4;
    }
    else
    {
        frame.error = true;
        return false;
    }
    for (array_index; array_index < mesg_size; ++array_index)
    {
        frame.data.push_back(recv_buff_[array_index]);
    }
    frame.dlc = frame.data.size();
    frame.error = false;
    return true;
}

void DLAdapterRP1210::J1939ToBuff(CanFrame &frame, short &bytes_to_send)
{
    send_buff_[0] = (frame.pgn & 0xFF);
    send_buff_[1] = (frame.pgn >> 8) & 0xFF;
    send_buff_[2] = (frame.pgn >> 16) & 0xFF;
    send_buff_[3] = frame.mesg_priority; // need to fix!!!
    send_buff_[4] = frame.sa;
    send_buff_[5] = frame.da;
    bytes_to_send = 6;
    for (uint8_t& ch : frame.data)
    {
        send_buff_[bytes_to_send] = ch;
        bytes_to_send++;
    }
    bytes_to_send--;
}

void DLAdapterRP1210::CanToBuff(CanFrame &frame, short &bytes_to_send)
{
    if (frame.extended)
    {
        send_buff_[0] = 0x01;
        send_buff_[1] = (frame.mesg_id >> 24) & 0xFF;
        send_buff_[2] = (frame.mesg_id >> 16) & 0xFF;
        send_buff_[3] = (frame.mesg_id >> 8) & 0xFF;
        send_buff_[4] = frame.mesg_id & 0xFF;
        bytes_to_send = 5;
    }
    else
    {
        send_buff_[0] = 0x00;
        send_buff_[1] = (frame.mesg_id >> 8) & 0xFF;
        send_buff_[2] = frame.mesg_id & 0xFF;
        bytes_to_send = 3;
    }
    for (uint8_t& ch : frame.data)
    {
        send_buff_[bytes_to_send] = ch;
        bytes_to_send++;
    }
}

bool DLAdapterRP1210::ClearAllFilters()
{
    short ret_val = RP1210SendCommand(static_cast<short>(RP1210_COMMAND_NUMBER::SET_ALL_FILTER_STATES_TO_PASS), client_id_, nullptr, 0);
    if (ret_val > 127)
    {
        SetErrorMesgText("Failed setting clear all filters\n");
        return false;
    }
    return true;
}

bool DLAdapterRP1210::SetProtocolFilters()
{
    switch (protocol_)
    {
    case PROTOCOL::CAN:

        break;
    case PROTOCOL::ISO15765:

        break;
    case PROTOCOL::ISO9141:

        break;
    case PROTOCOL::J1708:

        break;
    case PROTOCOL::J1850_104K:

        break;
    case PROTOCOL::J1850_416K:

        break;
    case PROTOCOL::J1939:

        break;
    case PROTOCOL::J2284:

        break;
    case PROTOCOL::KWP2000:

        break;
    case PROTOCOL::PLC:

        break;

    }

    return true; //not currently used
}

bool DLAdapterRP1210::DestroyConnection()
{
    UnloadDll();
    status_ = STATUS::UNCONFIGURED;
    return FreeLibrary(dll_);
}

bool DLAdapterRP1210::Connect()
{
    client_id_ = RP1210ClientConnect(nullptr, dla_info_.RP1210DeviceId(), dla_info_.RP1210Protocol().c_str(),
            dla_info_.RP1210TxBufSize(), dla_info_.RP1210RxBufSize(), dla_info_.RP1210AppPackSize());
    if (client_id_ > 127)
    {
        return false;
    }
    //set filters for connection type
    if (!ClearAllFilters())
    {
        return false;
    }

    return true;
}

bool DLAdapterRP1210::Disconnect()
{
    short ret_val = pRP1210_ClientDisconnect(client_id_);
    if (ret_val > 127)
    {
        return false;
    }
    return true;
}

void DLAdapterRP1210::UnloadDll()
{
    pRP1210_ClientConnect = nullptr;
    pRP1210_ClientDisconnect = nullptr;
    pRP1210_ReadMessage = nullptr;
    pRP1210_SendMessage = nullptr;
    pRP1210_SendCommand = nullptr;
    pRP1210_ReadVersion = nullptr;
    pRP1210_GetErrorMsg = nullptr;
    pRP1210_GetLastErrorMsg = nullptr;
    pRP1210_GetHardwareStatus = nullptr;
    pRP1210_ReadDetailedVersion = nullptr;
    pRP1210_Ioctl = nullptr;

}

bool DLAdapterRP1210::ReadCANMesg(CanFrame &frame)
{
    short ret_val = RP1210ReadMessage(client_id_, (char*)&recv_buff_, 1796, static_cast<short>(RP1210_BLOCKING_MODE::NON_BLOCKING_IO));
        if ((ret_val == 0) || (ret_val > 127))
        {
            return false;
        }
        if (!ConvertRecvBuffToStruct(frame, ret_val))
        {
            return false;
        }
        return true;
}

bool DLAdapterRP1210::SendCANMesg(CanFrame &frame)
{
    short bytes_to_send = 0;
    switch (protocol_)
    {
    case PROTOCOL::CAN:
        CanToBuff(frame, bytes_to_send);
        break;
    case PROTOCOL::J1939:
        BuffToJ1939(frame, bytes_to_send);
        break;
    default:
        return false;
    }
    short ret_val = RP1210SendMessage(client_id_, (char*)&send_buff_[0], bytes_to_send, 0, static_cast<short>(RP1210_BLOCKING_MODE::NON_BLOCKING_IO));
    if (ret_val < 128)
    {
        return true;;
    }
    SetErrorMesg(ret_val);
    return false;
}

bool DLAdapterRP1210::SetFilter(const CAN_MESG_LEN can_mesg_len, const uint32_t mask, const uint32_t header)
{
    char buffer[9];
    buffer[0] = static_cast<uint8_t>(can_mesg_len);
    buffer[1] = (mask >> 24) & 0xFF;
    buffer[2] = (mask >> 16) & 0xFF;
    buffer[3] = (mask >> 8) & 0xFF;
    buffer[4] = mask & 0xFF;
    buffer[5] = (header >> 24) & 0xFF;
    buffer[6] = (header >> 16) & 0xFF;
    buffer[7] = (header >> 8) & 0xFF;
    buffer[8] = header & 0xFF;

    short ret_val = RP1210SendCommand(static_cast<short>(RP1210_COMMAND_NUMBER::SET_ALL_MESSAGE_FILTERING_FOR_CAN), client_id_, &buffer[0], 9);

    if (ret_val > 127)
    {
        SetErrorMesgText("Error setting the message filters in SetFilter!\n");
        return false;
    }

    return true;
}

bool DLAdapterRP1210::SetMesgIdFilter(uint32_t mesg_id)
{
    return false; //TODO
}

bool DLAdapterRP1210::Configure()
{
    if (!LoadDllFile(dla_info_.RP1210DllFilename()))
    {
        SetErrorMesgText("RP1210Connector - Error loading the dll files\n");
        return false;
    }
    ActualVersion();
    return true;
}
