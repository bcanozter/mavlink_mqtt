#pragma once

#include <string>

class MavlinkManager
{
public:
    int initUDP(int port);
    int initSerial(std::string device, int baud);

private:
};