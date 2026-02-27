#include <chrono>
#include <iostream>
#include <string>

#include "mqtt/async_client.h"
#include "mavlink/v2.0/common/mavlink.h"
#include "mqtt_client.hpp"
#include "mavlink_manager.hpp"

const std::string DFLT_SERVER_URI{"mqtt://localhost:1883"};
static const int QOS = 1;

static MqttClient mqtt_client(DFLT_SERVER_URI, "mavlink_mqtt_client", mqtt::NO_PERSISTENCE);
static MavlinkManager mavlink_manager;

int main(int argc, char *argv[])
{
    int err_type = mqtt_client.connect();
    if (err_type != 0)
    {
        std::cerr << "Error connecting to mqtt server.." << std::endl;
        return -1;
    }
    std::cout << "Connected" << std::endl;
    mavlink_manager.initUDP("127.0.0.1", 14552);

    return 0;
}