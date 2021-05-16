#include <iostream>
#include "can-tooling/can_socket.h"
#include "can-tooling/dla_info.h"

int main(int argc,  char** argv)
{
    DlaInfo dla_info;
    dla_info.SetRP1210Protocol("can");
    dla_info.SetRP1210DllFilename("CIL7R32.dll");
    dla_info.SetRP1210DeviceId(1);

    CANSocket<DLAdapterRP1210> socket(dla_info);

    return 0;
}
