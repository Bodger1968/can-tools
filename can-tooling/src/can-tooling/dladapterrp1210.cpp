#include "can-tooling/dladapterrp1210.h"

DLAdapterRP1210::DLAdapterRP1210()
{

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
           return ret_val;
        }
        SetErrorMesg(ret_val);
        status_ = STATUS::CONNECTED;
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
    if(status_ == STATUS::CONFIGURED || status_ == STATUS::CONNECTED || status_ == STATUS::DISCONNECTED)
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
    if(status_ == STATUS::CONFIGURED || status_ == STATUS::CONNECTED || status_ == STATUS::DISCONNECTED)
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
    return ret_val;
}

short DLAdapterRP1210::RP1210GetLastErrMsg(short err_code, int *sub_err_code, char *fpch_desc, short client_id)
{
    short ret_val = 0;
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
    return ret_val;
}

short DLAdapterRP1210::RP1210GetHardwareStatus(short client_id, char *fpch_client_info, short info_size, short block_on_request)
{
    short ret_val = 0;
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
    if (!found) { SetErrorMesgText("RP1210_ReadVersion not loaded\n"); }
    found = false;
    pRP1210_GetErrorMsg = LoadDllFunc<short(short, char*)>("RP1210_GetErrorMsg", found);
    if (!found) { SetErrorMesgText("RP1210_GetErrorMsg not loaded\n"); }
    found = false;
    pRP1210_ReadDetailedVersion = LoadDllFunc<short(short, char*, char*, char*)>("RP1210_ReadDetailedVersion", found);
    if (!found) { SetErrorMesgText("RP1210_ReadDetailedVersion not loaded\n"); }
    found = false;
    pRP1210_GetLastErrorMsg = LoadDllFunc<short(short, int*, char*, short)>("RP1210_GetLastErrorMsg", found);
    if (!found) { SetErrorMesgText("RP1210_GetLastErrorMsg not loaded\n"); }
    found = false;
    pRP1210_GetHardwareStatus = LoadDllFunc<short(short, char*, short, short)>("RP1210_GetHardwareStatus", found);
    if (!found) { SetErrorMesgText("RP1210_GetHardwareStatus not loaded\n"); }
    found = false;
    pRP1210_Ioctl = LoadDllFunc<short(short, long, void*, void*)>("RP1210_Ioctl", found);
    if (!found) { SetErrorMesgText("RP1210_Ioctl not loaded\n"); }

    return true; //at least minimal functions loaded
}

bool DLAdapterRP1210::LoadMinConfig()
{
    bool found = false;
        pRP1210_ClientConnect = LoadDllFunc<short(HWND, short, const char*, long, long, short)>("RP1210_ClientConnect", found);
        if (!found) { return false; }
        found = false;
        pRP1210_ClientDisconnect = LoadDllFunc<short(short)>("RP1210_ClientDisconnect", found);
        if (!found) { return false; }
        found = false;
        pRP1210_ReadMessage = LoadDllFunc<short(short, char*, short, short)>("RP1210_ReadMessage", found);
        if (!found) { return false; }
        found = false;
        pRP1210_SendMessage = LoadDllFunc<short(short, char*, short, short, short)>("RP1210_SendMessage", found);
        if (!found) { return false; }
        found = false;
        pRP1210_SendCommand = LoadDllFunc<short(short, short, char*, short)>("RP1210_SendCommand", found);
        if (!found) { return false; }
        return found;
}
