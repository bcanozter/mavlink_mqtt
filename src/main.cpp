#include <chrono>
#include <iostream>
#include <string>

#include "mqtt/async_client.h"
#include "mavlink/v2.0/common/mavlink.h"
#include "mqtt_client.hpp"

const std::string DFLT_SERVER_URI{"mqtt://localhost:1283"};
static MqttClient mqtt_client(DFLT_SERVER_URI, "mavlink_mqtt_client", mqtt::NO_PERSISTENCE);
static const int QOS = 1;

int main(int argc, char *argv[])
{
    int err_type = mqtt_client.connect();
    if (err_type != 0)
    {
        std::cerr << "Error connecting to mqtt server.." << std::endl;
        return -1;
    }
    std::cout << "Connected" << std::endl;
    mqtt_client.publish("test", "Is anyone listening?", QOS);
    return 0;
}