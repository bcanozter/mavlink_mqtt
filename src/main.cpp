#include <chrono>
#include <iostream>
#include <string>

#include "mqtt/async_client.h"
#include "mqtt_client.hpp"
#include "mavlink_manager.hpp"

const std::string DFLT_SERVER_URI{"ssl://localhost:18884"};
MqttClient mqtt_client(DFLT_SERVER_URI, "mavlink_mqtt_client", mqtt::NO_PERSISTENCE);
static MavlinkManager mavlink_manager;

void run_mqtt(MqttClient &mqtt)
{
    mqtt.task_loop();
}

void run_mavlink(MavlinkManager &mav)
{
    mav.task_loop();
}

int main(int argc, char *argv[])
{
    std::thread mqtt_thread(run_mqtt, std::ref(mqtt_client));
    std::thread mavlink_thread(run_mavlink, std::ref(mavlink_manager));
    mqtt_thread.join();
    mavlink_thread.join();
    return 0;
}