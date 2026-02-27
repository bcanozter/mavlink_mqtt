#include <chrono>
#include <iostream>
#include <string>

#include "mqtt/async_client.h"
#include "mqtt_client.hpp"
#include "mavlink_manager.hpp"

static MavlinkManager mavlink_manager;

int main(int argc, char *argv[])
{
    int err_type = mqtt_client.connect();
    if (err_type != 0)
    {
        std::cerr << "Error connecting to mqtt server.." << std::endl;
        return -1;
    }
    std::cout << "Connected to mqtt server" << std::endl;
    mavlink_manager.initUDP("127.0.0.1", 14552);

    return 0;
}