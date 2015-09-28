#include <iostream>
#include "libEdimax.h"

int main(int argc, char* argv[])
{
    EdimaxDevice device("192.168.176.34");
    EdimaxDevice::PowerInformation powerinfo = device.getCurrentPowerUsage();
    std::cout<<"Watts:"<<powerinfo.watts<<" Amps:"<<powerinfo.amps<<std::endl;
}
